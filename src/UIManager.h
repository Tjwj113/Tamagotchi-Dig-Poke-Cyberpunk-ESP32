#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <Arduino.h>
#include <TFT_eSPI.h>

/**
 * UIManager: Gerencia o Menu de Ações e a interface de interação.
 * Agora suporta Menus Hierárquicos e cliques longos para Voltar.
 */
class UIManager {
public:
    enum class UIState {
        IDLE,
        MAIN_MENU,
        SUB_MENU,
        EXECUTING,
        STATUS_SCREEN
    };

    static void init(TFT_eSPI* tft);
    static void update();

private:
    static TFT_eSPI* _tft;
    static UIState _currentState;
    
    static int _mainMenuIdx;
    static const int MAIN_MENU_COUNT = 4;
    static const char* _mainMenuItems[MAIN_MENU_COUNT];

    static int _subMenuIdx;
    static int _subMenuCount;
    static const char* _subMenuItems[4]; // Max 4 subitems per category

    static unsigned long _lastInteractionTime;
    static const unsigned long MENU_TIMEOUT = 8000; // 8s

    static void drawActionBar();
    static void enterSubMenu();
    static void exitSubMenu();
    static void executeAction();
    static void showStatusScreen();
    static void exitStatusScreen();

    // Minigames
    static void playCyberReflex();
    static void playPatternHacker();
};

#endif
