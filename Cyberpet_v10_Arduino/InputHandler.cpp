#include "InputHandler.h"

bool InputHandler::_btn1LastState = HIGH;
bool InputHandler::_btn2LastState = HIGH;
bool InputHandler::_btn1Clicked = false;
bool InputHandler::_btn2Clicked = false;

void InputHandler::init() {
    pinMode(BTN1_PIN, INPUT_PULLUP);
    pinMode(BTN2_PIN, INPUT); // GPIO 35 no TTGO não tem pull-up interno, mas a placa costuma ter externo.
}

void InputHandler::update() {
    // Reset flags
    _btn1Clicked = false;
    _btn2Clicked = false;

    static unsigned long lastTrigger1 = 0;
    static unsigned long lastTrigger2 = 0;

    bool b1 = digitalRead(BTN1_PIN);
    bool b2 = digitalRead(BTN2_PIN);

    // Detecção de clique no falling edge (soltar o botão) com cooldown de 250ms
    if (b1 == LOW && _btn1LastState == HIGH && (millis() - lastTrigger1 > 250)) {
        _btn1Clicked = true;
        lastTrigger1 = millis();
    }
    if (b2 == LOW && _btn2LastState == HIGH && (millis() - lastTrigger2 > 250)) {
        _btn2Clicked = true;
        lastTrigger2 = millis();
    }

    _btn1LastState = b1;
    _btn2LastState = b2;
}

bool InputHandler::getBtn1Click() {
    return _btn1Clicked;
}

bool InputHandler::getBtn2Click() {
    return _btn2Clicked;
}
