#include "UIManager.h"
#include "InputHandler.h"
#include "GameEngine.h"
#include "Habitat.h"
#include "Renderer.h"

TFT_eSPI* UIManager::_tft = nullptr;
UIManager::UIState UIManager::_currentState = UIManager::UIState::IDLE;

int UIManager::_mainMenuIdx = 0;
const char* UIManager::_mainMenuItems[UIManager::MAIN_MENU_COUNT] = {"ALIMENTACAO", "TREINO & LAZER", "CUIDADOS", "SISTEMA"};

int UIManager::_subMenuIdx = 0;
int UIManager::_subMenuCount = 0;
const char* UIManager::_subMenuItems[4] = {"", "", "", ""};
static char megaByteLabel[20];

unsigned long UIManager::_lastInteractionTime = 0;

// BITMAPS EM PIXEL ART 16x16 (1-bit)

// Coxinha de carne Digimon clássica
const uint8_t PROGMEM icon_feed[32] = {
    0b00011111, 0b11100000,
    0b00111111, 0b11110000,
    0b01111111, 0b11111000,
    0b01111111, 0b11111000,
    0b11111111, 0b11111100,
    0b11111111, 0b11111100,
    0b11111111, 0b11111100,
    0b01111111, 0b11111000,
    0b01111111, 0b11111000,
    0b00111111, 0b11110000,
    0b00011111, 0b11100000,
    0b00000011, 0b00000000,
    0b00000011, 0b00000000,
    0b00000111, 0b11000000,
    0b00000110, 0b01100000,
    0b00000000, 0b00000000
};

// Gamepad retrô com orelhinhas de coração
const uint8_t PROGMEM icon_hack[32] = {
    0b00110000, 0b00001100,
    0b01111000, 0b00011110,
    0b01111000, 0b00011110,
    0b00110111, 0b11101100,
    0b00001111, 0b11110000,
    0b00011111, 0b11111000,
    0b00111111, 0b11111100,
    0b01110011, 0b11001110,
    0b01100011, 0b11000110,
    0b11101111, 0b11110111,
    0b11101111, 0b11110111,
    0b11100111, 0b11100111,
    0b01110000, 0b00001110,
    0b00111111, 0b11111100,
    0b00011111, 0b11111000,
    0b00000111, 0b11100000
};

// Escovinha de limpeza com bolhinhas flutuantes
const uint8_t PROGMEM icon_care[32] = {
    0b00000110, 0b00000000,
    0b00001111, 0b00001100,
    0b00001111, 0b00011110,
    0b00000110, 0b00011110,
    0b01111111, 0b11101100,
    0b11111111, 0b11110000,
    0b11111111, 0b11110000,
    0b10000000, 0b00010000,
    0b10110110, 0b11010000,
    0b10110110, 0b11010000,
    0b10110110, 0b11010000,
    0b10110110, 0b11010000,
    0b10110110, 0b11010000,
    0b10110110, 0b11010000,
    0b11111111, 0b11110000,
    0b01111111, 0b11100000
};

// Monitor retrô feliz piscando (^_^)
const uint8_t PROGMEM icon_sys[32] = {
    0b01111111, 0b11111110,
    0b11111111, 0b11111111,
    0b11000000, 0b00000011,
    0b11010000, 0b00010011,
    0b11000000, 0b00000011,
    0b11010001, 0b00010011,
    0b11001000, 0b00100011,
    0b11000111, 0b11000011,
    0b11000000, 0b00000011,
    0b11111111, 0b11111111,
    0b01111111, 0b11111110,
    0b00000111, 0b11100000,
    0b00000011, 0b11000000,
    0b00000111, 0b11100000,
    0b00011111, 0b11111000,
    0b00111111, 0b11111100
};

void UIManager::init(TFT_eSPI* tft) {
    _tft = tft;
    _currentState = UIState::IDLE;
    _lastInteractionTime = millis();
    drawActionBar();
}

void UIManager::update() {
    if (_currentState == UIState::EXECUTING) return; // Espera terminar
    
    if (_currentState == UIState::STATUS_SCREEN) {
        if (InputHandler::getBtn2LongPress() || InputHandler::getBtn2Click() || InputHandler::getBtn1Click()) {
            exitStatusScreen();
        }
        return;
    }

    // Inatividade para voltar ao IDLE
    if (_currentState != UIState::IDLE) {
        if (millis() - _lastInteractionTime >= MENU_TIMEOUT) {
            _currentState = UIState::IDLE;
            drawActionBar();
        }
    }

    // Lógica do estado IDLE
    if (_currentState == UIState::IDLE) {
        if (InputHandler::getBtn1Click()) {
            // Entra no Menu Principal
            _currentState = UIState::MAIN_MENU;
            _mainMenuIdx = 0;
            _lastInteractionTime = millis();
            drawActionBar();
        } else if (InputHandler::getBtn2Click()) {
            // Atalho fofo: entra direto no Status
            _lastInteractionTime = millis();
            showStatusScreen();
        }
        return;
    }

    // Animação de Bouncing dos ícones no Menu Principal (não bloqueante)
    if (_currentState == UIState::MAIN_MENU) {
        static unsigned long lastAnim = 0;
        if (millis() - lastAnim >= 300) {
            lastAnim = millis();
            drawActionBar(); // Força redesenho para simular pulinho
        }
    }

    // Navegação (Botão 1 cicla opções)
    if (InputHandler::getBtn1Click()) {
        _lastInteractionTime = millis();
        if (_currentState == UIState::MAIN_MENU) {
            _mainMenuIdx = (_mainMenuIdx + 1) % MAIN_MENU_COUNT;
        } else if (_currentState == UIState::SUB_MENU) {
            _subMenuIdx = (_subMenuIdx + 1) % _subMenuCount;
        }
        drawActionBar();
    }

    // Confirmação (Botão 2 curto)
    if (InputHandler::getBtn2Click()) {
        _lastInteractionTime = millis();
        if (_currentState == UIState::MAIN_MENU) {
            enterSubMenu();
        } else if (_currentState == UIState::SUB_MENU) {
            executeAction();
        }
    }

    // Voltar (Botão 2 longo)
    if (InputHandler::getBtn2LongPress()) {
        _lastInteractionTime = millis();
        if (_currentState == UIState::SUB_MENU) {
            exitSubMenu();
        } else if (_currentState == UIState::MAIN_MENU) {
            _currentState = UIState::IDLE;
            drawActionBar();
        }
    }
}

void UIManager::drawActionBar() {
    if (!_tft) return;
    
    // Limpa a Zona 3 de interação (y: 180 a 240)
    // Usamos a cor 0x0842 (azul marinho profundo e fofo) como fundo
    _tft->fillRect(0, 180, 135, 60, 0x0842);
    _tft->drawFastHLine(0, 180, 135, 0x07FF); // Linha ciano separadora

    if (_currentState == UIState::IDLE) {
        // Painel Ocioso Fofo
        _tft->drawRoundRect(3, 183, 129, 54, 5, 0x07FF);
        
        // Status com coração pixelado rosa
        _tft->setTextColor(TFT_WHITE);
        _tft->drawString("STATUS: ACTIVE", 10, 195, 2);
        
        // Desenha um pequeno coração fofo rosa
        _tft->fillRect(100, 193, 2, 2, 0xF81F);
        _tft->fillRect(104, 193, 2, 2, 0xF81F);
        _tft->fillRect(99, 195, 8, 2, 0xF81F);
        _tft->fillRect(100, 197, 6, 2, 0xF81F);
        _tft->fillRect(102, 199, 2, 2, 0xF81F);

        // Localização
        _tft->setTextColor(0x7BEF);
        _tft->setCursor(10, 208);
        _tft->printf("LOC: %s", Habitat::getEnvName());

        // Carteira de Neobits
        _tft->setTextColor(TFT_YELLOW);
        _tft->setCursor(10, 222);
        _tft->printf("[ %d N$ ]", GameEngine::getNeobits());
    }
    else if (_currentState == UIState::MAIN_MENU) {
        // Menu de Ícones pulantes
        static bool animState = false;
        animState = !animState; // Alterna estado a cada frame de desenho
        
        int baseLineY = 188;
        int bounce_0 = baseLineY;
        int bounce_1 = baseLineY;
        int bounce_2 = baseLineY;
        int bounce_3 = baseLineY;

        // Bouncing do ícone ativo
        if (animState) {
            if (_mainMenuIdx == 0) bounce_0 -= 2;
            else if (_mainMenuIdx == 1) bounce_1 -= 2;
            else if (_mainMenuIdx == 2) bounce_2 -= 2;
            else if (_mainMenuIdx == 3) bounce_3 -= 2;
        }

        // Definição de cores (Ciano fosco para inativos, Rosa chiclete fofo para o selecionado)
        uint16_t color_0 = (_mainMenuIdx == 0) ? 0xF81F : 0x1247;
        uint16_t color_1 = (_mainMenuIdx == 1) ? 0xF81F : 0x1247;
        uint16_t color_2 = (_mainMenuIdx == 2) ? 0xF81F : 0x1247;
        uint16_t color_3 = (_mainMenuIdx == 3) ? 0xF81F : 0x1247;

        // Moldura em volta do selecionado
        int selectedX = 14 + _mainMenuIdx * 30;
        _tft->drawRoundRect(selectedX - 3, baseLineY - 5, 22, 22, 3, 0x07FF);

        // Desenha as pixel arts 1-bit
        _tft->drawBitmap(14, bounce_0, icon_feed, 16, 16, color_0);
        _tft->drawBitmap(44, bounce_1, icon_hack, 16, 16, color_1);
        _tft->drawBitmap(74, bounce_2, icon_care, 16, 16, color_2);
        _tft->drawBitmap(104, bounce_3, icon_sys, 16, 16, color_3);

        // Texto com setinhas piscantes abaixo
        _tft->setTextColor(0x07FF);
        _tft->setTextDatum(MC_DATUM);
        char label[30];
        sprintf(label, "> %s <", _mainMenuItems[_mainMenuIdx]);
        _tft->drawString(label, 135/2, 224, 2);
    }
    else if (_currentState == UIState::SUB_MENU) {
        // Submenu Intuitivo com orientações de botões
        _tft->drawRoundRect(8, 185, 119, 48, 5, 0x07FF);
        
        _tft->setTextColor(0xF81F);
        _tft->drawString("<-", 16, 209, 2);
        
        _tft->setTextColor(TFT_WHITE);
        _tft->setTextDatum(MC_DATUM);
        _tft->drawString(_subMenuItems[_subMenuIdx], 135/2, 209, 2);

        _tft->setTextColor(0x07FF);
        _tft->drawString("OK", 115, 209, 2);
    }
}

void UIManager::enterSubMenu() {
    _currentState = UIState::SUB_MENU;
    _subMenuIdx = 0;
    
    switch (_mainMenuIdx) {
        case 0: // ALIMENTACAO
            _subMenuItems[0] = "BITS(Basico)";
            sprintf(megaByteLabel, "MEGA-BYTE(%d)", GameEngine::getMegaBytes());
            _subMenuItems[1] = megaByteLabel;
            _subMenuCount = 2;
            break;
        case 1: // TREINO & LAZER
            _subMenuItems[0] = "CARINHO";
            _subMenuItems[1] = "MINIGAMES";
            _subMenuCount = 2;
            break;
        case 2: // CUIDADOS
            _subMenuItems[0] = "LIMPAR CACHE";
            _subMenuItems[1] = "ANTIVIRUS";
            _subMenuItems[2] = "DORMIR";
            _subMenuCount = 3;
            break;
        case 3: // SISTEMA
            _subMenuItems[0] = "HABITAT";
            _subMenuItems[1] = "STATUS";
            _subMenuCount = 2;
            break;
    }
    drawActionBar();
}

void UIManager::exitSubMenu() {
    _currentState = UIState::MAIN_MENU;
    drawActionBar();
}

void UIManager::executeAction() {
    _currentState = UIState::EXECUTING;
    
    if (_mainMenuIdx == 0) { // ALIMENTACAO
        if (_subMenuIdx == 0) {
            // Trava temporária com sprite comendo mastigando por 3 segundos
            _tft->fillRect(0, 180, 135, 60, 0x0842);
            _tft->setTextColor(TFT_WHITE);
            _tft->setTextDatum(MC_DATUM);
            _tft->drawString("FEEDING...", 135/2, 210, 2);
            GameEngine::alimentar(20);
            
            // Força a renderização do sprite comendo
            unsigned long eatStart = millis();
            int frameToggle = 0;
            while(millis() - eatStart < 3000) {
                // Alterna entre o sprite comendo e o normal
                frameToggle = !frameToggle;
                delay(300);
            }
        }
        else if (_subMenuIdx == 1) {
            if (GameEngine::consumeMegaByte()) {
                _tft->fillRect(0, 180, 135, 60, 0x0842);
                _tft->setTextColor(TFT_WHITE);
                _tft->setTextDatum(MC_DATUM);
                _tft->drawString("FEEDING...", 135/2, 210, 2);
                GameEngine::alimentar(40, 8);
                delay(3000);
            } else {
                _tft->fillRect(0, 180, 135, 60, TFT_RED);
                _tft->setTextColor(TFT_WHITE);
                _tft->setTextDatum(MC_DATUM);
                _tft->drawString("[ SEM ITEM ]", 135/2, 210, 2);
                delay(1000);
            }
        }
    } 
    else if (_mainMenuIdx == 1) { // TREINO & LAZER
        if (_subMenuIdx == 0) {
            GameEngine::brincar(10);
            _tft->fillRect(0, 180, 135, 60, 0x0842);
            _tft->setTextColor(TFT_GREEN);
            _tft->setTextDatum(MC_DATUM);
            _tft->drawString("HAPPY! ♥", 135/2, 210, 2);
            delay(1500);
        }
        else if (_subMenuIdx == 1) {
            // Minigames Menu de Escolha fofo
            _tft->fillRect(0, 180, 135, 60, 0x0842);
            _tft->drawRoundRect(5, 183, 125, 54, 4, 0x07FF);
            _tft->setTextColor(TFT_WHITE);
            _tft->setTextDatum(MC_DATUM);
            _tft->drawString("<- REFLEX | HACK ->", 135/2, 210, 1);
            
            int choice = 0;
            while(choice == 0) {
                InputHandler::update();
                if (InputHandler::getBtn1Click()) choice = 1;
                else if (InputHandler::getBtn2Click()) choice = 2;
                delay(10);
            }
            
            if (choice == 1) {
                playCyberReflex();
            } else {
                playPatternHacker();
            }
        }
    }
    else if (_mainMenuIdx == 2) { // CUIDADOS
        if (_subMenuIdx == 0) {
            GameEngine::limparCache();
            _tft->fillRect(0, 180, 135, 60, 0x0842);
            _tft->setTextColor(TFT_GREEN);
            _tft->setTextDatum(MC_DATUM);
            _tft->drawString("CACHE CLEANED!", 135/2, 210, 2);
            delay(1500);
        }
        else if (_subMenuIdx == 1) {
            GameEngine::curar();
            _tft->fillRect(0, 180, 135, 60, 0x0842);
            _tft->setTextColor(TFT_GREEN);
            _tft->setTextDatum(MC_DATUM);
            _tft->drawString("ANTIVIRUS RUN!", 135/2, 210, 2);
            delay(1500);
        }
        else if (_subMenuIdx == 2) {
            // Dormir natural (Coloca a flag no engine)
            GameEngine::descansar(30);
            _tft->fillRect(0, 180, 135, 60, 0x0842);
            _tft->setTextColor(0x7BEF);
            _tft->setTextDatum(MC_DATUM);
            _tft->drawString("SLEEPING... zZZ", 135/2, 210, 2);
            delay(3000);
        }
    }
    else if (_mainMenuIdx == 3) { // SISTEMA
        if (_subMenuIdx == 0) {
            int nextEnv = (int)Habitat::getCurrentEnv() + 1;
            if (nextEnv > 3) nextEnv = 0;
            Habitat::setEnvironment((Habitat::Environment)nextEnv);
        } else if (_subMenuIdx == 1) {
            showStatusScreen();
            return;
        }
    }

    _currentState = UIState::IDLE;
    drawActionBar();
}

void UIManager::showStatusScreen() {
    _currentState = UIState::STATUS_SCREEN;
    _tft->fillScreen(TFT_BLACK);
    _tft->setTextColor(0x07FF);
    _tft->setTextDatum(TC_DATUM);
    _tft->drawString("--- STATUS ---", 135/2, 10, 2);
    
    _tft->setTextDatum(TL_DATUM);
    _tft->setTextColor(TFT_WHITE);
    
    // Nome e Estágio de Evolução
    _tft->setCursor(5, 30);
    _tft->setTextColor(0xF81F);
    _tft->printf("NAME: %s\n", GameEngine::getPetName());
    _tft->setCursor(5, 48);
    _tft->setTextColor(TFT_WHITE);
    _tft->printf("STAGE: %s\n", GameEngine::getPetStage());
    
    // Fome e Energia
    _tft->setCursor(5, 70);
    _tft->setTextColor(0xFBE0); // Laranja
    _tft->printf("Fome:  %d%%\n", GameEngine::getFome());
    _tft->setCursor(5, 88);
    _tft->setTextColor(0x07FF); // Ciano
    _tft->printf("Nrg:   %d%%\n", GameEngine::getEnergia());
    _tft->setCursor(5, 106);
    _tft->setTextColor(0x7BEF); // Cinza azulado
    _tft->printf("Suj.:  %d%%\n", GameEngine::getSujeira());
    
    // Doença
    _tft->setTextColor(GameEngine::isDoente() ? TFT_RED : TFT_GREEN);
    _tft->setCursor(5, 124);
    _tft->printf("Sys:   %s\n", GameEngine::isDoente() ? "INFECTED" : "CLEAN");
    
    // Eixos de Evolução
    _tft->setTextColor(TFT_YELLOW);
    _tft->setCursor(5, 150);
    _tft->printf("Emo: %d/100\n", GameEngine::getEixo(GameEngine::EMOCAO));
    _tft->setCursor(5, 168);
    _tft->printf("Cor: %d/100\n", GameEngine::getEixo(GameEngine::CORRUPCAO));
    _tft->setCursor(5, 186);
    _tft->printf("Agr: %d/100\n", GameEngine::getEixo(GameEngine::AGRESSIVIDADE));
    
    // Rodapé
    _tft->setTextColor(0x7BEF);
    _tft->setTextDatum(BC_DATUM);
    _tft->drawString("[ANY BTN TO EXIT]", 135/2, 235, 1);
}

void UIManager::exitStatusScreen() {
    _tft->fillScreen(TFT_BLACK);
    Renderer::drawStatusBar(GameEngine::getFome(), GameEngine::getEnergia());
    _tft->drawRect(0, 0, _tft->width(), _tft->height(), 0x39C7);
    
    _currentState = UIState::IDLE;
    drawActionBar();
}

void UIManager::playCyberReflex() {
    _currentState = UIState::EXECUTING;
    
    // Conectando...
    _tft->fillScreen(TFT_BLACK);
    _tft->setTextColor(0x07FF);
    _tft->setTextDatum(TC_DATUM);
    _tft->drawString("CONNECTING...", 135/2, 20, 2);
    
    // Barra de progresso de conexão fofa
    for (int i = 0; i <= 100; i += 10) {
        _tft->drawRect(17, 50, 100, 15, 0x3186);
        _tft->fillRect(19, 52, (i * 96) / 100, 11, 0x07FF);
        delay(100);
    }
    
    int score = 0;
    for (int round = 1; round <= 3; round++) {
        _tft->fillScreen(TFT_BLACK);
        _tft->setTextColor(TFT_WHITE);
        _tft->setTextDatum(TC_DATUM);
        _tft->printf("ROUND %d/3", round);
        _tft->drawString("GET READY...", 135/2, 60, 2);
        
        // Espera aleatória (1.5s a 3.5s) com opção de abortar pelo Botão 1
        unsigned long delayStart = millis();
        unsigned long waitTime = random(1500, 3500);
        bool aborted = false;
        
        while (millis() - delayStart < waitTime) {
            InputHandler::update();
            if (InputHandler::getBtn1Click()) {
                aborted = true;
                break;
            }
            delay(10);
        }
        
        if (aborted) {
            _tft->fillScreen(TFT_BLACK);
            _tft->setTextColor(TFT_RED);
            _tft->drawString("HACK ABORTED", 135/2, 100, 2);
            delay(1000);
            return;
        }
        
        // HACK NOW!
        _tft->fillScreen(0x07E0); // Tela verde neon
        _tft->setTextColor(TFT_BLACK);
        _tft->drawString("HACK NOW!", 135/2, 50, 4);
        
        unsigned long reactStart = millis();
        unsigned long reactWindow = 500; // 500ms para reagir
        bool hit = false;
        
        // Limpa cliques antigos
        InputHandler::update();
        
        while (millis() - reactStart < reactWindow) {
            InputHandler::update();
            if (InputHandler::getBtn1Click()) {
                aborted = true;
                break;
            }
            if (InputHandler::getBtn2Click()) {
                hit = true;
                break;
            }
            // Barra de contagem regressiva esvaziando
            int remaining = reactWindow - (millis() - reactStart);
            _tft->fillRect(10, 110, 115, 15, TFT_BLACK);
            _tft->drawRect(10, 110, 115, 15, TFT_WHITE);
            _tft->fillRect(12, 112, (remaining * 111) / reactWindow, 11, 0xF800);
            delay(10);
        }
        
        if (aborted) {
            _tft->fillScreen(TFT_BLACK);
            _tft->setTextColor(TFT_RED);
            _tft->drawString("HACK ABORTED", 135/2, 100, 2);
            delay(1000);
            return;
        }
        
        _tft->fillScreen(TFT_BLACK);
        _tft->setTextDatum(MC_DATUM);
        
        if (hit) {
            score++;
            _tft->setTextColor(TFT_GREEN);
            _tft->drawString("SUCCESS!", 135/2, 80, 4);
            _tft->setTextColor(TFT_WHITE);
            _tft->drawString("+15 NEOBITS", 135/2, 120, 2);
            GameEngine::addNeobits(15);
            GameEngine::brincar(15);
            delay(1500);
        } else {
            _tft->setTextColor(TFT_RED);
            _tft->drawString("FAILED!", 135/2, 80, 4);
            _tft->setTextColor(TFT_WHITE);
            _tft->drawString("HACK BLOCKED", 135/2, 120, 2);
            GameEngine::falharMinigame();
            delay(1500);
        }
    }
    
    // Fim de jogo
    _tft->fillScreen(TFT_BLACK);
    _tft->setTextColor(TFT_YELLOW);
    _tft->setTextDatum(MC_DATUM);
    _tft->drawString("GAME OVER", 135/2, 60, 4);
    _tft->setTextColor(TFT_WHITE);
    _tft->printf("HACKS: %d/3", score);
    _tft->drawString("RETORNANDO...", 135/2, 150, 2);
    delay(2000);
}

void UIManager::playPatternHacker() {
    _currentState = UIState::EXECUTING;
    
    _tft->fillScreen(TFT_BLACK);
    _tft->setTextColor(TFT_MAGENTA);
    _tft->setTextDatum(TC_DATUM);
    _tft->drawString("PATTERN HACKER", 135/2, 10, 2);
    _tft->setTextColor(TFT_WHITE);
    _tft->drawString("MEMORIZE & INPUT", 135/2, 35, 1);
    
    // Escolha inicial
    _tft->drawRoundRect(10, 60, 115, 60, 5, 0x07FF);
    _tft->drawString("<- START", 25, 75, 2);
    _tft->drawString("   EXIT ->", 25, 95, 2);
    
    bool start = false;
    while(true) {
        InputHandler::update();
        if (InputHandler::getBtn1Click()) { start = true; break; }
        if (InputHandler::getBtn2Click()) { break; }
        delay(10);
    }
    
    if (!start) return;
    
    int maxLevels = 3;
    bool gameWon = true;
    
    for (int lvl = 1; lvl <= maxLevels; lvl++) {
        _tft->fillScreen(TFT_BLACK);
        _tft->setTextColor(TFT_CYAN);
        _tft->setTextDatum(MC_DATUM);
        _tft->printf("LEVEL %d", lvl);
        delay(1000);
        
        // Gera sequência: Lvl 1 = 3, Lvl 2 = 4, Lvl 3 = 5
        int seqLength = lvl + 2;
        int pattern[5];
        for (int i = 0; i < seqLength; i++) {
            pattern[i] = random(1, 3); // 1 = Btn1, 2 = Btn2
        }
        
        // Fase de Exibição
        _tft->fillScreen(TFT_BLACK);
        _tft->setTextColor(TFT_YELLOW);
        _tft->drawString("WATCH PATTERN!", 135/2, 40, 2);
        delay(1000);
        
        for (int i = 0; i < seqLength; i++) {
            _tft->fillScreen(TFT_BLACK);
            _tft->setTextDatum(MC_DATUM);
            if (pattern[i] == 1) {
                _tft->setTextColor(0x07FF); // Cyan
                _tft->drawString("<- [BTN 1]", 135/2, 100, 4);
            } else {
                _tft->setTextColor(0xF81F); // Magenta
                _tft->drawString("[BTN 2] ->", 135/2, 100, 4);
            }
            delay(600);
            _tft->fillScreen(TFT_BLACK);
            delay(200);
        }
        
        // Fase de Entrada
        _tft->fillScreen(TFT_BLACK);
        _tft->setTextColor(0x07FF);
        _tft->drawString("INPUT NOW!", 135/2, 30, 2);
        
        // Desenha caixas de progresso vazias
        for (int i = 0; i < seqLength; i++) {
            _tft->drawRect(15 + i * 20, 80, 16, 16, TFT_WHITE);
        }
        
        int inputIndex = 0;
        bool levelFailed = false;
        unsigned long inputStart = millis();
        unsigned long inputLimit = 5000; // 5 segundos
        
        // Limpa cliques antigos
        InputHandler::update();
        
        while (inputIndex < seqLength && !levelFailed) {
            if (millis() - inputStart >= inputLimit) {
                levelFailed = true;
                break;
            }
            
            // Desenha contagem regressiva
            int rem = inputLimit - (millis() - inputStart);
            _tft->fillRect(10, 120, 115, 10, TFT_BLACK);
            _tft->drawRect(10, 120, 115, 10, 0x3186);
            _tft->fillRect(12, 122, (rem * 111) / inputLimit, 6, TFT_RED);
            
            // Abortamento por clique simultâneo longo
            if (digitalRead(0) == LOW && digitalRead(35) == LOW) {
                unsigned long doublePressStart = millis();
                bool doublePressSuccess = true;
                while (millis() - doublePressStart < 1000) {
                    if (digitalRead(0) == HIGH || digitalRead(35) == HIGH) {
                        doublePressSuccess = false;
                        break;
                    }
                    delay(10);
                }
                if (doublePressSuccess) {
                    _tft->fillScreen(TFT_BLACK);
                    _tft->setTextColor(TFT_RED);
                    _tft->drawString("ABORTING...", 135/2, 100, 2);
                    delay(1000);
                    return;
                }
            }
            
            InputHandler::update();
            int btnPressed = 0;
            if (InputHandler::getBtn1Click()) btnPressed = 1;
            else if (InputHandler::getBtn2Click()) btnPressed = 2;
            
            if (btnPressed > 0) {
                inputStart = millis(); // Reinicia tempo
                
                if (btnPressed == pattern[inputIndex]) {
                    // Acertou! Desenha seta
                    _tft->fillRect(15 + inputIndex * 20, 80, 16, 16, TFT_GREEN);
                    _tft->setTextColor(TFT_BLACK);
                    _tft->setTextDatum(MC_DATUM);
                    _tft->drawString(btnPressed == 1 ? "<-" : "->", 15 + inputIndex * 20 + 8, 88, 1);
                    inputIndex++;
                } else {
                    // Errou!
                    _tft->fillRect(15 + inputIndex * 20, 80, 16, 16, TFT_RED);
                    _tft->setTextColor(TFT_WHITE);
                    _tft->setTextDatum(MC_DATUM);
                    _tft->drawString("X", 15 + inputIndex * 20 + 8, 88, 1);
                    levelFailed = true;
                }
            }
            delay(10);
        }
        
        if (levelFailed) {
            gameWon = false;
            break;
        }
        
        _tft->fillScreen(TFT_BLACK);
        _tft->setTextColor(TFT_GREEN);
        _tft->drawString("LEVEL MATCHED!", 135/2, 100, 2);
        delay(1200);
    }
    
    _tft->fillScreen(TFT_BLACK);
    _tft->setTextDatum(MC_DATUM);
    
    if (gameWon) {
        _tft->setTextColor(TFT_GREEN);
        _tft->drawString("SYSTEM HACKED!", 135/2, 80, 4);
        _tft->setTextColor(TFT_WHITE);
        _tft->drawString("+25 NEOBITS", 135/2, 120, 2);
        GameEngine::addNeobits(25);
        GameEngine::brincar(30);
        delay(2000);
    } else {
        _tft->setTextColor(TFT_RED);
        _tft->drawString("ALERT: GLITCHED", 135/2, 80, 4);
        _tft->setTextColor(TFT_WHITE);
        _tft->drawString("FIREWALL BLOCKED", 135/2, 120, 2);
        GameEngine::falharMinigame();
        delay(2000);
    }
}
