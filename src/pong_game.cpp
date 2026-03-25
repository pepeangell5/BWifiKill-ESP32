#include "pong_game.h"
#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

#define BTN_UP 26
#define BTN_DOWN 33

// Variables de posición y física
static float ballX = 64, ballY = 32;
static float ballDX = 2.0, ballDY = 1.0;
static float speedMultiplier = 1.0; // Multiplicador de velocidad
static int paddleY = 24;
static int cpuY = 24;
static int scorePlayer = 0;
static int scoreCPU = 0;

void pongSetup() {
    ballX = 64; ballY = 32;
    ballDX = 2.0; ballDY = 1.0;
    speedMultiplier = 1.0; // Reiniciar velocidad al inicio
    scorePlayer = 0; scoreCPU = 0;
}

void pongLoop() {
    u8g2.clearBuffer();

    // Dibujar Red central
    for(int i=0; i<64; i+=4) u8g2.drawVLine(64, i, 2);

    // --- LÓGICA JUGADOR ---
    if (digitalRead(BTN_UP) == LOW && paddleY > 0) paddleY -= 3;
    if (digitalRead(BTN_DOWN) == LOW && paddleY < 48) paddleY += 3;

    // --- LÓGICA IA CPU ---
    if (ballX > 50) { 
        if (ballY > cpuY + 8 && cpuY < 48) cpuY += 2;
        if (ballY < cpuY + 8 && cpuY > 0) cpuY -= 2;
    }

    // --- FÍSICA Y ACELERACIÓN ---
    ballX += ballDX * speedMultiplier;
    ballY += ballDY * speedMultiplier;

    // Aumentar la velocidad muy lentamente cada frame (0.01% por frame)
    if (speedMultiplier < 3.5) { // Límite máximo para que no sea injugable
        speedMultiplier += 0.0005; 
    }

    // Rebote Paredes Superior/Inferior
    if (ballY <= 0 || ballY >= 63) {
        ballDY = -ballDY;
        speedMultiplier += 0.02; 
    }

    // --- COLISIONES CON PALETAS ---
    // Colisión Jugador (Izquierda)
    if (ballX <= 10 && ballY >= paddleY && ballY <= paddleY + 16) {
        ballDX = -ballDX;
        ballX = 11;
        speedMultiplier += 0.05; // Aumentar velocidad al golpear
    }

    // Colisión CPU (Derecha)
    if (ballX >= 118 && ballY >= cpuY && ballY <= cpuY + 16) {
        ballDX = -ballDX;
        ballX = 117;
        speedMultiplier += 0.05; // Aumentar velocidad al golpear
    }

    // --- SISTEMA DE PUNTUACIÓN ---
    if (ballX < 0) { 
        scoreCPU++; 
        ballX = 64; ballY = 32;
        ballDX = 2.0; 
        speedMultiplier = 1.0; // Resetear velocidad al anotar
    }
    if (ballX > 127) { 
        scorePlayer++; 
        ballX = 64; ballY = 32;
        ballDX = -2.0; 
        speedMultiplier = 1.0; // Resetear velocidad al anotar
    }

    // --- DIBUJAR ELEMENTOS ---
    u8g2.drawFrame(4, paddleY, 4, 16);  // Paleta Jugador
    u8g2.drawFrame(120, cpuY, 4, 16);   // Paleta CPU
    u8g2.drawBox((int)ballX, (int)ballY, 3, 3); // Pelota (convertida a int para dibujar)

    // Marcador
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.setCursor(50, 10); u8g2.print(scorePlayer);
    u8g2.setCursor(75, 10); u8g2.print(scoreCPU);

    // Indicador de "Turbo" (Opcional, muestra la velocidad actual)
    u8g2.setCursor(105, 10); u8g2.print("x"); u8g2.print(speedMultiplier, 1);

    u8g2.sendBuffer();
}