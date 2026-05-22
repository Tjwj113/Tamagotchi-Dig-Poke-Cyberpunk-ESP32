#include "GameEngine.h"
#include "Habitat.h"
#include "sprites/Evolutions.h"
#include <Preferences.h>

Preferences preferences;

// Inicialização de membros estáticos
int GameEngine::_fome = 100;
int GameEngine::_energia = 100;
int GameEngine::_desconfianca = 0;
int GameEngine::_traumaCount = 0;
int GameEngine::_eixos[6] = {50, 50, 50, 0, 50, 50}; // Valores iniciais neutros (Corrupção começa em 0)
int GameEngine::_neobits = 50; // Começa com um pouco de grana
int GameEngine::_inventory[3] = {0, 0, 0};
int GameEngine::_petID = 0; 
bool GameEngine::_isSleeping = false;
bool GameEngine::_hasEvolved = false;
GameEngine::EmotionState GameEngine::_currentState = GameEngine::EmotionState::CONTENTE;
unsigned long GameEngine::_lastTick = 0;
unsigned long GameEngine::_sleepStartTime = 0;

void GameEngine::init() {
    _lastTick = millis();
    _neobits = 0;
    loadState(); // O progresso (incluindo _petID) é recuperado aqui
    computeState();
    Serial.println(F("GameEngine: Inicializada."));
}

void GameEngine::saveState() {
    preferences.begin("cyberpet", false);
    preferences.putInt("fome", _fome);
    preferences.putInt("enrg", _energia);
    preferences.putInt("neobits", _neobits);
    preferences.putInt("desc", _desconfianca);
    preferences.putInt("trmc", _traumaCount);
    preferences.putBytes("eixos", _eixos, sizeof(_eixos));
    preferences.putInt("petID", _petID);
    preferences.putInt("env", (int)Habitat::getCurrentEnv());
    preferences.putBytes("inv", _inventory, sizeof(_inventory));
    preferences.end();
    Serial.println(F("NVS: Progresso salvo com sucesso."));
}

void GameEngine::loadState() {
    preferences.begin("cyberpet", true);
    // Se a chave não existir, usa o valor padrão fornecido no segundo argumento
    _fome = preferences.getInt("fome", 100);
    _energia = preferences.getInt("enrg", 100);
    _neobits = preferences.getInt("neobits", 0);
    _desconfianca = preferences.getInt("desc", 0);
    _traumaCount = preferences.getInt("trmc", 0);
    _petID = preferences.getInt("petID", 0);
    preferences.getBytes("inv", _inventory, sizeof(_inventory));
    
    // Verifica se há dados de eixos
    if (preferences.getBytesLength("eixos") == sizeof(_eixos)) {
        preferences.getBytes("eixos", _eixos, sizeof(_eixos));
    }
    
    int env = preferences.getInt("env", 0);
    Habitat::setEnvironment((Habitat::Environment)env);
    
    preferences.end();
    Serial.println(F("NVS: Dados carregados da memória flash."));
}

void GameEngine::update() {
    unsigned long now = millis();
    static unsigned long _lastSave = 0;

    if (now - _lastTick >= TICK_INTERVAL) {
        updateNeeds();
        computeState();
        checkEvolution();
        _lastTick = now;
        
        Serial.printf("TICK: Fome:%d | Energia:%d | Estado:%d | Env:%d\n", 
                      _fome, _energia, (int)_currentState, (int)Habitat::getCurrentEnv());
    }

    // Auto-save a cada 5 minutos
    if (now - _lastSave >= 300000) {
        saveState();
        _lastSave = now;
    }
}

void GameEngine::computeState() {
    if (_isSleeping) {
        _currentState = EmotionState::DORMINDO;
        return;
    }
    // Lógica baseada no Documento de Inicialização (Roadmap 3.0)
    
    if (_eixos[CORRUPCAO] > 70 && _fome < 10) {
        _currentState = EmotionState::EM_COLAPSO;
    } else if (_desconfianca > 50) {
        _currentState = EmotionState::DESCONFIANTE;
    } else if (_energia < 25) {
        _currentState = EmotionState::SONOLENTO;
    } else if (_fome < 30) {
        _currentState = EmotionState::COM_FOME;
    } else if (_eixos[AGRESSIVIDADE] > 70) {
        _currentState = EmotionState::IRRITADO;
    } else if (_eixos[EMOCAO] < 30) {
        _currentState = EmotionState::TRISTE;
    } else if (_eixos[EMOCAO] > 85) {
        _currentState = EmotionState::EUFORICO;
    } else {
        _currentState = EmotionState::CONTENTE;
    }
}

void GameEngine::checkEvolution() {
    int oldID = _petID;
    
    // 1. Verificação de Trauma Crítico
    if (_traumaCount >= 3 && _petID != ID_NULLENTITY) {
        _petID = ID_NULLENTITY;
        _hasEvolved = true;
        return;
    }

    // 2. Evolução Inicial
    if (_petID == ID_NEOLARVA) {
        int bestEixo = -1;
        int maxVal = 0;
        
        if (_eixos[EMOCAO] > 80 && _eixos[EMOCAO] > maxVal) { maxVal = _eixos[EMOCAO]; bestEixo = EMOCAO; }
        if (_eixos[AGRESSIVIDADE] > 80 && _eixos[AGRESSIVIDADE] > maxVal) { maxVal = _eixos[AGRESSIVIDADE]; bestEixo = AGRESSIVIDADE; }
        if (_eixos[CORRUPCAO] > 60 && _eixos[CORRUPCAO] > maxVal) { maxVal = _eixos[CORRUPCAO]; bestEixo = CORRUPCAO; }
        
        if (bestEixo == EMOCAO) _petID = ID_LUMI;
        else if (bestEixo == AGRESSIVIDADE) _petID = ID_RAZR;
        else if (bestEixo == CORRUPCAO) _petID = ID_GLITCH;

        if (_petID != ID_NEOLARVA) {
            _hasEvolved = true;
            return; // Garante que ele pare na forma base este tick
        }
    }

    // 3. Evoluções de Estágio 2
    else if (_petID == ID_LUMI) { 
        if (_eixos[LOGICA] > 80) _petID = ID_SERAPH_EXE;
        else if (_eixos[AGRESSIVIDADE] > 50) _petID = ID_AURELIUS;
        
        if (_petID != ID_LUMI) { _hasEvolved = true; return; }
    }
    else if (_petID == ID_RAZR && _eixos[AGRESSIVIDADE] > 90) {
        _petID = ID_WARHOUND_X;
        _hasEvolved = true; return;
    }
    else if (_petID == ID_GLITCH && _eixos[LOGICA] > 70) {
        _petID = ID_MALWARE_X;
        _hasEvolved = true; return;
    }

    // 4. Redenção
    if (_desconfianca == 0 && _traumaCount > 0 && _petID < ID_RECLUSE) {
        if (_petID == ID_NULLENTITY) _petID = ID_NULLENTITY_RESTORED; 
        else _petID = ID_RECLUSE;
        
        if (_petID != oldID) _hasEvolved = true;
    }
}

void GameEngine::updateNeeds() {
    // Contador para lentificar a queda de status sem afetar o resto do jogo
    static int status_tick = 0;
    status_tick++;
    
    // AJUSTE METABÓLICO: Reduz status a cada 60 ticks (15 minutos)
    if (status_tick >= 60) {
        if (_isSleeping) {
            _energia = min(100, _energia + 12); // Recupera ~12% a cada 15 min (Full em ~2h)
 
            // PENALIDADE POR SONO EXCESSIVO
            // Se dormir por mais de 8 horas (28.800.000 ms), começa a degradar eixos
            if (millis() - _sleepStartTime > 28800000) { 
                _eixos[CORRUPCAO] = min(100, _eixos[CORRUPCAO] + 1);
                _eixos[HUMANIDADE] = max(0, _eixos[HUMANIDADE] - 1);
                Serial.println(F("AVISO: Pet está dormindo demais. Estagnação digital detectada."));
            }
        } else {
            if (_fome > 0) _fome--;
            if (_energia > 0) _energia--;
        }

        if (_fome > 70 && _energia > 70 && _desconfianca > 0) {
            _desconfianca--;
        }
        status_tick = 0;
    }

    // Lógica de Trauma por Negligência
    if (_fome == 0) {
        _eixos[CORRUPCAO] = min(100, _eixos[CORRUPCAO] + 2);
        _desconfianca = min(100, _desconfianca + 1);
        
        if (_desconfianca == 15) {
            _traumaCount++;
            Serial.println(F("TRAUMA: Pet se sente abandonado."));
        }
    }

    // Influência do Habitat (Fase 4)
    Habitat::Environment env = Habitat::getCurrentEnv();
    switch (env) {
        case Habitat::LABORATORIO:
            _eixos[LOGICA] = min(100, _eixos[LOGICA] + 1);
            _eixos[INSTINTO] = max(0, _eixos[INSTINTO] - 1);
            break;
        case Habitat::JARDIM_NEON:
            _eixos[HUMANIDADE] = min(100, _eixos[HUMANIDADE] + 1);
            _eixos[EMOCAO] = min(100, _eixos[EMOCAO] + 1);
            break;
        case Habitat::FABRICA_INDUSTRIAL:
            _eixos[AGRESSIVIDADE] = min(100, _eixos[AGRESSIVIDADE] + 1);
            _eixos[INSTINTO] = min(100, _eixos[INSTINTO] + 1);
            break;
        default: break; // Quarto Cyberpunk é neutro
    }
}

void GameEngine::alimentar(int pts, int emoPts) {
    _fome += pts;
    if (_fome > 100) _fome = 100;
    
    // Alimentar no horário melhora a emoção (conforme doc)
    _eixos[EMOCAO] += emoPts;
    if (_eixos[EMOCAO] > 100) _eixos[EMOCAO] = 100;
}

void GameEngine::brincar(int pts) {
    _eixos[EMOCAO] = min(100, _eixos[EMOCAO] + pts);
    _eixos[INSTINTO] = min(100, _eixos[INSTINTO] + pts);
    _energia -= 5; // Brincar gasta energia
    
    if (_energia < 0) _energia = 0;
}

void GameEngine::descansar(int pts) {
    // Agora o botão de descansar apenas inicia o sono
    setSleeping(true);
}

void GameEngine::setSleeping(bool sleeping) {
    _isSleeping = sleeping;
    if (sleeping) _sleepStartTime = millis();
    computeState();
    Habitat::draw(); // Redesenha o habitat para aplicar o escurecimento/clareamento
    Serial.println(sleeping ? F("GameEngine: Pet foi dormir.") : F("GameEngine: Pet acordou."));
}

void GameEngine::addNeobits(int amount) {
    _neobits += amount;
    Serial.printf("ECONOMY: Ganhou %d Neobits. Total: %d\n", amount, _neobits);
}

bool GameEngine::buyItem(ItemType type) {
    int price = 0;
    if (type == ItemType::RACAO_PADRAO) price = 10;
    else if (type == ItemType::MEGA_BYTE) price = 50;
    else if (type == ItemType::BRINQUEDO_DIGITAL) price = 100;

    if (_neobits >= price) {
        _neobits -= price;
        _inventory[(int)type]++;
        Serial.printf("SHOP: Comprou item %d. Saldo: %d\n", (int)type, _neobits);
        return true;
    }
    return false;
}

bool GameEngine::useItem(ItemType type) {
    if (_inventory[(int)type] > 0) {
        _inventory[(int)type]--;
        
        if (type == ItemType::RACAO_PADRAO) alimentar(25, 2);
        else if (type == ItemType::MEGA_BYTE) alimentar(50, 10);
        else if (type == ItemType::BRINQUEDO_DIGITAL) brincar(40);
        
        Serial.printf("ITEM: Usou item %d.\n", (int)type);
        return true;
    }
    return false;
}

int GameEngine::getItemCount(ItemType type) {
    return _inventory[(int)type];
}

const char* GameEngine::getPetName() {
    switch (_petID) {
        case 0: return "NEOLARVA";
        case 1: return "LUMI";
        case 2: return "RAZ-R";
        case 3: return "GLITCH";
        case 4: return "AURELIUS";
        case 5: return "WARHOUND-X";
        case 6: return "MALWARE-X";
        case 7: return "SERAPH.EXE";
        case 8: return "DOMINION";
        case 9: return "RECLUSE";
        case 10: return "PHANTOM G.";
        case 11: return "PHOENIX.DAT";
        case 12: return "PHOENIX.WAR";
        case 13: return "NULLENTITY";
        case 14: return "RESTORED";
        default: return "UNKNOWN";
    }
}

const char* GameEngine::getPetStage() {
    if (_petID == 0) return "STAGE: LARVA";
    if (_petID >= 1 && _petID <= 3) return "STAGE: BASE";
    if (_petID >= 4 && _petID <= 8) return "STAGE: ADULT";
    if (_petID == 13) return "STAGE: VOID";
    return "STAGE: SPECIAL";
}
