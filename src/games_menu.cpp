#include "games_menu.h"
#include <U8g2lib.h>
#include "snake_game.h"
#include "pong_game.h"
#include "flappy_game.h"
#include "space_game.h"
#include "dino_game.h"

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

#define BTN_UP 26
#define BTN_DOWN 33
#define BTN_OK 32
#define BTN_BACK 25

static int game_index = 0;
static const int TOTAL_GAMES = 5; 
bool inGame = false;

const char* game_labels[] = {"SNAKE", "PONG", "FLAPPY", "INVADERS", "DINO RUN"};

void drawGamesMenu() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(35, 12, "ARCADE MODE");
    u8g2.drawHLine(0, 15, 128);

    int start_item = (game_index >= 2) ? game_index - 1 : 0;
    if (game_index == TOTAL_GAMES - 1 && TOTAL_GAMES > 2) start_item = TOTAL_GAMES - 2;

    for (int i = 0; i < 2; i++) {
        int current_item = start_item + i;
        if (current_item >= TOTAL_GAMES) break;
        if (current_item == game_index) {
            u8g2.drawBox(0, 20 + (i * 15), 128, 14);
            u8g2.setDrawColor(0);
        }
        u8g2.drawStr(10, 31 + (i * 15), game_labels[current_item]);
        u8g2.setDrawColor(1);
    }
    u8g2.drawHLine(0, 53, 128);
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.drawStr(5, 62, "UP/DN: Nav  OK: Jugar  BK: Salir");
    u8g2.sendBuffer();
}

void gamesLoop() {
    if (!inGame) {
        drawGamesMenu();
        if (digitalRead(BTN_UP) == LOW) { game_index--; if(game_index < 0) game_index = TOTAL_GAMES - 1; delay(200); }
        if (digitalRead(BTN_DOWN) == LOW) { game_index++; if(game_index >= TOTAL_GAMES) game_index = 0; delay(200); }
        if (digitalRead(BTN_OK) == LOW) {
            inGame = true;
            if (game_index == 0) resetSnake(); 
            else if (game_index == 1) pongSetup();
            else if (game_index == 2) flappySetup();
            else if (game_index == 3) spaceSetup();
            else if (game_index == 4) dinoSetup();
            delay(300);
        }
    } else {
        switch(game_index) {
            case 0: snakeLoop(); break;
            case 1: pongLoop(); break;
            case 2: flappyLoop(); break;
            case 3: spaceLoop(); break;
            case 4: dinoLoop(); break;
        }
        if (digitalRead(BTN_BACK) == LOW) { inGame = false; u8g2.setDrawColor(1); delay(300); }
    }
}