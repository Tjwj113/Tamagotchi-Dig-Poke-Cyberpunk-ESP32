#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <Arduino.h>
#include <TFT_eSPI.h>

/**
 * UIManager: Gerencia o Menu de Ações e a interface de interação.
 */
class UIManager {
public:
    static void init(TFT_eSPI* tft);
    static void update();
    static void draw();

private:
    static TFT_eSPI* _tft;
    static int _menuIdx;
    static const int MENU_COUNT = 6;
    static const char* _menuItems[MENU_COUNT];

    static void drawActionBar();
    static void executeAction();
};

#endif
