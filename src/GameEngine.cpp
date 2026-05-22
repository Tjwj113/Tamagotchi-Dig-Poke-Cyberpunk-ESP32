#include <Arduino.h>
#include "GameEngine.h"
#include "Habitat.h"
#include <Preferences.h>

Preferences preferences;

// Inicialização de membros estáticos
int GameEngine::_fome = 100;
int GameEngine::_energia = 100;
int GameEngine::_desconfianca = 0;
int GameEngine::_traumaCount = 0;
int GameEngine::_eixos[6] = {50, 50, 50, 0, 50, 50}; // Valores iniciais neutros (Corrupção começa em 0)
int GameEngine::_neobits = 0;
int GameEngine::_neobitsDailyEarned = 0;
int GameEngine::_petID = 0; // 0: NeoLarva, 1: LUMI, 2: RAZ-R, 3: GLITCH
int GameEngine::_sujeiraDigital = 0;
bool GameEngine::_isDoente = false;
int GameEngine::_invMegaBytes = 0;
GameEngine::EmotionState GameEngine::_currentState = GameEngine::EmotionState::CONTENTE;
unsigned long GameEngine::_lastTick = 0;

void GameEngine::init() {
    _lastTick = millis();
    _neobits = 0;
    _petID = 0; 
    loadState(); // Tenta carregar progresso salvo
    computeState();
    Serial.println(F("GameEngine: Inicializada."));
}

void GameEngine::saveState() {
    preferences.begin("cyberpet", false);
    preferences.putInt("fome", _fome);
    preferences.putInt("enrg", _energia);
    preferences.putInt("neobits", _neobits);
    preferences.putInt("dailyNeobits", _neobitsDailyEarned);
    preferences.putInt("desc", _desconfianca);
    preferences.putInt("trmc", _traumaCount);
    preferences.putBytes("eixos", _eixos, sizeof(_eixos));
    preferences.putInt("petID", _petID);
    preferences.putInt("env", (int)Habitat::getCurrentEnv());
    preferences.putInt("sujeira", _sujeiraDigital);
    preferences.putBool("doente", _isDoente);
    preferences.putInt("megabytes", _invMegaBytes);
    preferences.end();
    Serial.println(F("NVS: Progresso salvo com sucesso."));
}

void GameEngine::loadState() {
    preferences.begin("cyberpet", true);
    // Se a chave não existir, usa o valor padrão fornecido no segundo argumento
    _fome = preferences.getInt("fome", 100);
    _energia = preferences.getInt("enrg", 100);
    _neobits = preferences.getInt("neobits", 0);
    _neobitsDailyEarned = preferences.getInt("dailyNeobits", 0);
    _desconfianca = preferences.getInt("desc", 0);
    _traumaCount = preferences.getInt("trmc", 0);
    _petID = preferences.getInt("petID", 0);
    
    // Verifica se há dados de eixos
    if (preferences.getBytesLength("eixos") == sizeof(_eixos)) {
        preferences.getBytes("eixos", _eixos, sizeof(_eixos));
    }
    
    int env = preferences.getInt("env", 0);
    Habitat::setEnvironment((Habitat::Environment)env);
    
    _sujeiraDigital = preferences.getInt("sujeira", 0);
    _isDoente = preferences.getBool("doente", false);
    _invMegaBytes = preferences.getInt("megabytes", 0);
    
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
    // Lógica baseada no Documento de Inicialização (Roadmap 3.0)
    
    if (_sujeiraDigital >= 70) {
        _currentState = EmotionState::TRISTE;
    } else if (_eixos[CORRUPCAO] > 70 && _fome < 10) {
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
    // 1. Verificação de Trauma Crítico (Prioridade Máxima)
    if (_traumaCount >= 3 && _petID != 13) {
        _petID = 13; // Evolui para NULLENTITY (Abandono total)
        Serial.println(F("EVOLUÇÃO: O pet se tornou uma NULLENTITY."));
        return;
    }

    // 2. Evolução Inicial (Larva -> Base)
    if (_petID == 0) {
        if (_eixos[EMOCAO] > 80) {
            _petID = 1; // Evolui para LUMI
        } else if (_eixos[AGRESSIVIDADE] > 80) {
            _petID = 2; // Evolui para RAZ-R
        } else if (_eixos[CORRUPCAO] > 60) {
            _petID = 3; // Evolui para GLITCH
        }
    }

    // 3. Evoluções de Estágio 2 e Variantes
    else if (_petID == 1) { // Lumi
        if (_eixos[LOGICA] > 80) _petID = 7; // SERAPH.EXE
        else if (_eixos[AGRESSIVIDADE] > 50) _petID = 4; // AURELIUS
    }
    else if (_petID == 2 && _eixos[AGRESSIVIDADE] > 90) {
        _petID = 5; // WARHOUND-X
    }
    else if (_petID == 3 && _eixos[LOGICA] > 70) {
        _petID = 6; // MALWARE-X
    }

    // 4. Caminho de Redenção (Trauma Recuperado)
    if (_desconfianca == 0 && _traumaCount > 0) {
        if (_petID == 13) _petID = 14; // NULLENTITY -> RESTORED
        else if (_petID < 9) _petID = 9; // Evolui para RECLUSE
    }
}

void GameEngine::updateNeeds() {
    // Contador para lentificar a queda de status sem afetar o resto do jogo
    static int status_tick = 0;
    status_tick++;
    
    // AJUSTE METABÓLICO: Reduz status a cada 60 ticks (60 * 15s = 15 minutos)
    // Isso faz com que 100 pontos durem 1500 minutos (~25 horas)
    if (status_tick >= 60) {
        if (_fome > 0) _fome--;
        if (_energia > 0) _energia--;
        
        // Recuperação passiva de trauma se estiver bem cuidado
        if (_fome > 70 && _energia > 70 && _desconfianca > 0) {
            _desconfianca--;
        }

        // Aumenta sujeira digital levemente
        _sujeiraDigital = min(100, _sujeiraDigital + 1);
        if (_sujeiraDigital > 70 && random(0, 100) < 5) {
            _isDoente = true;
        }

        status_tick = 0;
    }

    // Doença aumenta corrupção
    if (_isDoente) {
        _eixos[CORRUPCAO] = min(100, _eixos[CORRUPCAO] + 1);
    }

    // Lógica de Trauma e Corrupção por Negligência
    if (_fome == 0) {
        // A cada tick com fome zero, aumenta a corrupção e desconfiança
        _eixos[CORRUPCAO] = min(100, _eixos[CORRUPCAO] + 2);
        _desconfianca = min(100, _desconfianca + 1);
        
        if (_desconfianca == 15) { // Primeiro estágio de trauma
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

    // Comer gera sujeira rapidamente
    _sujeiraDigital = min(100, _sujeiraDigital + 15);
    if (_sujeiraDigital > 70 && random(0, 100) < 10) {
        _isDoente = true;
    }
}

void GameEngine::brincar(int pts) {
    _eixos[EMOCAO] = min(100, _eixos[EMOCAO] + pts);
    _eixos[INSTINTO] = min(100, _eixos[INSTINTO] + pts);
    _energia -= 5; // Brincar gasta energia
    
    if (_energia < 0) _energia = 0;
}

void GameEngine::descansar(int pts) {
    _energia = min(100, _energia + pts);
    _eixos[LOGICA] = min(100, _eixos[LOGICA] + 2); // Descanso aumenta lógica
    Serial.println(F("GameEngine: Pet descansou."));
    checkSleepReset();
}

void GameEngine::checkSleepReset() {
    if (_energia >= 100) {
        if (_neobitsDailyEarned > 0) {
            _neobitsDailyEarned = 0;
            saveState();
            Serial.println(F("GameEngine: Limite diario de NEOBITS reiniciado (Sono concluido!)."));
        }
    }
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

void GameEngine::addNeobits(int qtd) {
    int capRemaining = 150 - _neobitsDailyEarned;
    if (capRemaining <= 0) {
        Serial.println(F("GameEngine: CAP ALCANCADO (Daily cap reached)"));
        return;
    }
    if (qtd > capRemaining) {
        qtd = capRemaining;
    }
    _neobits += qtd;
    _neobitsDailyEarned += qtd;
    Serial.printf("GameEngine: +%d Neobits (Total: %d, Daily: %d/150)\n", qtd, _neobits, _neobitsDailyEarned);
    saveState();
}

void GameEngine::limparCache() {
    _sujeiraDigital = 0;
    Serial.println(F("GameEngine: Cache limpo!"));
}

void GameEngine::curar() {
    if (_isDoente) {
        _isDoente = false;
        _eixos[EMOCAO] = min(100, _eixos[EMOCAO] + 10);
        Serial.println(F("GameEngine: Pet curado do virus!"));
    }
}

void GameEngine::addMegaByte(int qtd) {
    _invMegaBytes += qtd;
}

bool GameEngine::consumeMegaByte() {
    if (_invMegaBytes > 0) {
        _invMegaBytes--;
        return true;
    }
    return false;
}

void GameEngine::falharMinigame() {
    _eixos[EMOCAO] = min(100, _eixos[EMOCAO] + 5);
    _energia -= 5;
    if (_energia < 0) _energia = 0;
    _eixos[AGRESSIVIDADE] = min(100, _eixos[AGRESSIVIDADE] + 10);
    Serial.println(F("GameEngine: Pet falhou no minigame (frustrado)."));
}

void GameEngine::darBronca() {
    _eixos[AGRESSIVIDADE] -= 20;
    if (_eixos[AGRESSIVIDADE] < 0) _eixos[AGRESSIVIDADE] = 0;
    
    _eixos[CORRUPCAO] -= 10;
    if (_eixos[CORRUPCAO] < 0) _eixos[CORRUPCAO] = 0;
    
    _eixos[EMOCAO] -= 15;
    if (_eixos[EMOCAO] < 0) _eixos[EMOCAO] = 0;
    
    Serial.println(F("GameEngine: Pet recebeu bronca."));
}
