#include "Renderer.h"
#include "Habitat.h"

TFT_eSPI* Renderer::_tft = nullptr;
TFT_eSprite* Renderer::_canvas = nullptr;

void Renderer::init(TFT_eSPI* tft) {
    _tft = tft;
    if (_canvas == nullptr) {
        _canvas = new TFT_eSprite(tft);
        _canvas->createSprite(32, 32);
    }
}

void Renderer::drawStatusBar(int fome, int energia) {
    if (!_tft) return;
    
    // Área Superior (135x40)
    _tft->fillRect(0, 0, 135, 40, 0x0000); // Fundo Total Preto
    _tft->drawFastHLine(0, 39, 135, 0x39C7); // Linha divisória Neon

    // Barra de Fome (HNG)
    _tft->setTextSize(1);
    _tft->setTextColor(0x07FF); // Cyan
    _tft->setCursor(5, 8);
    _tft->print("HNG");
    
    // Background da barra
    _tft->drawRect(35, 7, 90, 10, 0x3186); // Borda escura
    uint16_t hColor = (fome > 60) ? 0x07E0 : (fome > 25 ? 0xFFE0 : 0xF800);
    _tft->fillRect(37, 9, (fome * 86) / 100, 6, hColor);

    // Barra de Energia (NRG)
    _tft->setTextColor(0xF81F); // Magenta
    _tft->setCursor(5, 22);
    _tft->print("NRG");
    
    _tft->drawRect(35, 21, 90, 10, 0x3186);
    uint16_t eColor = (energia > 60) ? 0x07FF : (energia > 25 ? 0xFBE0 : 0xF800);
    _tft->fillRect(37, 23, (energia * 86) / 100, 6, eColor);
}

void Renderer::drawSprite(int x, int y, const uint16_t* sprite, int w, int h, uint16_t transparentColor, int scale) {
    if (!_tft) return;

    if (scale <= 1) {
        if (_canvas && w <= 32 && h <= 32) {
            Habitat::drawArea(x, y, w, h);
            _canvas->fillSprite(transparentColor); 
            _canvas->pushImage(0, 0, w, h, sprite);
            _canvas->pushSprite(x, y, transparentColor);
        } else {
            _tft->pushImage(x, y, w, h, sprite);
        }
    } else {
        // Desenha pixel a pixel: opacos = pet, transparentes = fundo do habitat
        // Isso evita que o sprite cubra o cenário com um retângulo vazio
        for (int sy = 0; sy < h; sy++) {
            for (int sx = 0; sx < w; sx++) {
                uint16_t color = pgm_read_word(&sprite[sy * w + sx]);
                if (color != transparentColor) {
                    _tft->fillRect(x + sx * scale, y + sy * scale, scale, scale, color);
                } else {
                    // Pixel transparente: redesenha o fundo do habitat nessa posição
                    Habitat::drawArea(x + sx * scale, y + sy * scale, scale, scale);
                }
            }
        }
    }
}

void Renderer::drawDirtOverlay(int x, int y, int w, int h, int scale) {
    if (!_tft) return;
    
    // Desenha de 3 a 8 quadradinhos simulando sujeira/bugs nas bordas
    int numGlitches = random(3, 8);
    for (int i = 0; i < numGlitches; i++) {
        int gx = x + random(-10, w * scale + 10);
        int gy = y + random(-10, h * scale + 10);
        int gw = random(2, 6);
        int gh = random(2, 6);
        
        uint16_t color = (random(0, 2) == 0) ? 0x07E0 : 0xF81F; // Verde limão ou Magenta
        _tft->fillRect(gx, gy, gw, gh, color);
    }
}

void Renderer::clearArea(int x, int y, int w, int h, uint16_t color) {
    if (!_tft) return;
    _tft->fillRect(x, y, w, h, color);
}
