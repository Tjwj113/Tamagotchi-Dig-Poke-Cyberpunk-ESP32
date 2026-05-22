#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "Renderer.h"
#include "sprites/NeoLarva.h"
#include "sprites/Evolutions.h"
#include "sprites/MalwareX.h"
#include "GameEngine.h"
#include "Habitat.h"
#include "InputHandler.h"
#include "UIManager.h"

// Instância do display
TFT_eSPI tft = TFT_eSPI();

void setup() {
    // Inicializa serial para debug
    Serial.begin(115200);
    delay(1000);
    
    // Inicializa display
    tft.init();
    tft.setRotation(0); 
    tft.fillScreen(TFT_BLACK);
    
    // Liga o backlight
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

    // Inicializa os Motores
    Renderer::init(&tft);
    GameEngine::init();
    Habitat::init(&tft);
    Habitat::setEnvironment(Habitat::QUARTO_CYBERPUNK);
    InputHandler::init();
    UIManager::init(&tft);

    Serial.println(F("--- NEOPET CYBERPUNK ---"));
    Serial.println(F("Fase 5: Interface Candy Cyber V-Pet"));
    
    // Borda da tela
    tft.drawRect(0, 0, tft.width(), tft.height(), 0x39C7);
}

// --- Variáveis de Animação ---
int  currentFrame   = 0;
int  sleepCycle     = 0;    // Conta ciclos de respiração para o estouro da bolha
bool eatingLocked   = false; // Trava de alimentação
unsigned long eatingStart = 0;

unsigned long lastAnim = 0;

// ZONA 2: Limites fixos de renderização do pet
const int ZONE2_Y     = 40;
const int ZONE2_H     = 140;  // y: 40 a 180

void loop() {
    // 1. INPUT
    InputHandler::update();
    
    // 2. UPDATE
    GameEngine::update();
    UIManager::update();
    
    // 3. RENDER
    GameEngine::EmotionState state = GameEngine::getState();
    
    int animSpeed    = 500;
    uint16_t borderColor = 0x39C7; 
    
    if (state == GameEngine::EmotionState::SONOLENTO) animSpeed = 900;
    if (state == GameEngine::EmotionState::EUFORICO)  animSpeed = 220;
    if (state == GameEngine::EmotionState::EM_COLAPSO) {
        animSpeed   = 120;
        borderColor = TFT_RED;
    }
    if (state == GameEngine::EmotionState::COM_FOME) borderColor = TFT_YELLOW;

    unsigned long now = millis();

    // Trava de alimentação: bloqueia animações normais por 3 segundos
    if (eatingLocked && now - eatingStart >= 3000) {
        eatingLocked = false;
    }

    if (now - lastAnim >= (unsigned long)animSpeed) {
        lastAnim = now;
        
        // 1. Atualiza Status Bar (Zona 1: y 0 a 35)
        Renderer::drawStatusBar(GameEngine::getFome(), GameEngine::getEnergia());
        
        // 2. Limpeza restrita à ZONA 2 (y: 40 a 180) - anti-flickering
        tft.fillRect(0, ZONE2_Y, tft.width(), ZONE2_H, 0x0000);

        // 3. Desenha fundo do habitat na Zona 2
        Habitat::drawArea(0, ZONE2_Y, tft.width(), ZONE2_H);
        
        // 4. Lógica de seleção de sprite
        const uint16_t* spritePtr;
        int petID = GameEngine::getPetID();

        if (petID == 0) { // NeoLarva
            if (eatingLocked) {
                // Trava: animação de mastigação
                spritePtr = NeoLarva_Eating;
            }
            else if (state == GameEngine::EmotionState::SONOLENTO) {
                // Ciclo orgânico de sono: Sleep1 -> Sleep2 -> Sleep3 -> Sleep4
                // A cada 5 ciclos de respiração, estoura a bolha (Sleep3)
                int sleepFrame = sleepCycle % 12;
                if      (sleepFrame < 4)  spritePtr = NeoLarva_Sleep1;  // Bolha pequena
                else if (sleepFrame < 8)  spritePtr = NeoLarva_Sleep2;  // Bolha grande
                else if (sleepFrame == 8) spritePtr = NeoLarva_Sleep3;  // Bolha estoura!
                else                      spritePtr = NeoLarva_Sleep4;  // Fecha olhinhos
                sleepCycle++;
            }
            else if (state == GameEngine::EmotionState::TRISTE ||
                     state == GameEngine::EmotionState::COM_FOME ||
                     state == GameEngine::EmotionState::EM_COLAPSO) {
                // Pisca triste/urgente a cada 2 frames
                spritePtr = (currentFrame % 2 == 0) ? NeoLarva_Happy : NeoLarva_Idle1;
                // Usa Happy com olhos fechados como 'sad eye' e Idle como neutro
                // (Substituir por NeoLarva_Sad se criado futuramente)
                spritePtr = NeoLarva_Idle1;
            }
            else if (state == GameEngine::EmotionState::EUFORICO) {
                spritePtr = NeoLarva_Happy;
            }
            else {
                // Normal: Idle com piscada ocasional (1 em cada 8 frames)
                spritePtr = (currentFrame % 8 == 0) ? NeoLarva_Blink : NeoLarva_Idle1;
            }
        } else {
            // Placeholders para Evoluções (Evolutions.h)
            switch(petID) {
                case 1: spritePtr = Lumi_Idle1; break;
                case 2: spritePtr = RazR_Idle1; break;
                case 3: spritePtr = Glitch_Idle1; break;
                case 6: // MalwareX
                    if (state == GameEngine::EmotionState::SONOLENTO) spritePtr = MalwareX_Sleep;
                    else if (state == GameEngine::EmotionState::TRISTE) spritePtr = MalwareX_Sad;
                    else if (currentFrame % 8 == 0) spritePtr = MalwareX_Idle2;
                    else if (state == GameEngine::EmotionState::EUFORICO) spritePtr = MalwareX_Happy;
                    else if (state == GameEngine::EmotionState::EM_COLAPSO) spritePtr = MalwareX_Attack;
                    else spritePtr = MalwareX_Idle1;
                    break;
                case 13: spritePtr = NullEntity_Idle1; break;
                default: spritePtr = NeoLarva_Idle1; break;
            }
        }

        // 5. Renderiza Pet (Escala 2x), centrado na Zona 2
        int petX = tft.width() / 2 - 32;
        int petY = ZONE2_Y + (ZONE2_H / 2) - 32;
        Renderer::drawSprite(petX, petY, spritePtr, 32, 32, 0x0000, 2);
        
        // 6. Overlay de Sujeira Digital (acima do sprite, ainda na Zona 2)
        if (GameEngine::getSujeira() >= 70) {
            Renderer::drawDirtOverlay(petX, petY, 32, 32, 2);
        }
        
        // 7. Borda pulsatória
        tft.drawRect(0, 0, tft.width(), tft.height(), borderColor);
        
        currentFrame++;
    }
}
