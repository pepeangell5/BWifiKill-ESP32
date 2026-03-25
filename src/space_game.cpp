#include "space_game.h"
#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// Botones para control vertical (clásico)
#define BTN_UP 26
#define BTN_DOWN 33
#define BTN_OK 32

// Estructura básica para naves y balas
struct Entity { float x, y; bool active; };

static Entity player;
static Entity bullet;
// Matriz de enemigos (3 filas x 4 columnas para que quepan bien)
static Entity enemies[12]; 
static int enemyDir = 1;
static float enemySpeed = 0.4;
static int score = 0;
static bool gameOver = false;

// Reinicia o inicializa el estado del juego
void spaceSetup() {
    // Jugador abajo al centro
    player = {60.0, 55.0, true};
    bullet = {0, 0, false};
    score = 0;
    enemySpeed = 0.4;
    enemyDir = 1;
    gameOver = false;

    // Inicializar formación de enemigos arriba
    for(int i=0; i<12; i++) {
        // Formación 4x3: (Columna * Espacio, Fila * Espacio + Margen)
        enemies[i] = { (float)(20 + (i%4)*25), (float)(10 + (i/4)*12), true };
    }
}

void spaceLoop() {
    u8g2.clearBuffer();

    if (!gameOver) {
        // --- CONTROLES (UP/DOWN MUEVEN IZQ/DER) ---
        if (digitalRead(BTN_UP) == LOW && player.x > 5) player.x -= 3;
        if (digitalRead(BTN_DOWN) == LOW && player.x < 115) player.x += 3;
        
        // --- DISPARO MÁS RÁPIDO ---
        if (digitalRead(BTN_OK) == LOW && !bullet.active) {
            // Bala sale del centro de la nave hacia ARRIBA
            bullet = {player.x + 3, player.y - 4, true};
        }

        // --- MOVIMIENTO BALA (RÁPIDO) ---
        if (bullet.active) {
            bullet.y -= 5; // Aumentada la velocidad de disparo
            if (bullet.y < 0) bullet.active = false;
        }

        // --- MOVIMIENTO ENEMIGOS (IA DE HORDA) ---
        bool touchEdge = false;
        for(int i=0; i<12; i++) {
            if (!enemies[i].active) continue;
            
            // Movimiento horizontal
            enemies[i].x += enemyDir * enemySpeed;
            
            // Verificar si alguno tocó el borde lateral
            if (enemies[i].x > 120 || enemies[i].x < 5) touchEdge = true;
            
            // Colisión con jugador o invasión total (derrota)
            if (enemies[i].y >= player.y - 2) gameOver = true;
        }

        if (touchEdge) {
            enemyDir *= -1;
            for(int i=0; i<12; i++) {
                if(enemies[i].active) enemies[i].y += 5; // Bajan 5 píxeles
            }
            // Aumenta la velocidad ligeramente para más dificultad
            if (enemySpeed < 2.5) enemySpeed += 0.08; 
        }

        // --- COLISIONES BALA-ENEMIGO ---
        if (bullet.active) {
            for(int i=0; i<12; i++) {
                // Caja de colisión simple (6x6)
                if (enemies[i].active && bullet.x >= enemies[i].x && bullet.x <= enemies[i].x + 6 &&
                    bullet.y >= enemies[i].y && bullet.y <= enemies[i].y + 6) {
                    enemies[i].active = false;
                    bullet.active = false;
                    score += 10;
                }
            }
        }

        // --- VERIFICAR VICTORIA (RESPAWN) ---
        bool allDead = true;
        for(int i=0; i<12; i++) if(enemies[i].active) allDead = false;
        if (allDead) spaceSetup(); 

        // --- DIBUJAR ELEMENTOS ---
        // Nave Jugador (Triángulo hacia arriba)
        u8g2.drawTriangle(player.x, player.y+6, player.x+6, player.y+6, player.x+3, player.y); 
        
        // Bala (Línea vertical)
        if (bullet.active) u8g2.drawVLine(bullet.x, bullet.y, 4); 

        // Horda de Invasores
        for(int i=0; i<12; i++) {
            if (enemies[i].active) {
                // Dibujar invasor 
                u8g2.drawFrame(enemies[i].x, enemies[i].y, 6, 5);
                u8g2.drawPixel(enemies[i].x, enemies[i].y);
                u8g2.drawPixel(enemies[i].x+5, enemies[i].y);
                u8g2.drawPixel(enemies[i].x+2, enemies[i].y+6);
                u8g2.drawPixel(enemies[i].x+3, enemies[i].y+6);
            }
        }

        // Marcador
        u8g2.setFont(u8g2_font_5x7_tr);
        u8g2.setCursor(2, 8); u8g2.print("SCORE: "); u8g2.print(score);

    } else {
        // PANTALLA DE DERROTA
        u8g2.setFont(u8g2_font_6x12_tr);
        u8g2.drawStr(30, 30, "SYSTEM BREACHED");
        u8g2.setCursor(45, 45); u8g2.print("SCORE: "); u8g2.print(score);
        u8g2.setFont(u8g2_font_5x7_tr);
        u8g2.drawStr(25, 60, "OK: REINTENTAR");
        if (digitalRead(BTN_OK) == LOW) { spaceSetup(); delay(300); }
    }
    u8g2.sendBuffer();
}