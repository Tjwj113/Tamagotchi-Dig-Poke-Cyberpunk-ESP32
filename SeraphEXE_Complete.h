#ifndef SERAPH_EXE_SPRITES_H
#define SERAPH_EXE_SPRITES_H

#include <Arduino.h>

// SERAPH.EXE - 32x32 px, RGB565 - Anjo de Dados (Perfect stage)
// Evolução máxima do Aurelius (caminho luz)
// Inspiração: Seraphimon (Digimon) + Arceus (Pokémon)
// 5 frames: Idle, Attack, Heal, Ultimate, Fly

#define _  0x0000  // Transparente
#define W  0xFFFF  // Branco puro
#define Wb 0xEF7D  // Branco brilhante
#define Wg 0xC618  // Cinza claro
#define Au 0xFEA0  // Dourado
#define Ad 0xC560  // Dourado escuro
#define Ag 0xFFE0  // Dourado brilhante
#define Cy 0x07FF  // Cyan brilhante
#define Cb 0x5FFF  // Cyan claro
#define Cd 0x0618  // Cyan escuro
#define Bl 0x39E7  // Azul claro
#define O  0x2945  // Outline preto
#define Y  0xFFE0  // Amarelo (energia divina)

// Frame 1: Idle - Postura majestosa com 6 asas
const uint16_t SeraphEXE_Idle[1024] PROGMEM = {
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,Cy,_,_,_,_,Cy,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,Cy,_,_,_,_,Cy,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,O,Au,O,_,_,O,Au,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,O,Au,Ag,Au,O,O,Au,Ag,Au,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Au,Ag,W,Ag,Au,Au,Ag,W,Ag,Au,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Au,Ag,W,Wb,W,Ag,Ag,W,Wb,W,Ag,Au,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Au,Au,Ag,Au,Au,Au,Au,Au,Ag,Au,Au,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,Bl,Bl,W,W,W,W,Bl,Bl,W,W,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,Cy,Bl,W,W,W,W,Cy,Bl,W,W,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,W,W,W,W,W,W,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,W,W,O,O,O,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,_,
_,Cb,Cb,Cb,_,_,_,_,_,_,_,O,O,W,W,W,O,O,_,_,_,_,_,_,Cb,Cb,Cb,_,_,_,_,
Cb,Cy,Cy,Cb,_,_,_,_,_,_,O,Wb,Wb,Wb,Wb,Wb,Wb,O,_,_,_,_,_,Cb,Cy,Cy,Cb,_,_,_,
Cb,Cy,Wb,Cy,Cb,_,_,_,_,O,Wb,W,Wb,Wb,Wb,W,Wb,Wb,O,_,_,_,Cb,Cy,Wb,Cy,Cb,_,_,
Cb,Cy,Wb,Cy,Cb,_,_,_,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,_,_,Cb,Cy,Wb,Cy,Cb,_,_,
Cb,Cy,Cy,Cy,Cb,_,_,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,_,Cb,Cy,Cy,Cy,Cb,_,_,
_,Cb,Cy,Cb,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,Cb,Cy,Cb,_,_,_,
_,_,Cb,_,_,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,_,Cb,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Wg,Wb,Wb,Wb,Wg,Wg,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Au,Wg,Wb,Wg,Au,Wg,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wg,Au,Wg,Wb,Wg,Au,Wg,Wg,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,_,Wg,_,Wb,_,Wg,_,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Wg,_,_,Wg,_,Wb,_,Wg,_,_,Wg,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,_,_,_,_,W,_,_,_,_,W,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Au,_,_,_,_,_,_,_,Au,Au,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,O,_,_,_,_,_,_,_,O,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Cy,Cy,Cy,Cy,Cy,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,Cy,Cy,Wb,Wb,Wb,Cy,Cy,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Cy,Cy,Cy,Cy,Cy,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_
};

// Frame 2: Attack - Lança divina de luz
const uint16_t SeraphEXE_Attack[1024] PROGMEM = {
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,Y,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,Cy,_,_,_,_,Cy,_,_,_,_,_,_,_,Y,Ag,Y,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,Cy,_,_,_,_,Cy,_,_,_,_,_,_,Y,Ag,W,Ag,Y,_,
_,_,_,_,_,_,_,_,_,_,_,_,O,Au,O,_,_,O,Au,O,_,_,_,_,Y,Ag,W,Wb,W,Y,_,
_,_,_,_,_,_,_,_,_,_,_,O,Au,Ag,Au,O,O,Au,Ag,Au,O,_,Y,Ag,W,Wb,W,Ag,Y,_,
_,_,_,_,_,_,_,_,_,_,O,Au,Ag,W,Ag,Au,Au,Ag,W,Ag,Au,O,Y,W,Wb,W,Ag,Y,_,_,
_,_,_,_,_,_,_,_,_,O,Au,Ag,W,Wb,W,Ag,Ag,W,Wb,W,Ag,Au,O,W,Wb,Ag,Y,_,_,_,
_,_,_,_,_,_,_,_,_,O,Au,Au,Ag,Au,Au,Au,Au,Au,Ag,Au,Au,O,Wb,Ag,Y,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,Bl,Bl,W,W,W,W,Bl,Bl,W,W,O,Ag,Y,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,Cy,Bl,W,W,W,W,Cy,Bl,W,W,O,Y,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,W,W,W,W,W,W,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,W,W,O,O,O,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,_,
_,Cb,Cb,Cb,_,_,_,_,_,_,_,O,O,W,W,W,O,O,_,_,_,_,_,_,Cb,Cb,Cb,_,_,_,_,
Cb,Cy,Cy,Cb,_,_,_,_,_,_,O,Wb,Wb,Wb,Wb,Wb,Wb,O,_,_,_,_,_,Cb,Cy,Cy,Cb,_,_,_,
Cb,Cy,Wb,Cy,Cb,_,_,_,_,O,Wb,W,Wb,Wb,Wb,W,Wb,Wb,O,_,_,_,Cb,Cy,Wb,Cy,Cb,_,_,
Cb,Cy,Wb,Cy,Cb,_,_,_,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,_,_,Cb,Cy,Wb,Cy,Cb,_,_,
Cb,Cy,Cy,Cy,Cb,_,_,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,_,Cb,Cy,Cy,Cy,Cb,_,_,
_,Cb,Cy,Cb,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,Cb,Cy,Cb,_,_,_,
_,_,Cb,_,_,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,_,Cb,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Wg,Wb,Wb,Wb,Wg,Wg,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Au,Wg,Wb,Wg,Au,Wg,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wg,Au,Wg,Wb,Wg,Au,Wg,Wg,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,_,Wg,_,Wb,_,Wg,_,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Wg,_,_,Wg,_,Wb,_,Wg,_,_,Wg,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,_,_,_,_,W,_,_,_,_,W,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Au,_,_,_,_,_,_,_,Au,Au,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,O,_,_,_,_,_,_,_,O,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Cy,Cy,Cy,Cy,Cy,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,Cy,Cy,Wb,W,Wb,Cy,Cy,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Cy,Cy,Cy,Cy,Cy,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_
};

// Frame 3: Heal - Aura de cura
const uint16_t SeraphEXE_Heal[1024] PROGMEM = {
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,Cy,_,_,_,_,Cy,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,Cy,_,_,_,_,_,Cy,_,_,_,_,Cy,_,_,_,_,Cy,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,O,Au,O,_,_,O,Au,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,O,Au,Ag,Au,O,O,Au,Ag,Au,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Au,Ag,W,Ag,Au,Au,Ag,W,Ag,Au,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Au,Ag,W,Wb,W,Ag,Ag,W,Wb,W,Ag,Au,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Au,Au,Ag,Au,Au,Au,Au,Au,Ag,Au,Au,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,O,O,W,W,W,W,O,O,W,W,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,W,W,W,W,W,W,W,W,W,W,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,W,W,W,W,W,W,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,W,W,O,O,O,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,_,
_,Cy,Cb,Cb,Cy,_,_,_,_,_,_,O,O,W,W,W,O,O,_,_,_,_,_,Cy,Cb,Cb,Cy,_,_,_,_,
Cy,Cb,Cy,Cy,Cb,_,_,_,_,_,O,Wb,Wb,Wb,Wb,Wb,Wb,O,_,_,_,_,Cb,Cy,Cy,Cb,Cy,_,_,
Cy,Cb,Cy,Wb,Cy,Cb,_,_,_,O,Wb,W,Wb,Wb,Wb,W,Wb,Wb,O,_,_,Cb,Cy,Wb,Cy,Cb,Cy,_,_,
Cy,Cb,Cy,Wb,Cy,Cb,_,_,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,_,Cb,Cy,Wb,Cy,Cb,Cy,_,_,
Cy,Cb,Cy,Cy,Cy,Cb,_,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,Cb,Cy,Cy,Cy,Cb,Cy,_,_,
_,Cy,Cb,Cy,Cb,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,Cb,Cy,Cb,Cy,_,_,_,
_,_,Cy,Cb,_,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,Cb,Cy,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Wg,Wb,Wb,Wb,Wg,Wg,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Au,Wg,Wb,Wg,Au,Wg,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wg,Au,Wg,Wb,Wg,Au,Wg,Wg,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,_,Wg,_,Wb,_,Wg,_,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Wg,_,_,Wg,_,Wb,_,Wg,_,_,Wg,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,_,_,_,_,W,_,_,_,_,W,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Au,_,_,_,_,_,_,_,Au,Au,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,O,_,_,_,_,_,_,_,O,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,Cy,_,_,_,_,_,Cy,Cy,Cy,Cy,Cy,_,_,_,_,_,Cy,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,Cy,Cy,Wb,Wb,Wb,Cy,Cy,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Cy,Cy,Cy,Cy,Cy,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,Cy,_,Cy,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_
};

// Frame 4: Ultimate - Poder máximo (6 asas expandidas)
const uint16_t SeraphEXE_Ultimate[1024] PROGMEM = {
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,Y,_,_,_,_,Y,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,Cy,_,_,_,_,Cy,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,O,Au,O,_,_,O,Au,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,O,Au,Ag,Au,O,O,Au,Ag,Au,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Au,Ag,W,Ag,Au,Au,Ag,W,Ag,Au,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Au,Ag,W,Wb,W,Ag,Ag,W,Wb,W,Ag,Au,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Au,Au,Ag,Y,Au,Au,Au,Y,Ag,Au,Au,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,Bl,Bl,W,W,W,W,Bl,Bl,W,W,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,Cy,Bl,W,W,W,W,Cy,Bl,W,W,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,W,W,W,W,W,W,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,W,W,Y,Y,Y,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,_,
Cy,Cb,Cb,Cb,Cy,_,_,_,_,_,_,O,O,W,W,W,O,O,_,_,_,_,_,Cy,Cb,Cb,Cb,Cy,_,_,_,
Cy,Cb,Cy,Cy,Cb,Cy,_,_,_,_,O,Wb,Wb,Wb,Wb,Wb,Wb,O,_,_,_,Cy,Cb,Cy,Cy,Cb,Cy,_,_,_,
Cy,Cb,Cy,Wb,Cy,Cb,Cy,_,_,O,Wb,W,Wb,Wb,Wb,W,Wb,Wb,O,_,Cy,Cb,Cy,Wb,Cy,Cb,Cy,_,_,
Cy,Cb,Cy,Wb,Cy,Cb,Cy,_,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,Cy,Cb,Cy,Wb,Cy,Cb,Cy,_,_,
Cy,Cb,Cy,Cy,Cy,Cy,Cb,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,Cb,Cy,Cy,Cy,Cy,Cb,Cy,_,
_,Cy,Cb,Cy,Cy,Cb,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,Cb,Cy,Cy,Cb,Cy,_,_,
_,_,Cy,Cb,Cy,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,Cy,Cb,Cy,_,_,_,
_,_,_,Cy,_,_,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,_,_,Cy,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Wg,Wb,Wb,Wb,Wg,Wg,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Au,Wg,Wb,Wg,Au,Wg,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wg,Au,Wg,Wb,Wg,Au,Wg,Wg,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,_,Wg,_,Wb,_,Wg,_,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Wg,_,_,Wg,_,Wb,_,Wg,_,_,Wg,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,_,_,_,_,W,_,_,_,_,W,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Au,_,_,_,_,_,_,_,Au,Au,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,O,_,_,_,_,_,_,_,O,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,Cy,Cy,Cy,Cy,Cy,Cy,Cy,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,Cy,Cy,Wb,Wb,W,Wb,Wb,Cy,Cy,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,Cy,Cy,Cy,Cy,Cy,Cy,Cy,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,Y,_,Y,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_
};

// Frame 5: Fly - Voando (asas em movimento)
const uint16_t SeraphEXE_Fly[1024] PROGMEM = {
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,Cy,_,_,_,_,Cy,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,Cy,_,_,_,_,Cy,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,O,Au,O,_,_,O,Au,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,O,Au,Ag,Au,O,O,Au,Ag,Au,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Au,Ag,W,Ag,Au,Au,Ag,W,Ag,Au,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Au,Ag,W,Wb,W,Ag,Ag,W,Wb,W,Ag,Au,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Au,Au,Ag,Au,Au,Au,Au,Au,Ag,Au,Au,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,Bl,Bl,W,W,W,W,Bl,Bl,W,W,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,Cy,Bl,W,W,W,W,Cy,Bl,W,W,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,W,W,W,W,W,W,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,W,W,O,O,O,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,_,
Cb,Cy,Cy,Cb,_,_,_,_,_,_,_,O,O,W,W,W,O,O,_,_,_,_,_,_,_,Cb,Cy,Cy,Cb,_,_,
_,Cb,Cy,Cy,Cb,_,_,_,_,_,O,Wb,Wb,Wb,Wb,Wb,Wb,O,_,_,_,_,_,Cb,Cy,Cy,Cb,_,_,_,
_,_,Cb,Cy,Cy,Cb,_,_,_,O,Wb,W,Wb,Wb,Wb,W,Wb,Wb,O,_,_,_,Cb,Cy,Cy,Cb,_,_,_,_,
_,_,_,Cb,Cy,Cy,Cb,_,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,_,Cb,Cy,Cy,Cb,_,_,_,_,
_,_,_,_,Cb,Cy,Cy,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,Cy,Cy,Cb,_,_,_,_,_,
_,_,_,_,_,Cb,Cy,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,Cy,Cb,_,_,_,_,_,_,
_,_,_,_,_,_,Cb,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,Cb,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Wg,Wb,Wb,Wb,Wg,Wg,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Au,Wg,Wb,Wg,Au,Wg,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wg,Au,Wg,Wb,Wg,Au,Wg,Wg,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,_,Wg,_,Wb,_,Wg,_,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Wg,_,_,Wg,_,Wb,_,Wg,_,_,Wg,O,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,_,_,_,_,W,_,_,_,_,W,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Au,_,_,_,_,_,_,_,Au,Au,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,O,_,_,_,_,_,_,_,O,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Cy,Cy,Cy,Cy,Cy,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,Cy,Cy,Wb,Wb,Wb,Cy,Cy,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Cy,Cy,Cy,Cy,Cy,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_
};

#undef _
#undef W
#undef Wb
#undef Wg
#undef Au
#undef Ad
#undef Ag
#undef Cy
#undef Cb
#undef Cd
#undef Bl
#undef O
#undef Y

#endif