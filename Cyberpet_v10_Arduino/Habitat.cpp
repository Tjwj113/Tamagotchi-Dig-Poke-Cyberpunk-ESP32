#include "Habitat.h"
#include "GameEngine.h"

TFT_eSPI* Habitat::_tft = nullptr;
Habitat::Environment Habitat::_currentEnv = Habitat::QUARTO_CYBERPUNK;

void Habitat::init(TFT_eSPI* tft) {
    _tft = tft;
}

void Habitat::setEnvironment(Environment env) {
    _currentEnv = env;
    draw(); 
}

const char* Habitat::getEnvName() {
    switch (_currentEnv) {
        case QUARTO_CYBERPUNK: return "QUARTO CYBER";
        case LABORATORIO:     return "LABORATORIO";
        case JARDIM_NEON:     return "JARDIM NEON";
        case FABRICA_INDUSTRIAL: return "FABRICA IND";
        default: return "DESCONHECIDO";
    }
}

void Habitat::draw() {
    if (!_tft) return;
    drawBackground();
}

#define HAB_AREA_Y  40
#define HAB_AREA_H  160
#define SCR_W       135

uint16_t Habitat::getGlowColor() {
    if (GameEngine::isSleeping()) return 0x0000; // Grade fica totalmente escura (desligada) no sono
    switch (_currentEnv) {
        case LABORATORIO: return 0x0010;
        case JARDIM_NEON: return 0x0100;
        case FABRICA_INDUSTRIAL: return 0x2000;
        default: return 0x1002;
    }
}

void Habitat::drawArea(int x, int y, int w, int h) {
    if (!_tft) return;
    
    uint16_t glowColor = getGlowColor();

    _tft->fillRect(x, y, w, h, TFT_BLACK);
    
    // Alinhamento global (y % 15)
    for (int gy = HAB_AREA_Y; gy < HAB_AREA_Y + HAB_AREA_H; gy += 15) {
        if (gy >= y && gy < y + h) {
            _tft->drawFastHLine(x, gy, w, glowColor);
        }
    }
    for (int gx = 0; gx < SCR_W; gx += 20) {
        if (gx >= x && gx < x + w) {
            _tft->drawFastVLine(gx, y, h, glowColor);
        }
    }
}

void Habitat::drawBackground() {
    if (!_tft) return;

    uint16_t gridColor = 0x39C7;
    uint16_t glowColor = 0x2104;
    bool isSleeping = GameEngine::isSleeping();

    switch (_currentEnv) {
        case LABORATORIO: gridColor = isSleeping?0x001F:0x07FF; glowColor = isSleeping?0x0002:0x0010; break;
        case JARDIM_NEON: gridColor = isSleeping?0x03E0:0x07E0; glowColor = isSleeping?0x0040:0x0100; break;
        case FABRICA_INDUSTRIAL: gridColor = isSleeping?0x7BE0:0xFBE0; glowColor = isSleeping?0x0800:0x2000; break;
        default: gridColor = isSleeping?0x780F:0xF81F; glowColor = isSleeping?0x0801:0x1002; break;
    }

    _tft->fillRect(0, HAB_AREA_Y, SCR_W, HAB_AREA_H, TFT_BLACK);
    for (int y = HAB_AREA_Y; y < HAB_AREA_Y + HAB_AREA_H; y += 15) {
        _tft->drawFastHLine(0, y, SCR_W, glowColor);
    }
    for (int x = 0; x < SCR_W; x += 20) {
        _tft->drawFastVLine(x, HAB_AREA_Y, HAB_AREA_H, glowColor);
    }
    _tft->drawRect(0, HAB_AREA_Y, SCR_W, HAB_AREA_H, gridColor);
    _tft->drawFastHLine(0, HAB_AREA_Y + HAB_AREA_H - 1, SCR_W, gridColor);
}
