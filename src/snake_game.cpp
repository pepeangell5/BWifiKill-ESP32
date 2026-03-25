#include "snake_game.h"
#include <U8g2lib.h>

// Referencias externas
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern bool runningApp;

// Estructuras y variables del juego
struct Point { int8_t x, y; };
Point snake[25]; 
Point food;
int8_t snake_len = 3; 
int8_t dirX = 1, dirY = 0;
bool gameOver = false; 
unsigned long lastMove = 0;
int currentScore = 0; 
RTC_DATA_ATTR int highScore = 0;

void resetSnake() {
    snake_len = 3; 
    currentScore = 0;
    snake[0] = {10, 8}; 
    snake[1] = {9, 8}; 
    snake[2] = {8, 8};
    dirX = 1; 
    dirY = 0;
    food = { (int8_t)random(2, 29), (int8_t)random(3, 14) };
    gameOver = false;
}

void snakeLoop() {
    if (gameOver) {
        u8g2.clearBuffer(); 
        u8g2.setFont(u8g2_font_6x12_tr);
        u8g2.drawStr(35, 25, "GAME OVER");
        u8g2.setCursor(30, 40); 
        u8g2.print("Score: "); 
        u8g2.print(currentScore);
        u8g2.sendBuffer();
        
        if (digitalRead(32) == LOW) { // BTN_OK
            resetSnake(); 
            delay(200); 
        }
        return;
    }

    // Controles
    if (digitalRead(26) == LOW) { int8_t t = dirX; dirX = -dirY; dirY = t; delay(150); } 
    if (digitalRead(33) == LOW) { int8_t t = dirX; dirX = dirY; dirY = -t; delay(150); } 

    if (millis() - lastMove > 130) {
        lastMove = millis();
        for (int i = snake_len - 1; i > 0; i--) snake[i] = snake[i - 1];
        
        snake[0].x += dirX; 
        snake[0].y += dirY;

        // Colisiones con bordes (Límites visuales)
        if (snake[0].x < 0 || snake[0].x > 31 || snake[0].y < 2 || snake[0].y > 15) gameOver = true;
        
        // Colisiones con el cuerpo
        for (int i = 1; i < snake_len; i++) {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) gameOver = true;
        }

        // Comer fruta
        if (snake[0].x == food.x && snake[0].y == food.y) {
            currentScore += 10; 
            if (currentScore > highScore) highScore = currentScore;
            if (snake_len < 25) snake_len++;
            food = { (int8_t)random(0, 31), (int8_t)random(2, 15) };
        }
    }

    // --- DIBUJO ---
    u8g2.clearBuffer(); 
    
    // 1. Dibujar el Marco de colisión (Paredes)
    // El área de juego es 32x14 cuadros de 4x4 píxeles.
    // X de 0 a 127, Y de 8 a 63.
    u8g2.drawFrame(0, 8, 128, 56); 

    // 2. UI Superior (Score)
    u8g2.setFont(u8g2_font_4x6_tf);
    u8g2.setCursor(2, 6); u8g2.print("SC: "); u8g2.print(currentScore);
    u8g2.setCursor(95, 6); u8g2.print("HI: "); u8g2.print(highScore);
    
    // 3. Dibujar Serpiente
    for (int i = 0; i < snake_len; i++) {
        u8g2.drawBox(snake[i].x * 4, snake[i].y * 4, 3, 3);
    }
    
    // 4. Dibujar Comida
    u8g2.drawDisc(food.x * 4 + 2, food.y * 4 + 2, 2);
    
    u8g2.sendBuffer();
}