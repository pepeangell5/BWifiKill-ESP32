#include <Arduino.h>
#include <BleKeyboard.h>
#include <U8g2lib.h>
#include "bt_remote.h"

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern bool runningApp;
extern void runPepeScript(); 

#define PIN_UP 26    
#define PIN_DOWN 33  
#define PIN_OK 32    
#define PIN_BACK 25  
#define PIN_OFF 27   // Botón de Reset de App

static bool isBleRemoteActive = false;
static bool modeSelected = false;
static int sub_menu_index = 0;

BleKeyboard bleKeyboard("Wireless Kbd 3000", "Logitech", 100);

void btRemoteLoop() {
    pinMode(PIN_UP, INPUT_PULLUP);
    pinMode(PIN_DOWN, INPUT_PULLUP);
    pinMode(PIN_OK, INPUT_PULLUP);
    pinMode(PIN_BACK, INPUT_PULLUP);
    pinMode(PIN_OFF, INPUT_PULLUP);

    // 1. MENU DE SELECCION
    if (!modeSelected) {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_6x12_tr);
        u8g2.drawStr(5, 15, "--- MODO REMOTO ---");
        
        if (sub_menu_index == 0) u8g2.drawButtonUTF8(5, 35, U8G2_BTN_INV, 118, 2, 2, "1. CONTROL MEDIA");
        else u8g2.drawStr(10, 35, "1. CONTROL MEDIA");
        
        if (sub_menu_index == 1) u8g2.drawButtonUTF8(5, 52, U8G2_BTN_INV, 118, 2, 2, "2. PAYLOAD (ATAQUE)");
        else u8g2.drawStr(10, 52, "2. PAYLOAD (ATAQUE)");
        
        u8g2.sendBuffer();

        if (digitalRead(PIN_UP) == LOW) { sub_menu_index = 0; delay(150); } 
        if (digitalRead(PIN_DOWN) == LOW) { sub_menu_index = 1; delay(150); } 
        if (digitalRead(PIN_OK) == LOW) { modeSelected = true; delay(300); } 
        if (digitalRead(PIN_BACK) == LOW) { runningApp = false; delay(300); }
        return; 
    }

    // 2. INICIO DE BLUETOOTH
    if (!isBleRemoteActive) {
        bleKeyboard.begin();
        isBleRemoteActive = true;
    }

    // 3. LOGICA DENTRO DEL MODO
    u8g2.clearBuffer();
    if (bleKeyboard.isConnected()) {
        u8g2.drawStr(5, 12, sub_menu_index == 0 ? "MODO: CONTROL" : "MODO: PAYLOAD");
        u8g2.drawBox(0, 15, 128, 13);
        u8g2.setDrawColor(0);
        u8g2.drawStr(30, 25, "CONECTADO");
        u8g2.setDrawColor(1);
        
        if (sub_menu_index == 0) { // CONTROL MEDIA
            u8g2.drawStr(5, 42, "UP:-V DW:+V OK:P/P");
            u8g2.drawStr(5, 55, "DER: RESET APP");

            if (digitalRead(PIN_UP) == LOW) { bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN); delay(150); }
            if (digitalRead(PIN_DOWN) == LOW) { bleKeyboard.write(KEY_MEDIA_VOLUME_UP); delay(150); }
            if (digitalRead(PIN_OK) == LOW) { bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE); delay(400); }
        } else { // PAYLOAD
            u8g2.drawStr(5, 45, "OK: Disparar");
            if (digitalRead(PIN_OK) == LOW) { runPepeScript(); delay(1000); }
        }

        // --- LA CLAVE DEL EXITO ---
        // Si presionas el 27 o el 25 estando conectado, reiniciamos el ESP.
        // Es la UNICA forma que encontré para que el Bluetooth vuelva a funcionar al 100%.
        if (digitalRead(PIN_OFF) == LOW || digitalRead(PIN_BACK) == LOW) {
            u8g2.clearBuffer();
            u8g2.drawStr(30, 35, "REINICIANDO...");
            u8g2.sendBuffer();
            delay(500);
            ESP.restart(); 
        }
    } else {
        u8g2.drawStr(5, 15, "BUSCANDO PC...");
        u8g2.drawStr(5, 55, "DER: Volver");
        if (digitalRead(PIN_OFF) == LOW || digitalRead(PIN_BACK) == LOW) {
            ESP.restart(); 
        }
    }
    u8g2.sendBuffer();
}