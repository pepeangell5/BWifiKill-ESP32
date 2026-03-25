#include "about_info.h"
#include <U8g2lib.h>

// Referencias externas 
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern bool runningApp;

// Pines de botones (GPIO)
#define BTN_BACK 25
#define BTN_UP   26
#define BTN_DOWN 33

// Variables de Scroll y Matrix
static int about_scroll = 0; 
struct MatrixDrop {
    int x;
    float y;
    float speed;
    char val;
};

static MatrixDrop drops[12]; 

// Inicializa la lluvia Matrix
void initMatrix() {
    for(int i=0; i<12; i++) {
        drops[i].x = random(0, 128); // X aleatorio en toda la pantalla
        drops[i].y = (float)random(-64, 0); // Empiezan arriba
        drops[i].speed = (float)random(10, 32) / 10.0; // Velocidad aleatoria
        // Caracteres Hexadecimales (0-9, A-F)
        drops[i].val = (random(0,2) == 0) ? (char)random(48,58) : (char)random(65,71);
    }
}

void aboutLoop() {
    static bool firstRun = true;
    if (firstRun) { initMatrix(); firstRun = false; }

    // 1. SALIR CON BOTÓN BACK
    if (digitalRead(BTN_BACK) == LOW) {
        about_scroll = 0;
        firstRun = true; // Reset matrix para la próxima entrada
        runningApp = false;
        delay(200);
        return;
    }

    // 2. CONTROL DE SCROLL (UP/DOWN)
    if (digitalRead(BTN_UP) == LOW) { if(about_scroll > 0) about_scroll -= 3; delay(10); } 
    if (digitalRead(BTN_DOWN) == LOW) { if(about_scroll < 90) about_scroll += 3; delay(10); }

    u8g2.clearBuffer();

    // 3. DIBUJAR LLUVIA MATRIX (FONDO COMPLETO)
    u8g2.setFont(u8g2_font_4x6_tf); // Fuente muy pequeña para el fondo
    u8g2.setDrawColor(1); // Blanco
    for(int i=0; i<12; i++) {
        u8g2.drawGlyph(drops[i].x, (int)drops[i].y, drops[i].val);
        drops[i].y += drops[i].speed; // Movimiento de caída
        // Reinicio al salir por abajo
        if (drops[i].y > 64) {
            drops[i].y = (float)random(-20, 0);
            drops[i].x = random(0, 128);
            drops[i].val = (random(0,2) == 0) ? (char)random(48,58) : (char)random(65,71);
        }
    }

    // 4. VENTANA DE TEXTO ESTILIZADA (Solución al problema)
    u8g2.setDrawColor(0); // Color negro (tapa la lluvia detrás)
    u8g2.drawBox(10, 18, 108, 44); 
    u8g2.setDrawColor(1); // Color blanco (borde y texto)
    u8g2.drawFrame(10, 18, 108, 44); // Borde estético

    // 5. CONTENIDO CON SCROLL (Dentro de la ventana negra)
    u8g2.setFont(u8g2_font_6x10_tr); 
    int textX = 15; // Margen izquierdo dentro del recuadro
    u8g2.drawStr(textX, 32 - about_scroll, "Dev: PepeAngell");
    u8g2.drawStr(textX, 44 - about_scroll, "HW: ESP32 + nRF24");
    u8g2.drawStr(textX, 56 - about_scroll, "NRF24+PA+LNA");
    u8g2.drawStr(textX, 68 - about_scroll, "Hybrid Mode: ON");
    u8g2.drawStr(textX, 80 - about_scroll, "IG: @PepeAngell");
    u8g2.drawStr(textX, 92 - about_scroll, "System: BWifiKill");
    u8g2.drawStr(textX, 104 - about_scroll, "Ver: 3.3 Stable");
    u8g2.drawStr(textX, 116 - about_scroll, "202 build");
    u8g2.drawStr(textX, 128 - about_scroll, "by PepeAngell"); 

    // 6. ENCABEZADO FIJO (Estilo Hacker con Máscara Superior)
   
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, 0, 128, 17); 
    u8g2.setDrawColor(1);
    
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(5, 12, "> TERMINAL_INFO"); 
    u8g2.drawHLine(0, 16, 128); 

    // Cursor parpadeante estilo terminal arriba a la derecha
    if ((millis() / 450) % 2 == 0) u8g2.drawBox(110, 4, 6, 9);

    u8g2.sendBuffer();
}