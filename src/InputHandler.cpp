#include "InputHandler.h"

bool InputHandler::_btn1LastState = HIGH;
bool InputHandler::_btn2LastState = HIGH;
bool InputHandler::_btn1Clicked = false;
bool InputHandler::_btn2Clicked = false;
bool InputHandler::_btn2LongPressed = false;
unsigned long InputHandler::_btn2PressTime = 0;

void InputHandler::init() {
    pinMode(BTN1_PIN, INPUT_PULLUP);
    pinMode(BTN2_PIN, INPUT); // GPIO 35 no TTGO não tem pull-up interno, mas a placa costuma ter externo.
}

void InputHandler::update() {
    bool b1 = digitalRead(BTN1_PIN);
    bool b2 = digitalRead(BTN2_PIN);

    // Resetando estados no início do frame
    _btn1Clicked = false;
    _btn2Clicked = false;
    _btn2LongPressed = false;

    // Detecção botão 1 (clique simples no falling edge)
    if (_btn1LastState == HIGH && b1 == LOW) {
        _btn1Clicked = true;
    }

    // Detecção botão 2 (medição de tempo)
    if (_btn2LastState == HIGH && b2 == LOW) {
        // Pressionou agora
        _btn2PressTime = millis();
    } else if (_btn2LastState == LOW && b2 == HIGH) {
        // Soltou agora
        unsigned long duration = millis() - _btn2PressTime;
        if (duration >= 500) {
            _btn2LongPressed = true;
        } else if (duration > 20) { // debounce de 20ms
            _btn2Clicked = true;
        }
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

bool InputHandler::getBtn2LongPress() {
    return _btn2LongPressed;
}
