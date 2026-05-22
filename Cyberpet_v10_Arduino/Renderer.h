#ifndef RENDERER_H
#define RENDERER_H

#include <Arduino.h>
#include <TFT_eSPI.h>

class Renderer {
public:
    static void init(TFT_eSPI* tft);
    static void drawSprite(int x, int y, const uint16_t* sprite, int w, int h, uint16_t transparentColor = 0x0000, int scale = 1, int yOffset = 0);
    static void drawStatusBar(int fome, int energia);
    static void clearArea(int x, int y, int w, int h, uint16_t color = 0x0000);

private:
    static TFT_eSPI* _tft;
    static TFT_eSprite* _canvas; 
};

#endif
