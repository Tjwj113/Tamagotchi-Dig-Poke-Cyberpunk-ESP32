#include "Habitat.h"

TFT_eSPI* Habitat::_tft = nullptr;
Habitat::Environment Habitat::_currentEnv = Habitat::QUARTO_CYBERPUNK;

void Habitat::init(TFT_eSPI* tft) {
    _tft = tft;
}

void Habitat::setEnvironment(Environment env) {
    _currentEnv = env;
    draw(); // Redesenha ao trocar
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

// ── Constantes de layout ─────────────────────────────────────────
#define HAB_AREA_Y  40
#define HAB_AREA_H  160
#define SCR_W       135

void Habitat::drawArea(int x, int y, int w, int h) {
    if (!_tft) return;
    
    uint16_t gridColor = 0x39C7;
    uint16_t bgColor = TFT_BLACK;

    switch (_currentEnv) {
        case LABORATORIO: gridColor = 0x07FF; break; // Cyan
        case JARDIM_NEON: gridColor = 0x07E0; break; // Verde
        case FABRICA_INDUSTRIAL: gridColor = 0xFBE0; break; // Laranja
        default: gridColor = 0xF81F; break; // Magenta (Quarto)
    }

    // Preenchimento base otimizado
    _tft->fillRect(x, y, w, h, bgColor);

    // Grid Minimalista (Apenas linhas horizontais para performance)
    for (int gy = y; gy < y + h; gy += 20) {
        _tft->drawFastHLine(x, gy, w, gridColor);
    }
}

void Habitat::drawBackground() {
    if (!_tft) return;

    uint16_t gridColor = 0x39C7;
    uint16_t glowColor = 0x2104;

    switch (_currentEnv) {
        case LABORATORIO: gridColor = 0x07FF; glowColor = 0x0010; break;
        case JARDIM_NEON: gridColor = 0x07E0; glowColor = 0x0100; break;
        case FABRICA_INDUSTRIAL: gridColor = 0xFBE0; glowColor = 0x2000; break;
        default: gridColor = 0xF81F; glowColor = 0x1002; break;
    }

    // 1. Fundo Limpo
    _tft->fillRect(0, HAB_AREA_Y, SCR_W, HAB_AREA_H, TFT_BLACK);

    // 2. Grid Cibernético (Perspectiva Simples)
    for (int y = HAB_AREA_Y; y < HAB_AREA_Y + HAB_AREA_H; y += 15) {
        _tft->drawFastHLine(0, y, SCR_W, glowColor);
    }
    for (int x = 0; x < SCR_W; x += 20) {
        _tft->drawFastVLine(x, HAB_AREA_Y, HAB_AREA_H, glowColor);
    }

    // 3. Moldura Neon da Área Principal
    _tft->drawRect(0, HAB_AREA_Y, SCR_W, HAB_AREA_H, gridColor);
    
    // 4. Efeito de Horizonte
    _tft->drawFastHLine(0, HAB_AREA_Y + HAB_AREA_H - 1, SCR_W, gridColor);
}

