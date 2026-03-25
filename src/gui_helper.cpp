#include "gui_helper.h"
#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern int menu_index;
extern float currentPos;
extern int TOTAL_OPTIONS;
extern const char* menu_labels[];
extern bool attack_confirmed; 

// --- COLECCIÓN DE ICONOS ÚNICOS ---
void drawBolt(int x, int y) { u8g2.drawLine(x+22, y+2, x+10, y+16); u8g2.drawLine(x+10, y+16, x+24, y+16); u8g2.drawLine(x+24, y+16, x+12, y+30); }
void drawFlame(int x, int y) { u8g2.drawTriangle(x+16, y+2, x+8, y+20, x+24, y+20); u8g2.drawDisc(x+16, y+22, 8); u8g2.setDrawColor(0); u8g2.drawDisc(x+16, y+24, 4); u8g2.setDrawColor(1); }
void drawBan(int x, int y) { u8g2.drawCircle(x+16, y+16, 13); u8g2.drawLine(x+7, y+25, x+25, y+7); }
void drawBTJam(int x, int y) { u8g2.drawLine(x+12, y+6, x+12, y+26); u8g2.drawLine(x+12, y+6, x+20, y+11); u8g2.drawLine(x+20, y+11, x+12, y+16); u8g2.drawLine(x+12, y+16, x+20, y+21); u8g2.drawLine(x+20, y+21, x+12, y+26); u8g2.drawLine(x+6, y+6, x+26, y+26); }
void drawRadar(int x, int y) { u8g2.drawCircle(x+16, y+16, 14); u8g2.drawCircle(x+16, y+16, 7); u8g2.drawLine(x+16, y+2, x+16, y+30); u8g2.drawLine(x+2, y+16, x+30, y+16); u8g2.drawLine(x+16, y+16, x+28, y+5); }
void drawSearch(int x, int y) { u8g2.drawCircle(x+14, y+12, 9); u8g2.drawLine(x+20, y+19, x+28, y+28); }
void drawMegaphone(int x, int y) { u8g2.drawTriangle(x+8, y+16, x+24, y+6, x+24, y+26); u8g2.drawBox(x+4, y+14, 6, 6); }
void drawLock(int x, int y) { u8g2.drawFrame(x+8, y+14, 16, 13); u8g2.drawCircle(x+16, y+14, 7, U8G2_DRAW_UPPER_RIGHT|U8G2_DRAW_UPPER_LEFT); u8g2.drawDisc(x+16, y+21, 2); }
void drawAlien(int x, int y) { u8g2.drawBox(x+6, y+10, 20, 10); u8g2.drawBox(x+4, y+12, 24, 6); u8g2.drawBox(x+8, y+22, 4, 4); u8g2.drawBox(x+20, y+22, 4, 4); u8g2.setDrawColor(0); u8g2.drawBox(x+9, y+13, 4, 4); u8g2.drawBox(x+19, y+13, 4, 4); u8g2.setDrawColor(1); }
void drawFolder(int x, int y) { u8g2.drawFrame(x+4, y+8, 24, 18); u8g2.drawBox(x+4, y+4, 10, 4); }
void drawInfo(int x, int y) { u8g2.drawCircle(x+16, y+16, 14); u8g2.drawBox(x+15, y+12, 2, 10); u8g2.drawDisc(x+16, y+8, 2); }
void drawSnakeIcon(int x, int y) { u8g2.drawFrame(x+6, y+6, 20, 20); u8g2.drawBox(x+10, y+10, 4, 4); u8g2.drawBox(x+14, y+10, 4, 4); u8g2.drawBox(x+14, y+14, 4, 4); u8g2.drawDisc(x+22, y+22, 2); }
void drawMonitorIcon(int x, int y) { u8g2.drawFrame(x+4, y+6, 24, 20); u8g2.drawLine(x+6, y+20, x+12, y+10); u8g2.drawLine(x+12, y+10, x+18, y+24); u8g2.drawLine(x+18, y+24, x+24, y+14); }
void drawScanIcon(int x, int y) { u8g2.drawCircle(x+14, y+12, 7); u8g2.drawLine(x+18, y+18, x+24, y+24); u8g2.drawHLine(x+4, y+28, 24); }
void drawControlIcon(int x, int y) { u8g2.drawFrame(x+6, y+10, 20, 12); u8g2.drawBox(x+14, y+22, 4, 4); u8g2.drawCircle(x+16, y+16, 3); }
void drawRemoteIcon(int x, int y) { u8g2.drawFrame(x+10, y+4, 12, 24); u8g2.drawBox(x+13, y+7, 6, 4); u8g2.drawBox(x+13, y+13, 2, 2); u8g2.drawBox(x+17, y+13, 2, 2); u8g2.drawBox(x+13, y+17, 6, 2); u8g2.drawDisc(x+16, y+23, 2); }

// --- NUEVOS DISEÑOS EXCLUSIVOS ---
void drawPortalIcon(int x, int y) { // EVIL PORTAL: Puerta con remolino
    u8g2.drawFrame(x+8, y+4, 16, 24); 
    u8g2.drawCircle(x+16, y+14, 4); 
    u8g2.drawCircle(x+16, y+14, 2);
    u8g2.drawHLine(x+4, y+28, 24);
}
void drawDashboardIcon(int x, int y) { // WEB DASHBOARD: Monitor de PC
    u8g2.drawFrame(x+4, y+6, 24, 16); 
    u8g2.drawBox(x+12, y+22, 8, 2); 
    u8g2.drawHLine(x+8, y+24, 16);
    u8g2.drawPixel(x+6, y+8); // Detalle pantalla
}
void drawIPScannerIcon(int x, int y) { // IP SCANNER: Lupa con "red"
    u8g2.drawCircle(x+12, y+12, 8); 
    u8g2.drawLine(x+12, y+8, x+12, y+16);
    u8g2.drawLine(x+8, y+12, x+16, y+12);
    u8g2.drawLine(x+18, y+18, x+26, y+26);
}

void drawCentinelaIcon(int x, int y) {
    if (attack_confirmed && (millis() / 250) % 2 == 0) return;
    u8g2.drawFrame(x+8, y+4, 16, 18);          
    u8g2.drawTriangle(x+8, y+22, x+24, y+22, x+16, y+30); 
    u8g2.drawCircle(x+16, y+14, 4);            
    u8g2.drawDisc(x+16, y+14, 1);              
    u8g2.drawLine(x+4, y+10, x+8, y+10);       
    u8g2.drawLine(x+24, y+10, x+28, y+10);
}

// --- DIBUJO DEL MENÚ PRINCIPAL ---
void drawBruceMenu() {
    u8g2.clearBuffer();
    if (abs(currentPos - menu_index) > 0.01) currentPos += (menu_index - currentPos) * 0.25;
    else currentPos = menu_index;

    for (int i = 0; i < TOTAL_OPTIONS; i++) {
        float y_offset = (i - currentPos) * 78;
        int icon_y = 18 + y_offset;
        
        if (icon_y > -32 && icon_y < 64) {
            switch(i) {
                case 0:  drawScanIcon(48, icon_y); break;      // WIFI SCANNER
                case 1:  drawBolt(48, icon_y); break;          // ANALIZADOR
                case 2:  drawSearch(48, icon_y); break;        // BT SCANNER
                case 3:  drawMonitorIcon(48, icon_y); break;   // PACKET MONITOR
                case 4:  drawCentinelaIcon(48, icon_y); break; // MODO CENTINELA
                case 5:  drawBan(48, icon_y); break;           // JAMMER CANAL
                case 6:  drawRadar(48, icon_y); break;         // BARRIDO TOTAL
                case 7:  drawBTJam(48, icon_y); break;         // BT JAMMER
                case 8:  drawMegaphone(48, icon_y); break;     // BEACON SPAM
                case 9:  drawFlame(48, icon_y); break;         // BLE SPAM
                case 10: drawAlien(48, icon_y); break;         // MODO HIBRIDO
                case 11: drawPortalIcon(48, icon_y); break;    // EVIL PORTAL
                case 12: drawIPScannerIcon(48, icon_y); break; // IP SCANNER
                case 13: drawControlIcon(48, icon_y); break;   // CONTROL ESCLAVO
                case 14: drawDashboardIcon(48, icon_y); break; // WEB DASHBOARD
                case 15: drawRemoteIcon(48, icon_y); break;    // BT REMOTE
                case 16: drawFolder(48, icon_y); break;        // LEER LOGS
                case 17: drawSnakeIcon(48, icon_y); break;     // SNAKE GAME
                case 18: drawInfo(48, icon_y); break;          // ABOUT
            }
            
            if (i == menu_index && abs(y_offset) < 5) {
                u8g2.setFont(u8g2_font_6x12_tr);
                int lw = u8g2.getStrWidth(menu_labels[i]);
                u8g2.drawBox(64-(lw/2)-3, 51, lw+6, 12);
                u8g2.setDrawColor(0); u8g2.drawStr(64-(lw/2), 61, menu_labels[i]); u8g2.setDrawColor(1);
            }
        }
    }

    u8g2.setDrawColor(0); u8g2.drawBox(0, 0, 128, 15); u8g2.setDrawColor(1);
    u8g2.drawRFrame(0, 0, 128, 64, 5); u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(22, 11, "BWifiKill v3.3"); u8g2.drawHLine(0, 14, 128);
    u8g2.sendBuffer();
}