#include "dino_game.h"
#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

#define BTN_OK 32
#define BTN_DOWN 33

// Física y Animación
static float dinoY = 48.0;
static float dinoVelocity = 0.0;
static float currentGravity = 0.5;   // Gravedad inicial
static const float dinoJump = -6.5;  // Salto base
static bool isJumping = false;
static bool isDucking = false;
static int animFrame = 0;

// Obstáculos
static float obstX = 128.0;
static float obstY = 42.0;    
static int obstType = 0;      
static float gameSpeed = 3.2; 
static int score = 0;
static bool gameOver = false;

void dinoSetup() {
    dinoY = 48.0;
    dinoVelocity = 0.0;
    currentGravity = 0.5; // Reset
    isJumping = false;
    isDucking = false;
    obstX = 128.0;
    obstY = 42.0;
    obstType = 0;
    gameSpeed = 3.2;
    score = 0;
    gameOver = false;
}

// Función para dibujar el Dino (Pixel Art)
void drawDino(int x, int y, bool ducking, int frame) {
    if (!ducking) {
        // Dino Normal
        u8g2.drawBox(x, y, 8, 8);         
        u8g2.drawBox(x + 4, y - 4, 6, 4); 
        u8g2.drawPixel(x + 8, y - 3);     
        // Patas
        if (frame == 0) { u8g2.drawVLine(x + 1, y + 8, 2); u8g2.drawVLine(x + 5, y + 8, 2); }
        else { u8g2.drawVLine(x + 2, y + 8, 2); u8g2.drawVLine(x + 6, y + 8, 2); }
    } else {
        // Dino Agachado
        u8g2.drawBox(x, y + 4, 12, 4);    
        u8g2.drawBox(x + 8, y + 2, 6, 3); 
        u8g2.drawPixel(x + 12, y + 3);    
    }
}

// Función: Dibujar Cactus Realista y Delgado
void drawRealCactus(int x, int y) {
    u8g2.drawBox(x, y, 4, 16); // Tronco central
    u8g2.drawBox(x - 3, y + 5, 2, 6); // Brazo Izq
    u8g2.drawHLine(x - 2, y + 9, 2);
    u8g2.drawBox(x + 5, y + 2, 2, 6); // Brazo Der
    u8g2.drawHLine(x + 4, y + 6, 1);
}

void dinoLoop() {
    u8g2.clearBuffer();

    if (!gameOver) {
        // --- ENTRADA ---
        // Saltar
        if (digitalRead(BTN_OK) == LOW && !isJumping && !isDucking) {
            dinoVelocity = dinoJump;
            isJumping = true;
        }

        // --- LÓGICA DE FAST FALL Y AGACHADO ---
        bool downPressed = (digitalRead(BTN_DOWN) == LOW);
        
        // Determinar qué tipo de gravedad aplicar
        float effectiveGravity = currentGravity;

        if (downPressed) {
            if (isJumping) {
                // MECÁNICA FAST FALL: Si está en el aire y presiona DOWN, 
                // caemos más rápido (se duplica la gravedad actual)
                effectiveGravity = currentGravity * 2.0;
                isDucking = false; // No se agacha en el aire
            } else {
                // Agachado normal en el suelo
                isDucking = true;
                isJumping = false;
            }
        } else {
            // No se presiona DOWN
            if (!isJumping) {
                isDucking = false;
            }
            // Si está saltando y suelta DOWN, vuelve a la gravedad normal (parabólica)
        }

        // --- FÍSICA APLICADA ---
        if (!isDucking) {
            // Aplicar la gravedad efectiva calculada (normal o fast fall)
            dinoVelocity += effectiveGravity;
            dinoY += dinoVelocity;
        }

        // Suelo
        if (dinoY >= 48.0) { 
            dinoY = 48.0; 
            dinoVelocity = 0; 
            isJumping = false; 
        }

        // --- MOVIMIENTO Y DIFICULTAD ---
        obstX -= gameSpeed;
        if (obstX < -20) {
            obstX = 128;
            score++;
            
            // Dificultad incremental
            if (gameSpeed < 9.5) {
                gameSpeed += 0.18; // Aumento de velocidad un poco más rápido
                if (currentGravity < 0.85) {
                    currentGravity += 0.02; 
                }
            }
            
            // Obstáculos: Aves a partir de 8 puntos
            if (score > 8 && random(0, 10) > 7) {
                obstType = 1; 
                obstY = 32.0; 
            } else {
                obstType = 0; 
                obstY = 42.0;
            }
        }

        // Animación de patas
        if ((millis() / 120) % 2 == 0) animFrame = 0; else animFrame = 1;

        // --- COLISIONES ---
        if (obstType == 0) { // Cactus
            if (obstX < 25 && obstX > 8) {
                if (dinoY > 38) gameOver = true;
            }
        } else { // Ave
            if (obstX < 23 && obstX > 10) {
                if (!isDucking) gameOver = true; 
            }
        }

        // --- DIBUJAR ---
        u8g2.drawHLine(0, 58, 128); // Suelo
        drawDino(15, (int)dinoY, isDucking, animFrame);
        
        if (obstType == 0) {
            drawRealCactus((int)obstX, (int)obstY); // Cactus
        } else {
            // Ave
            u8g2.drawBox((int)obstX, (int)obstY, 10, 4); 
            if (animFrame == 0) u8g2.drawTriangle(obstX+2, obstY, obstX+8, obstY, obstX+5, obstY-5); 
            else u8g2.drawTriangle(obstX+2, obstY+4, obstX+8, obstY+4, obstX+5, obstY+9);
        }

        // Marcador Score y Velocidad (v)
        u8g2.setFont(u8g2_font_5x7_tr);
        u8g2.setCursor(2, 8); u8g2.print("SCORE: "); u8g2.print(score);
        u8g2.setCursor(95, 8); u8g2.print("v:"); u8g2.print(gameSpeed, 1);

    } else {
        // GAME OVER
        u8g2.setFont(u8g2_font_6x12_tr);
        u8g2.drawStr(35, 30, "EXTINCTION");
        u8g2.setFont(u8g2_font_5x7_tr);
        u8g2.drawStr(25, 60, "OK: REINTENTAR");
        if (digitalRead(BTN_OK) == LOW) { dinoSetup(); delay(300); }
    }
    u8g2.sendBuffer();
}