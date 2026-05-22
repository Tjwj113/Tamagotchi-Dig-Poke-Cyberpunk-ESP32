#ifndef HABITAT_H
#define HABITAT_H

#include <Arduino.h>
#include <TFT_eSPI.h>

/**
 * Habitat: Gerencia o cenário e influências passivas no pet.
 */
class Habitat {
public:
    enum Environment {
        QUARTO_CYBERPUNK = 0,
        LABORATORIO,
        JARDIM_NEON,
        FABRICA_INDUSTRIAL
    };

    static void init(TFT_eSPI* tft);
    static void setEnvironment(Environment env);
    static void draw();
    static void drawArea(int x, int y, int w, int h);
    static Environment getCurrentEnv() { return _currentEnv; }
    static const char* getEnvName();
    static uint16_t getGlowColor();

private:
    static TFT_eSPI* _tft;
    static Environment _currentEnv;

    static void drawBackground();
};

#endif
