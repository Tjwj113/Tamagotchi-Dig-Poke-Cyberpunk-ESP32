#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <Arduino.h>

/**
 * GameEngine: Gerencia a lógica interna do pet.
 * Atributos, Eixos de Personalidade e Passagem de Tempo.
 */
class GameEngine {
public:
    enum Eixo {
        EMOCAO = 0,
        LOGICA,
        AGRESSIVIDADE,
        CORRUPCAO,
        HUMANIDADE,
        INSTINTO
    };

    enum class EmotionState {
        CONTENTE,
        SONOLENTO,
        COM_FOME,
        IRRITADO,
        TRISTE,
        DESCONFIANTE,
        EM_COLAPSO,
        DISSOCIADO,
        EUFORICO,
        NOSTALGICO
    };

    static void init();
    static void update();

    // Getters
    static int getFome() { return _fome; }
    static int getEnergia() { return _energia; }
    static int getEixo(Eixo eixo) { return _eixos[(int)eixo]; }
    static int getNeobits() { return _neobits; }
    static int getPetID() { return _petID; }
    static EmotionState getState() { return _currentState; }
    static int getSujeira() { return _sujeiraDigital; }
    static bool isDoente() { return _isDoente; }
    static int getMegaBytes() { return _invMegaBytes; }
    static const char* getPetName();
    static const char* getPetStage();
    static int getDailyNeobitsEarned() { return _neobitsDailyEarned; }
    static void checkSleepReset();

    // Ações (Placeholders para Fase 5 de UI)
    static void alimentar(int pts, int emoPts = 2);
    static void brincar(int pts);
    static void falharMinigame();
    static void descansar(int pts);
    static void limparCache();
    static void curar();
    static void darBronca();
    static void addMegaByte(int qtd);
    static bool consumeMegaByte();
    static void addNeobits(int qtd);

    static void saveState();
    static void loadState();

private:
    static int _desconfianca; // 0 a 100 (Trauma)
    static int _traumaCount;  // Quantas vezes o pet foi negligenciado severamente
    static int _fome;
    static int _energia;
    static int _eixos[6];
    static int _neobits;
    static int _neobitsDailyEarned;
    static int _petID;
    static EmotionState _currentState;
    static int _sujeiraDigital;
    static bool _isDoente;
    static int _invMegaBytes;

    static unsigned long _lastTick;
    static const unsigned long TICK_INTERVAL = 15000; // 15 segundos por "tick"

    static void updateNeeds();
    static void computeState();
    static void checkEvolution();
};

#endif
