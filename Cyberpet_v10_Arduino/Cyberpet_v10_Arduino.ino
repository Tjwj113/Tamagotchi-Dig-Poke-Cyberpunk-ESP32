#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "Renderer.h"
#include "sprites/NeoLarva.h"
#include "sprites/Aurelius.h"
#include "sprites/RazR.h"
#include "sprites/WarhoundX.h"
#include "sprites/SeraphEXE.h" // Inclui as novas sprites do SeraphEXE
#include "sprites/Evolutions.h"
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
    Serial.println(F("Arduino V10: Sistema Neon + 4 Frames"));
    
    // Borda da tela
    tft.drawRect(0, 0, tft.width(), tft.height(), 0x39C7);
}

int currentFrame = 0;
int sleepCycle = 0; // Controle para a animação de bolha da NeoLarva
unsigned long lastAnim = 0;

void loop() {
    // 1. INPUT
    InputHandler::update();
    
    // 2. UPDATE
    bool wasSleeping = (GameEngine::getState() == GameEngine::EmotionState::DORMINDO);
    
    GameEngine::update();
    UIManager::update();
    
    // 3. RENDER
    GameEngine::EmotionState state = GameEngine::getState();

    int animSpeed = 500;
    uint16_t borderColor = 0x39C7; 
    
    if (state == GameEngine::EmotionState::SONOLENTO) animSpeed = 1000;
    if (state == GameEngine::EmotionState::EUFORICO) animSpeed = 200;
    if (state == GameEngine::EmotionState::EM_COLAPSO) {
        animSpeed = 100;
        borderColor = TFT_RED;
    }
    if (state == GameEngine::EmotionState::COM_FOME) borderColor = TFT_YELLOW;
    if (state == GameEngine::EmotionState::DORMINDO) {
        animSpeed = 2000; // Respiração lenta
        borderColor = 0x3186; // Escuro
    }

    unsigned long now = millis();
    static int lastFome = -1;
    static int lastEnergia = -1;
    static GameEngine::EmotionState lastState = (GameEngine::EmotionState)-1;
    static int lastFrameIdx = -1;
    static Habitat::Environment lastEnv = (Habitat::Environment)-1;

    if (now - lastAnim >= animSpeed) {
        lastAnim = now;
        
        Habitat::Environment curEnv = Habitat::getCurrentEnv();
        bool envChanged = (curEnv != lastEnv);

        // 1. Status Bar (Apenas se mudar)
        int curFome = GameEngine::getFome();
        int curEnergia = GameEngine::getEnergia();
        if (curFome != lastFome || curEnergia != lastEnergia) {
            Renderer::drawStatusBar(curFome, curEnergia);
            lastFome = curFome;
            lastEnergia = curEnergia;
        }
        
        // 2. Seleção de Frame (Animação real e piscar)
        // Lógica Padrão: Alterna entre Olho Aberto (Pulo Alto) e Relaxado (Pulo Baixo)
        int frameIdx = (currentFrame % 2 == 0) ? 0 : 1; 

        // A cada 4 segundos, dá um piscar longo super feliz no alto
        if (currentFrame % 8 == 0) frameIdx = 2; 

        // Aplica as emoções por cima...
        if (state == GameEngine::EmotionState::EUFORICO) {
            frameIdx = (currentFrame % 2 == 0) ? 2 : 0; // Alterna com cara de blush (Frame 3)
        }
        else if (state == GameEngine::EmotionState::COM_FOME || state == GameEngine::EmotionState::EM_COLAPSO) {
            frameIdx = (currentFrame % 2 == 0) ? 3 : 1; // Alterna com boca aberta e relaxado
        }
        else if (state == GameEngine::EmotionState::DORMINDO) {
            frameIdx = 2; // Fica de olhos fechados (Frame 3)
            lastFrameIdx = -1; // Força redesenhar para o efeito Zzz
        }

        // 0. VFX de Evolução
        if (GameEngine::hasEvolved()) {
            tft.fillRect(0, 40, 135, 160, TFT_BLACK);
            tft.setTextColor(TFT_WHITE);
            tft.setTextDatum(MC_DATUM);
            tft.drawString("EVOLUINDO...", 135/2, 120, 2);
            
            // Efeito de glitch
            for (int i = 0; i < 30; i++) {
                tft.fillRect(random(135), random(40, 160), random(10, 50), random(2, 10), (i % 2 == 0) ? TFT_CYAN : TFT_MAGENTA);
                delay(50);
            }
            
            GameEngine::clearEvolvedFlag();
            lastFrameIdx = -1; // força redesenho
            lastState = (GameEngine::EmotionState)-1; // força texto
        }

        int bounceOffset = (state == GameEngine::EmotionState::DORMINDO) ? 0 : ((currentFrame % 2 == 0) ? 0 : 2);
        static int lastBounceOffset = -1;

        // Redesenha sempre que o frame mudar, o ambiente mudar, o estado mudar ou o pulinho (bounce) mudar
        if (frameIdx != lastFrameIdx || envChanged || state != lastState || bounceOffset != lastBounceOffset) {
            const uint16_t* spritePtr;
            int petID = GameEngine::getPetID();

            if (petID == 0) { // NeoLarva
                if (state == GameEngine::EmotionState::DORMINDO || state == GameEngine::EmotionState::SONOLENTO) {
                    // Ciclo de 8 frames para o sono da NeoLarva (Sleep1 a Sleep4)
                    int sleepFrame = sleepCycle % 12;
                    if      (sleepFrame < 4)  spritePtr = NeoLarva_Sleep1;
                    else if (sleepFrame < 8)  spritePtr = NeoLarva_Sleep2;
                    else if (sleepFrame == 8) spritePtr = NeoLarva_Sleep3; // Estouro da bolha
                    else                      spritePtr = NeoLarva_Sleep4;
                    sleepCycle++;
                } 
                else if (state == GameEngine::EmotionState::COM_FOME) {
                    spritePtr = NeoLarva_Eating;
                }
                else if (frameIdx == 2) spritePtr = NeoLarva_Happy;
                else if (frameIdx == 1) spritePtr = (currentFrame % 8 == 0) ? NeoLarva_Blink : NeoLarva_Idle1;
                else spritePtr = NeoLarva_Idle1;
            } else {
                switch(petID) {
                    case 1: // LUMI
                        if (frameIdx == 1) spritePtr = Lumi_Idle2;
                        else if (frameIdx == 2) spritePtr = Lumi_Happy;
                        else if (frameIdx == 3) spritePtr = Lumi_Action;
                        else spritePtr = Lumi_Idle1;
                        break;
                    case 2: // RAZ-R
                        if (frameIdx == 1) spritePtr = RazR_Idle2;
                        else if (frameIdx == 2) spritePtr = RazR_Angry;
                        else if (frameIdx == 3) spritePtr = RazR_Attack;
                        else spritePtr = RazR_Idle1;
                        break;
                    case 3: // GLITCH
                        if (frameIdx == 1) spritePtr = Glitch_Idle2;
                        else if (frameIdx == 2) spritePtr = Glitch_Happy;
                        else if (frameIdx == 3) spritePtr = Glitch_Action;
                        else spritePtr = Glitch_Idle1;
                        break;
                    case 4: // AURELIUS
                        if (frameIdx == 1) spritePtr = Aurelius_Defend;
                        else if (frameIdx == 2) spritePtr = Aurelius_Special;
                        else if (frameIdx == 3) spritePtr = Aurelius_Attack;
                        else spritePtr = Aurelius_Idle;
                        break;
                    case 7: // SERAPH.EXE
                        if (frameIdx == 1) spritePtr = SeraphEXE_Fly;
                        else if (frameIdx == 2 || state == GameEngine::EmotionState::EUFORICO) spritePtr = SeraphEXE_Heal;
                        else if (frameIdx == 3) spritePtr = SeraphEXE_Attack;
                        else spritePtr = SeraphEXE_Idle;
                        break;
                    case 5: // WARHOUND-X
                        if (frameIdx == 1) spritePtr = WarhoundX_Run;
                        else if (frameIdx == 2) spritePtr = WarhoundX_Howl;
                        else if (frameIdx == 3) spritePtr = WarhoundX_Attack;
                        else spritePtr = WarhoundX_Idle;
                        break;
                    case 13: spritePtr = NullEntity_Idle1; break;
                    default: spritePtr = NeoLarva_Idle1; break;
                }
            }

            // 3. Renderiza Pet com pulinho (bounce) para dar efeito de respiração
            Renderer::drawSprite(tft.width()/2 - 48, 60, spritePtr, 32, 32, 0x0000, 3, bounceOffset);
            
            // Partículas Zzz se estiver dormindo
            if (state == GameEngine::EmotionState::DORMINDO) {
                static int zzzPhase = 0;
                tft.fillRect(90, 40, 40, 40, TFT_BLACK); // Apaga a área dos Zzz para não borrar
                tft.setTextColor(0x07FF); // Ciano
                tft.setTextDatum(TL_DATUM);
                if (zzzPhase == 0) tft.drawString("z", 90, 60, 2);
                else if (zzzPhase == 1) tft.drawString("z Z", 90, 50, 2);
                else if (zzzPhase == 2) tft.drawString("z Z Z", 90, 40, 2);
                zzzPhase = (zzzPhase + 1) % 3; // Corrigido: cicla direto para não ter tempo morto
            }

            lastFrameIdx = frameIdx;
            lastBounceOffset = bounceOffset;
        }
        
        // 4. Interface de Texto (Apenas se mudar o estado ou ambiente)
        if (state != lastState || envChanged) {
            tft.fillRect(0, 165, 135, 35, 0x0000); 
            tft.setCursor(10, 175);
            tft.setTextColor(borderColor);
            switch(state) {
                case GameEngine::EmotionState::CONTENTE: tft.print("> STATUS: OK"); break;
                case GameEngine::EmotionState::COM_FOME: tft.print("> STATUS: HUNGRY"); break;
                case GameEngine::EmotionState::EM_COLAPSO: tft.print("> ALERT: COLLAPSE"); break;
                case GameEngine::EmotionState::EUFORICO: tft.print("> STATUS: EUPHORIC"); break;
                case GameEngine::EmotionState::DORMINDO: tft.print("> STATUS: Zzz..."); break;
                default: tft.print("> STATUS: ACTIVE"); break;
            }

            tft.setCursor(10, 188);
            tft.setTextColor(0x7BEF);
            tft.printf("LOC: %s", Habitat::getEnvName());
            
            // Atualiza a borda da tela com a cor do estado
            tft.drawRect(0, 0, tft.width(), tft.height(), borderColor);
            
            lastState = state;
            lastEnv = curEnv;
        }
        
        currentFrame++;
    }
}
