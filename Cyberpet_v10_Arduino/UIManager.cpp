#include "UIManager.h"
#include "InputHandler.h"
#include "GameEngine.h"
#include "Habitat.h"

TFT_eSPI* UIManager::_tft = nullptr;
int UIManager::_menuIdx = 0;
const char* UIManager::_menuItems[6] = {"ALIMENTAR", "CARINHO", "MINIGAME", "HABITAT", "DORMIR", "STATUS"};

void UIManager::init(TFT_eSPI* tft) {
    _tft = tft;
    drawActionBar();
}

void UIManager::update() {
    // Se o pet estiver dormindo, qualquer botão serve apenas para acordá-lo
    if (GameEngine::isSleeping()) {
        if (InputHandler::getBtn1Click() || InputHandler::getBtn2Click()) {
            GameEngine::setSleeping(false);
            if (_tft) {
                _tft->fillRect(0, 201, 135, 39, 0x07FF);
                _tft->setTextColor(TFT_BLACK);
                _tft->drawString("BOM DIA!", 135/2, 220, 2);
                delay(800);
            }
            drawActionBar();
        }
        return; // Não processa o menu enquanto dorme
    }

    if (InputHandler::getBtn1Click()) {
        _menuIdx = (_menuIdx + 1) % 6;
        drawActionBar();
    }

    if (InputHandler::getBtn2Click()) {
        executeAction();
    }
}

void UIManager::drawActionBar() {
    if (!_tft) return;
    
    // Área do Menu (Y: 200 a 240)
    _tft->fillRect(0, 201, 135, 39, 0x1082); // Azul Marinho Cyber
    _tft->drawFastHLine(0, 200, 135, 0x07FF); // Linha divisória Neon Cyan

    _tft->setTextColor(TFT_WHITE);
    _tft->setTextDatum(MC_DATUM);
    _tft->drawString(_menuItems[_menuIdx], 135/2, 220, 2);
    
    // Indicadores de Botão
    _tft->setTextColor(0x07FF); // Cyan
    _tft->drawString(">", 10, 220, 2);   // Btn 1: Próximo
    _tft->drawString("OK", 120, 220, 2); // Btn 2: Selecionar
}

void UIManager::executeAction() {
    switch (_menuIdx) {
        case 0: // ALIMENTAR (Sem loja, ração infinita)
            GameEngine::alimentar(25, 2);
            Serial.println(F("UI: Alimentou o Pet."));
            _tft->fillRect(0, 201, 135, 39, TFT_GREEN);
            _tft->setTextColor(TFT_BLACK);
            _tft->drawString("NHAM NHAM", 135/2, 220, 2);
            delay(800);
            break;
        case 1: // CARINHO
            GameEngine::brincar(10);
            _tft->fillRect(0, 201, 135, 39, 0xF81F); // Rosa Pink
            _tft->setTextColor(TFT_BLACK);
            _tft->drawString("<3 <3 <3", 135/2, 220, 2);
            delay(800);
            break;
        case 2: // MINIGAME
            _tft->fillRect(0, 201, 135, 39, TFT_RED);
            _tft->setTextColor(TFT_WHITE);
            _tft->drawString("PREPARE-SE", 135/2, 220, 2);
            delay(random(1000, 2500));
            _tft->fillRect(0, 201, 135, 39, TFT_GREEN);
            _tft->setTextColor(TFT_BLACK);
            _tft->drawString("AGORA!", 135/2, 220, 2);
            {
                unsigned long start = millis();
                bool acerto = false;
                while(millis() - start < 450) { 
                    InputHandler::update();
                    if(InputHandler::getBtn2Click()) { acerto = true; break; }
                }
                if(acerto) {
                    GameEngine::brincar(30);
                    GameEngine::addNeobits(20); // Ganha Neobits ao vencer!
                    _tft->fillRect(0, 201, 135, 39, TFT_CYAN);
                    _tft->setTextColor(TFT_BLACK);
                    _tft->drawString("VENCEU! +$20", 135/2, 220, 2);
                    delay(1000);
                } else {
                    _tft->fillRect(0, 201, 135, 39, TFT_RED);
                    _tft->setTextColor(TFT_WHITE);
                    _tft->drawString("PERDEU...", 135/2, 220, 2);
                    delay(1000);
                }
            }
            break;
        case 3: // HABITAT
            {
                int nextEnv = (int)Habitat::getCurrentEnv() + 1;
                if (nextEnv > 3) nextEnv = 0;
                Habitat::setEnvironment((Habitat::Environment)nextEnv);
                
                _tft->fillRect(0, 201, 135, 39, 0x1082); // Azul Marinho
                _tft->setTextColor(TFT_WHITE);
                _tft->drawString("VIAJANDO...", 135/2, 220, 2);
                delay(600);
            }
            break;
        case 4: // DORMIR
            GameEngine::setSleeping(true);
            _tft->fillRect(0, 201, 135, 39, 0x0000); // Preto
            _tft->setTextColor(TFT_WHITE);
            _tft->drawString("BOA NOITE...", 135/2, 220, 2);
            delay(800);
            break;
        case 5: // STATUS
            {
                _tft->fillScreen(TFT_BLACK);
                _tft->drawRect(0, 0, 135, 240, 0x07FF); // Borda Neon
                
                _tft->setTextColor(TFT_WHITE);
                _tft->setTextDatum(TC_DATUM);
                _tft->drawString("STATUS DO PET", 135/2, 12, 2);
                
                _tft->setTextDatum(TL_DATUM);
                _tft->setTextColor(0x07FF); // Cyan
                _tft->drawString(GameEngine::getPetName(), 10, 40, 2);
                
                _tft->setTextColor(TFT_WHITE);
                _tft->drawString(GameEngine::getPetStage(), 10, 58, 1);
                
                _tft->setTextColor(0xFFE0); // Amarelo
                _tft->setCursor(10, 75);
                _tft->printf("NEOBITS: %d NB", GameEngine::getNeobits());

                // Exibição dos Eixos de Personalidade
                const char* labels[] = {"EMO", "LOG", "AGR", "COR", "HUM", "INS"};
                uint16_t colors[] = {0xF81F, 0x07FF, 0xF800, 0x780F, 0xFFE0, 0xFBE0};
                
                for(int i = 0; i < 6; i++) {
                    int y = 100 + (i * 18);
                    _tft->setTextColor(0x7BEF); // Cinza
                    _tft->drawString(labels[i], 10, y, 1);
                    
                    int val = GameEngine::getEixo((GameEngine::Eixo)i);
                    _tft->drawRect(45, y, 80, 10, 0x3186); // Barra vazia
                    _tft->fillRect(47, y + 2, (val * 76) / 100, 6, colors[i]); // Progresso
                }

                _tft->setTextDatum(BC_DATUM);
                _tft->setTextColor(TFT_WHITE);
                _tft->drawString("VOLTAR [OK]", 135/2, 230, 1);

                delay(500); // Debounce
                while(true) {
                    InputHandler::update();
                    if(InputHandler::getBtn1Click() || InputHandler::getBtn2Click()) break;
                }

                _tft->fillScreen(TFT_BLACK);
                Habitat::draw();
            }
            break;
    }

    drawActionBar();
}
