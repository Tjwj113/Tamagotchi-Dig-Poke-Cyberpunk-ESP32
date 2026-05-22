#ifndef AURELIUS_SPRITES_H
#define AURELIUS_SPRITES_H

#include <Arduino.h>

// AURELIUS - 32x32 px, RGB565 - Cavaleiro Angelical (Adult stage)
// Evolução do Lumi por carinho alto
// Inspiração: MagnaAngemon (Digimon) + Gallade (Pokémon)
// 4 frames: Idle, Attack, Defend, Special

#define _  0x0000  // Transparente
#define W  0xFFFF  // Branco puro
#define Wb 0xDEFB  // Branco azulado
#define Wg 0xC618  // Cinza claro
#define Au 0xFEA0  // Dourado
#define Ad 0xC560  // Dourado escuro
#define Ag 0xFFE0  // Dourado brilhante
#define Ab 0x001F  // Azul escuro
#define Ac 0x07FF  // Cyan brilhante
#define Al 0x5D7F  // Azul claro
#define Am 0x3D5F  // Azul médio
#define O  0x2945  // Outline preto
#define R  0xF800  // Vermelho (espada)

// Frame 1: Idle - Postura nobre
const uint16_t Aurelius_Idle[1024] PROGMEM = {
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,O,O,O,O,O,O,O,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Au,Au,Au,Au,Au,Au,Au,O,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Au,Ag,Ag,Au,Au,Ag,Ag,Au,Au,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Ag,W,W,Au,Au,W,W,Ag,Au,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Au,Au,Au,Au,Au,Au,Au,Au,Au,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,W,Ab,Ab,W,W,Ab,Ab,W,W,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,W,Ac,Ab,W,W,Ac,Ab,W,W,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,W,W,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,O,O,O,W,W,W,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,O,W,W,W,O,O,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,Al,Al,Al,_,_,_,_,_,O,Wb,Wb,Wb,Wb,O,_,_,_,_,_,Al,Al,Al,_,_,_,_,_,
_,Al,Am,Am,Al,_,_,_,_,O,Wb,Wb,Wb,Wb,Wb,Wb,O,_,_,_,Al,Am,Am,Al,_,_,_,_,
Al,Am,Ab,Ab,Am,Al,_,_,O,Wb,W,Wb,Wb,Wb,W,Wb,Wb,O,_,Al,Am,Ab,Ab,Am,Al,_,_,
Al,Am,Ab,Ac,Ab,Al,_,_,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,_,Al,Ab,Ac,Ab,Am,Al,_,_,
Al,Am,Ac,Ab,Am,Al,_,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,Al,Am,Ab,Ac,Am,Al,_,_,
_,Al,Am,Am,Al,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,Al,Am,Am,Al,_,_,_,
_,_,Al,Al,_,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,Al,Al,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Wg,Wg,Wg,Wg,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Au,Wg,Au,Wg,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wg,Au,Wg,Au,Wg,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,_,Wg,_,Wg,_,Wg,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Wg,_,_,Wg,_,Wg,_,_,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,_,_,_,_,_,_,_,W,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Au,_,_,_,_,_,Au,Au,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,O,_,_,_,_,_,O,O,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Ac,Ac,Ac,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,Ac,Ac,Ac,Ac,Ac,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Ac,Ac,Ac,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_
};

// Frame 2: Attack - Espada de luz!
const uint16_t Aurelius_Attack[1024] PROGMEM = {
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,W,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,O,O,O,O,O,O,O,_,_,_,_,_,_,_,_,W,Ac,Ac,W,_,_,
_,_,_,_,_,_,_,_,_,_,O,Au,Au,Au,Au,Au,Au,Au,O,_,_,_,_,W,Ac,Ac,Ac,Ac,W,_,_,
_,_,_,_,_,_,_,_,_,O,Au,Ag,Ag,Au,Au,Ag,Ag,Au,Au,O,_,W,Ac,Ac,W,Ac,Ac,W,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Ag,W,W,Au,Au,W,W,Ag,Au,O,W,Ac,Ac,W,_,W,W,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Au,Au,Au,Au,Au,Au,Au,Au,Au,O,Ac,Ac,W,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,W,Ab,Ab,W,W,Ab,Ab,W,W,O,Ac,W,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,W,Ac,Ab,W,W,Ac,Ab,W,W,O,W,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,W,W,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,O,O,O,W,W,W,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,O,W,W,W,O,O,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,Al,Al,Al,_,_,_,_,_,O,Wb,Wb,Wb,Wb,O,_,_,_,_,_,Al,Al,Al,_,_,_,_,_,
_,Al,Am,Am,Al,_,_,_,_,O,Wb,Wb,Wb,Wb,Wb,Wb,O,_,_,_,Al,Am,Am,Al,_,_,_,_,
Al,Am,Ab,Ab,Ac,Ac,Ac,Ac,O,Wb,W,Wb,Wb,Wb,W,Wb,Wb,O,_,Al,Am,Ab,Ab,Am,Al,_,_,
Al,Am,Ab,Ac,Ac,Ac,Ac,Ac,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,_,Al,Ab,Ac,Ab,Am,Al,_,_,
Al,Am,Ac,Ac,Ac,Ac,_,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,Al,Am,Ab,Ac,Am,Al,_,_,
_,Al,Am,Am,Al,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,Al,Am,Am,Al,_,_,_,
_,_,Al,Al,_,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,Al,Al,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Wg,Wg,Wg,Wg,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Au,Wg,Au,Wg,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wg,Au,Wg,Au,Wg,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,_,Wg,_,Wg,_,Wg,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Wg,_,_,Wg,_,Wg,_,_,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,_,_,_,_,_,_,_,W,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Au,_,_,_,_,_,Au,Au,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,O,_,_,_,_,_,O,O,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Ac,Ac,Ac,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,Ac,Ac,W,Ac,Ac,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Ac,Ac,Ac,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_
};

// Frame 3: Defend - Escudo de luz
const uint16_t Aurelius_Defend[1024] PROGMEM = {
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,O,O,O,O,O,O,O,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Au,Au,Au,Au,Au,Au,Au,O,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Au,Ag,Ag,Au,Au,Ag,Ag,Au,Au,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Ag,W,W,Au,Au,W,W,Ag,Au,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Au,Au,Au,Au,Au,Au,Au,Au,Au,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,W,O,O,W,W,O,O,W,W,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,W,W,W,W,W,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,W,W,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,O,O,O,W,W,W,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,O,W,W,W,O,O,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,Ac,Ac,Ac,Ac,_,_,_,O,Wb,Wb,Wb,Wb,O,_,_,Ac,Ac,Ac,Ac,_,_,_,_,_,_,_,
_,_,Ac,Ac,Ac,Ac,Ac,Ac,_,O,Wb,Wb,Wb,Wb,Wb,Wb,O,Ac,Ac,Ac,Ac,Ac,Ac,_,_,_,_,_,
_,Ac,Ac,Ac,W,Ac,Ac,Ac,Ac,Wb,W,Wb,Wb,Wb,W,Wb,Wb,Ac,Ac,Ac,W,Ac,Ac,Ac,_,_,_,
Ac,Ac,Ac,W,W,W,Ac,Ac,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,Ac,Ac,W,W,W,Ac,Ac,Ac,_,
Ac,Ac,W,W,W,Ac,Ac,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,Ac,Ac,W,W,W,Ac,Ac,_,
_,Ac,Ac,W,Ac,Ac,Ac,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,Ac,Ac,Ac,W,Ac,Ac,_,_,
_,_,Ac,Ac,Ac,Ac,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,Ac,Ac,Ac,Ac,_,_,_,
_,_,_,Ac,Ac,_,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,_,Ac,Ac,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Wg,Wg,Wg,Wg,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Au,Wg,Au,Wg,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wg,Au,Wg,Au,Wg,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,_,Wg,_,Wg,_,Wg,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Wg,_,_,Wg,_,Wg,_,_,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,_,_,_,_,_,_,_,W,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Au,_,_,_,_,_,Au,Au,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,O,_,_,_,_,_,O,O,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Ac,Ac,Ac,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,Ac,Ac,W,Ac,Ac,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Ac,Ac,Ac,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_
};

// Frame 4: Special - Poder divino (aura completa)
const uint16_t Aurelius_Special[1024] PROGMEM = {
_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Ac,_,_,_,_,Ac,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,O,O,O,O,O,O,O,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Au,Au,Au,Au,Au,Au,Au,O,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Au,Ag,Ag,Au,Au,Ag,Ag,Au,Au,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Ag,W,W,Au,Au,W,W,Ag,Au,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Au,Au,Au,Au,Au,Au,Au,Au,Au,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,W,Ab,Ab,W,W,Ab,Ab,W,W,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,W,Ac,Ab,W,W,Ac,Ab,W,W,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,W,W,W,W,W,W,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,W,W,Ac,Ac,Ac,W,W,W,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,O,W,W,W,O,O,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,Ac,Ac,Ac,Ac,Ac,_,_,_,_,O,Wb,Wb,Wb,Wb,O,_,_,_,Ac,Ac,Ac,Ac,Ac,_,_,_,_,_,
Ac,Ac,Ac,Ac,Ac,Ac,Ac,_,_,O,Wb,Wb,Wb,Wb,Wb,Wb,O,_,Ac,Ac,Ac,Ac,Ac,Ac,Ac,_,_,_,
Ac,Ac,W,Ac,Ac,Ac,Ac,Ac,O,Wb,W,Wb,Wb,Wb,W,Wb,Wb,O,Ac,Ac,Ac,Ac,W,Ac,Ac,_,_,
Ac,Ac,Ac,W,W,Ac,Ac,Ac,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,Ac,Ac,Ac,W,W,Ac,Ac,Ac,_,
Ac,Ac,Ac,W,Ac,Ac,Ac,O,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,O,Ac,Ac,Ac,W,Ac,Ac,Ac,_,
Ac,Ac,Ac,Ac,Ac,Ac,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,Ac,Ac,Ac,Ac,Ac,Ac,_,
_,Ac,Ac,Ac,Ac,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,Ac,Ac,Ac,Ac,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wb,Wb,Wb,Wb,Wb,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Wg,Wg,Wg,Wg,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,O,Wg,Au,Wg,Au,Wg,O,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,Wg,Au,Wg,Au,Wg,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,Wg,_,Wg,_,Wg,_,Wg,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Wg,_,_,Wg,_,Wg,_,_,Wg,O,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,W,_,_,_,_,_,_,_,W,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,O,Au,Au,_,_,_,_,_,Au,Au,O,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,O,O,_,_,_,_,_,O,O,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,Ac,Ac,Ac,Ac,Ac,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,Ac,Ac,W,W,W,Ac,Ac,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,Ac,Ac,Ac,Ac,Ac,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
_,_,_,_,_,_,_,_,_,_,_,_,Ac,_,Ac,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_
};

#undef _
#undef W
#undef Wb
#undef Wg
#undef Au
#undef Ad
#undef Ag
#undef Ab
#undef Ac
#undef Al
#undef Am
#undef O
#undef R

#endif