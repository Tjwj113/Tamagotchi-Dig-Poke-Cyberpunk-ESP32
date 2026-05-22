#ifndef EVOLUTIONS_H
#define EVOLUTIONS_H

#include <Arduino.h>

// IDs de Evolução para referência no GameEngine
enum PetID {
    ID_NEOLARVA = 0,
    ID_LUMI = 1, ID_RAZR = 2, ID_GLITCH = 3,
    ID_AURELIUS = 4, ID_WARHOUND_X = 5, ID_MALWARE_X = 6,
    ID_SERAPH_EXE = 7, ID_DOMINION = 8,
    ID_RECLUSE = 9, ID_PHANTOM_GUARD = 10,
    ID_PHOENIX_DAT = 11, ID_PHOENIX_WAR = 12,
    ID_NULLENTITY = 13, ID_NULLENTITY_RESTORED = 14
};

// Estrutura de animação para os pets
struct PetAnimation {
    const uint16_t* frames[4];
};

// LUMI (Branco/Cyan)
const uint16_t Lumi_Idle1[1024] PROGMEM = { [0 ... 1023] = 0xFFFF }; // Quadrado Branco
const uint16_t Lumi_Idle2[1024] PROGMEM = { [0 ... 1023] = 0xAD75 }; // Quadrado Cinza Azulado
const uint16_t Lumi_Happy[1024] PROGMEM = { [0 ... 1023] = 0x07FF }; // Quadrado Cyan
const uint16_t Lumi_Action[1024] PROGMEM = { [0 ... 1023] = 0xFFFF };


// GLITCH (Roxo/Verde)
const uint16_t Glitch_Idle1[1024] PROGMEM = { 0x780F };
const uint16_t Glitch_Idle2[1024] PROGMEM = { 0x07E0 };
const uint16_t Glitch_Happy[1024] PROGMEM = { 0x780F };
const uint16_t Glitch_Action[1024] PROGMEM = { 0x07E0 };

// NULLENTITY (Preto)
const uint16_t NullEntity_Idle1[1024] PROGMEM = { 0x0000 };

#endif