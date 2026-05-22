#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <Arduino.h>

/**
 * GameEngine: Gerencia a lógica interna do pet.
 * Atributos, Eixos de Personalidade e Passagem de Tempo.
 */
class GameEngine {
public:
    enum class ItemType {
        RACAO_PADRAO = 0,
        MEGA_BYTE = 1,
        BRINQUEDO_DIGITAL = 2
    };

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
        NOSTALGICO,
        DORMINDO
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
    static bool isSleeping() { return _isSleeping; }
    static bool hasEvolved() { return _hasEvolved; }
    static void clearEvolvedFlag() { _hasEvolved = false; }

    static const char* getPetName();
    static const char* getPetStage();

    // Ações (Placeholders para Fase 5 de UI)
    static void alimentar(int pts, int emoPts = 2);
    static void brincar(int pts);
    static void descansar(int pts);
    static void setSleeping(bool sleeping);

    // Economia e Itens
    static void addNeobits(int amount);
    static bool buyItem(ItemType type);
    static bool useItem(ItemType type);
    static int getItemCount(ItemType type);

    static void saveState();
    static void loadState();

private:
    static int _desconfianca; // 0 a 100 (Trauma)
    static int _traumaCount;  // Quantas vezes o pet foi negligenciado
    static int _fome;
    static int _energia;
    static int _eixos[6];
    static int _neobits;
    static int _inventory[3]; // Quantidade de cada ItemType
    static int _petID;
    static bool _isSleeping;
    static bool _hasEvolved;
    static EmotionState _currentState;

    static unsigned long _lastTick;
    static unsigned long _sleepStartTime;
    static const unsigned long TICK_INTERVAL = 15000; // 15 segundos por "tick"

    static void updateNeeds();
    static void computeState();
    static void checkEvolution();
};

#endif
