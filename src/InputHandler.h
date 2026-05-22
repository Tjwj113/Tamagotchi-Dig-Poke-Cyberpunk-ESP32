#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <Arduino.h>

/**
 * InputHandler: Gerencia os botões físicos do TTGO T-Display.
 * Botão 1: GPIO 0
 * Botão 2: GPIO 35
 */
class InputHandler {
public:
    static void init();
    static void update();

    // Retorna true apenas no momento do clique (frente de descida)
    static bool getBtn1Click();
    static bool getBtn2Click();
    static bool getBtn2LongPress();

private:
    static const int BTN1_PIN = 0;
    static const int BTN2_PIN = 35;

    static bool _btn1LastState;
    static bool _btn2LastState;
    static bool _btn1Clicked;
    static bool _btn2Clicked;
    static bool _btn2LongPressed;
    static unsigned long _btn2PressTime;
};

#endif
