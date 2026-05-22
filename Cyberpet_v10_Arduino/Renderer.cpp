#include "Renderer.h"
#include "Habitat.h"

TFT_eSPI* Renderer::_tft = nullptr;
TFT_eSprite* Renderer::_canvas = nullptr;

void Renderer::init(TFT_eSPI* tft) {
    _tft = tft;
    if (_canvas == nullptr) {
        _canvas = new TFT_eSprite(tft);
        _canvas->createSprite(96, 96);
    }
}

void Renderer::drawStatusBar(int fome, int energia) {
    if (!_tft) return;
    
    _tft->fillRect(0, 0, 135, 40, 0x0000); 
    _tft->drawFastHLine(0, 39, 135, 0x39C7); 

    _tft->setTextSize(1);
    _tft->setTextColor(0x07FF); 
    _tft->setCursor(5, 8);
    _tft->print("HNG");
    
    _tft->drawRect(35, 7, 90, 10, 0x3186); 
    uint16_t hColor = (fome > 60) ? 0x07E0 : (fome > 25 ? 0xFFE0 : 0xF800);
    _tft->fillRect(37, 9, (fome * 86) / 100, 6, hColor);

    _tft->setTextColor(0xF81F); 
    _tft->setCursor(5, 22);
    _tft->print("NRG");
    
    _tft->drawRect(35, 21, 90, 10, 0x3186);
    uint16_t eColor = (energia > 60) ? 0x07FF : (energia > 25 ? 0xFBE0 : 0xF800);
    _tft->fillRect(37, 23, (energia * 86) / 100, 6, eColor);
}

void Renderer::drawSprite(int x, int y, const uint16_t* sprite, int w, int h, uint16_t transparentColor, int scale, int yOffset) {
    if (!_tft) return;

    if (scale > 1 && _canvas && (w * scale) <= 96 && (h * scale) <= 96) {
        int drawW = w * scale;
        int drawH = h * scale;
        
        // 1. Limpa o fundo no canvas
        _canvas->fillSprite(TFT_BLACK);
        
        // 2. Desenha a grade do Habitat no canvas
        uint16_t glowColor = Habitat::getGlowColor();
        for (int gy = 40; gy < 200; gy += 15) {
            if (gy >= y && gy < y + drawH) {
                _canvas->drawFastHLine(0, gy - y, drawW, glowColor);
            }
        }
        for (int gx = 0; gx < 135; gx += 20) {
            if (gx >= x && gx < x + drawW) {
                _canvas->drawFastVLine(gx - x, 0, drawH, glowColor);
            }
        }

        // 3. Desenha o pet escalado com offset Y (RLE para performance extra no canvas)
        for (int sy = 0; sy < h; sy++) {
            int sx = 0;
            while (sx < w) {
                uint16_t color = pgm_read_word(&sprite[sy * w + sx]);
                if (color != transparentColor) {
                    int runLength = 1;
                    while (sx + runLength < w && pgm_read_word(&sprite[sy * w + sx + runLength]) == color) {
                        runLength++;
                    }
                    // Adiciona o yOffset apenas na hora de preencher os pixels do pet no canvas
                    _canvas->fillRect(sx * scale, (sy * scale) + yOffset, runLength * scale, scale, color);
                    sx += runLength;
                } else {
                    sx++;
                }
            }
        }
        
        // 4. Push final pra tela (elimina 100% do flickering)
        _canvas->pushSprite(x, y);
    } else {
        Habitat::drawArea(x, y, w * scale, h * scale);
        
        if (scale <= 1) {
            if (_canvas && w <= 96 && h <= 96) {
                _canvas->fillSprite(transparentColor); 
                _canvas->pushImage(0, 0, w, h, sprite);
                _canvas->pushSprite(x, y, transparentColor);
            } else {
                _tft->pushImage(x, y, w, h, sprite);
            }
        } else {
            // Fallback: draw directly to screen with RLE
            for (int sy = 0; sy < h; sy++) {
                int sx = 0;
                while (sx < w) {
                    uint16_t color = pgm_read_word(&sprite[sy * w + sx]);
                    if (color != transparentColor) {
                        int runLength = 1;
                        while (sx + runLength < w && pgm_read_word(&sprite[sy * w + sx + runLength]) == color) {
                            runLength++;
                        }
                        _tft->fillRect(x + sx * scale, y + sy * scale, runLength * scale, scale, color);
                        sx += runLength;
                    } else {
                        sx++;
                    }
                }
            }
        }
    }
}

void Renderer::clearArea(int x, int y, int w, int h, uint16_t color) {
    if (!_tft) return;
    _tft->fillRect(x, y, w, h, color);
}
