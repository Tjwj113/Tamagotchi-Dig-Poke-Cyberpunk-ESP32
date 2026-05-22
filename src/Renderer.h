#ifndef RENDERER_H
#define RENDERER_H

#include <Arduino.h>
#include <TFT_eSPI.h>

class Renderer {
public:
    static void init(TFT_eSPI* tft);
    
    /**
     * Desenha um sprite da PROGMEM na tela com suporte a transparência.
     * @param x Posição X
     * @param y Posição Y
     * @param sprite Ponteiro para o array na Flash
     * @param w Largura
     * @param h Altura
     * @param transparentColor Cor a ser tratada como transparente (default: 0x0000 / Preto)
     */
    static void drawSprite(int x, int y, const uint16_t* sprite, int w, int h, uint16_t transparentColor = 0x0000, int scale = 1);
    
    static void drawDirtOverlay(int x, int y, int w, int h, int scale = 1);

    static void drawStatusBar(int fome, int energia);
    static void clearArea(int x, int y, int w, int h, uint16_t color = 0x0000);

private:
    static TFT_eSPI* _tft;
    static TFT_eSprite* _canvas; // Buffer pequeno para transparência
};

#endif
