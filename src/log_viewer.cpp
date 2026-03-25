#include <Arduino.h>
#include <U8g2lib.h>
#include "FS.h"
#include "SPIFFS.h"

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// --- CONFIGURACIÓN DE PINES ---
#define BTN_UP 26
#define BTN_DOWN 33
#define BTN_OK 32
#define BTN_BACK 25
#define BTN_ERASE 27 // Botón de activación de borrado

int logScroll = 0;
int maxLogScroll = 0; 

void logViewerLoop() {
    // Configuración inicial de pines
    static bool pinsConfigured = false;
    if (!pinsConfigured) {
        pinMode(BTN_ERASE, INPUT_PULLUP);
        pinMode(BTN_OK, INPUT_PULLUP);
        pinsConfigured = true;
    }

    u8g2.clearBuffer();
    
    // --- ENCABEZADO FIJO ---
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(0, 10, "HISTORIAL DE LOGS");
    u8g2.drawLine(0, 13, 128, 13);

    if (!SPIFFS.begin(true)) {
        u8g2.drawStr(0, 35, "Error SPIFFS");
    } else {
        File file = SPIFFS.open("/logins.txt", FILE_READ);
        if (!file || file.size() == 0) {
            u8g2.drawStr(0, 35, "No hay registros.");
            u8g2.drawStr(0, 50, "[BACK] para salir");
        } else {
            // --- NAVEGACIÓN ---
            if (digitalRead(BTN_UP) == LOW) { 
                logScroll += 6; 
                if (logScroll > 0) logScroll = 0; 
                delay(30); 
            }
            if (digitalRead(BTN_DOWN) == LOW) { 
                logScroll -= 6; 
                if (logScroll < maxLogScroll) logScroll = maxLogScroll; 
                delay(30); 
            }

            // --- RENDERIZADO CON CLIPPING ---
            int currentY = 25 + logScroll;
            int totalHeight = 0;

            while (file.available()) {
                String line = file.readStringUntil('\n');
                line.trim();

                if (currentY > 15 && currentY < 75) {
                    if (line.length() > 21) {
                        u8g2.setCursor(0, currentY); u8g2.print(line.substring(0, 21));
                        currentY += 10;
                        if (currentY > 15 && currentY < 75) {
                            u8g2.setCursor(0, currentY); u8g2.print(line.substring(21));
                        }
                    } else {
                        u8g2.setCursor(0, currentY); u8g2.print(line);
                    }
                }
                currentY += 12;
                totalHeight += (line.length() > 21) ? 22 : 12;
            }
            maxLogScroll = -(totalHeight - 40); 
            if (maxLogScroll > 0) maxLogScroll = 0;
            file.close();

            // --- LÓGICA DE SEGURIDAD (ERASE + OK) ---
            if (digitalRead(BTN_ERASE) == LOW) {
                u8g2.setDrawColor(0);
                u8g2.drawBox(5, 20, 118, 30);
                u8g2.setDrawColor(1);
                u8g2.drawFrame(5, 20, 118, 30);
                u8g2.drawStr(12, 32, "PRESIONA [OK]");
                u8g2.drawStr(12, 45, "PARA CONFIRMAR");
                u8g2.sendBuffer();

                // Si mientras presionas 27, pulsas 32...
                if (digitalRead(BTN_OK) == LOW) {
                    SPIFFS.remove("/logins.txt");
                    logScroll = 0;
                    u8g2.clearBuffer();
                    u8g2.drawStr(30, 35, "LOGS ELIMINADOS");
                    u8g2.sendBuffer();
                    delay(1000);
                }
            }
        }
    }

    u8g2.sendBuffer();
}