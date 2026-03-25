#include "flappy_game.h"
#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
#define BTN_OK 32

// Física del pájaro (Ajustada para micro-saltos)
static float birdY = 32.0;
static float velocity = 0.0;
static float gravity = 0.15;      // Gravedad base
static float jumpImpulse = -1.2;  // Salto muy corto (requiere clics rápidos)

// Variables de los tubos y dificultad
static float pipeX = 128;
static float pipeSpeed = 1.8;     // Velocidad inicial de los tubos
static int gapY = 32;
static int gapSize = 28;          // Hueco inicial
static int score = 0;
static bool gameOver = false;

// Temporizador para aumentar dificultad
static unsigned long lastDifficultyBoost = 0;

void flappySetup() {
    birdY = 32.0;
    velocity = 0.0;
    pipeX = 128;
    pipeSpeed = 1.8;              // Reset de velocidad
    gapY = random(18, 42); 
    score = 0;
    gameOver = false;
    lastDifficultyBoost = millis();
}

void flappyLoop() {
    u8g2.clearBuffer();

    if (!gameOver) {
        // --- ENTRADA (Micro-saltos) ---
        if (digitalRead(BTN_OK) == LOW) {
            velocity = jumpImpulse; 
        }

        // --- FÍSICA ---
        velocity += gravity;
        birdY += velocity;

        // --- DIFICULTAD PROGRESIVA ---
        // Cada 5 segundos aumentamos la velocidad de los tubos
        if (millis() - lastDifficultyBoost > 5000) {
            if (pipeSpeed < 4.5) pipeSpeed += 0.2; // Aumenta la velocidad
            lastDifficultyBoost = millis();
        }

        // --- MOVIMIENTO DE TUBOS ---
        pipeX -= pipeSpeed; 
        if (pipeX < -15) {
            pipeX = 128;
            gapY = random(18, 42);
            score++;
        }

        // --- COLISIONES ---
        // Si toca techo o suelo
        if (birdY < 0 || birdY > 63) gameOver = true;

        // Si toca los tubos
        if (pipeX < 25 && pipeX > 10) {
            if (birdY < gapY - (gapSize/2) || birdY > gapY + (gapSize/2)) {
                gameOver = true;
            }
        }

        // --- DIBUJAR ---
        // El pájaro
        u8g2.drawBox(15, (int)birdY, 4, 4); 
        
        // Tubos (con ancho de 12 para que se vean mejor al correr rápido)
        u8g2.drawBox((int)pipeX, 0, 12, gapY - (gapSize/2)); 
        u8g2.drawBox((int)pipeX, gapY + (gapSize/2), 12, 64); 

        // Marcador e indicador de velocidad (opcional para debug)
        u8g2.setFont(u8g2_font_5x7_tr);
        u8g2.setCursor(2, 8);
        u8g2.print("SCORE: "); u8g2.print(score);
        u8g2.setCursor(100, 8);
        u8g2.print("v"); u8g2.print(pipeSpeed, 1);

    } else {
        // PANTALLA DE MUERTE
        u8g2.setFont(u8g2_font_6x12_tr);
        u8g2.drawStr(35, 30, "GAME OVER");
        u8g2.setCursor(45, 45);
        u8g2.print("SCORE: "); u8g2.print(score);
        u8g2.setFont(u8g2_font_5x7_tr);
        u8g2.drawStr(25, 60, "OK: REINTENTAR");

        if (digitalRead(BTN_OK) == LOW) {
            flappySetup();
            delay(300);
        }
    }
    u8g2.sendBuffer();
}