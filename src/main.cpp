#include <Arduino.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <RF24.h>
#include <ESP32Ping.h> 
#include <AsyncUDP.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h> 
#include <esp_wifi.h> 
#include "spectrograph.h"
#include "jammer.h"
#include "wifiscan.h"
#include "total_jammer.h"
#include "btscan.h"
#include "bt_jammer.h"
#include "beacon_spam.h"
#include "hybrid_attack.h"
#include "ip_scanner.h"
#include "snake_game.h" 
#include "packet_monitor.h"
#include "gui_helper.h"
#include "slave_control.h"
#include "web_dashboard.h"
#include "about_info.h"
#include "evil_portal.h"
#include "web_server.h" 
#include "log_viewer.h"
#include "ble_spam.h"
#include "bt_remote.h"
#include "centinela.h" 
#include "games_menu.h"

// --- PROTOTIPOS ---
void wifiscanSetup(); void wifiscanLoop();
void jammerSetup(); void jammerLoop();
void spectrographSetup(); void spectrographLoop();
void totalJammerSetup(); void totalJammerLoop();
void btscanSetup(); void btscanLoop();
void btJammerSetup(); void btJammerLoop();
void beaconSpamLoop(); void bleSpamLoop(); 
void hybridAttackLoop(); 
void evilPortalLoop(); void logViewerLoop(); 
void startWebServer();
void btRemoteLoop(); 
void centinelaLoop(); 

// --- VARIABLES DE ESTADO ---
RTC_DATA_ATTR bool forceBleMode = false;
bool isTotalAttacking = false; 
bool isHybridActive = false; 
String target_ssid = "Ninguna"; 
int target_channel = 1;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 22, 21);
RF24 jam1(5, 17, 16000000);
RF24 jam2(16, 4, 16000000);

AsyncWebServer asyncServer(80);

#define BTN_UP 26
#define BTN_DOWN 33
#define BTN_OK 32
#define BTN_BACK 25 

int menu_index = 0;
float currentPos = 0; 
bool runningApp = false;
int TOTAL_OPTIONS = 19; 

// --- NUEVO ORDEN DE MENÚ ---
const char* menu_labels[] = {
    "WIFI SCANNER",    // 0
    "ANALIZADOR",      // 1
    "BT SCANNER",      // 2
    "PACKET MONITOR",  // 3
    "MODO CENTINELA",  // 4
    "JAMMER CANAL",    // 5
    "BARRIDO TOTAL",   // 6
    "BT JAMMER",       // 7
    "BEACON SPAM",     // 8
    "BLE SPAM (POP)",  // 9
    "MODO HIBRIDO",    // 10
    "EVIL PORTAL",     // 11
    "IP SCANNER",      // 12
    "CONTROL ESCLAVO", // 13
    "WEB DASHBOARD",   // 14
    "BT REMOTE",       // 15
    "LEER LOGS",       // 16
    "ARCADE",      // 17
    "ABOUT"            // 18
};

void setup() {
    Serial.begin(115200); u8g2.begin();
    pinMode(BTN_UP, INPUT_PULLUP); pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_OK, INPUT_PULLUP); pinMode(BTN_BACK, INPUT_PULLUP);

    for (int i = 0; i <= 100; i += 5) {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_10x20_tr); 
        u8g2.drawStr(22, 28, "BWifikill"); 
        u8g2.setFont(u8g2_font_6x12_tr);
        u8g2.drawStr(36, 45, "PepeAngell"); 
        u8g2.drawHLine(30, 48, 68);
        u8g2.drawFrame(14, 58, 100, 3); 
        u8g2.drawBox(16, 59, (i * 96) / 100, 1);
        u8g2.sendBuffer();
        delay(35); 
    }

    slaveControlSetup(); 
    jam1.begin(); jam2.begin();
}

void loop() {
    if (!runningApp) {
        if (digitalRead(BTN_UP) == LOW) { menu_index--; if(menu_index < 0) menu_index = TOTAL_OPTIONS-1; delay(200); }
        if (digitalRead(BTN_DOWN) == LOW) { menu_index++; if(menu_index >= TOTAL_OPTIONS) menu_index = 0; delay(200); }
        
        drawBruceMenu(); 

        if (digitalRead(BTN_OK) == LOW) {
            runningApp = true;
            if (menu_index == 0) wifiscanSetup();
            else if (menu_index == 1) spectrographSetup();
            else if (menu_index == 2) btscanSetup();
            else if (menu_index == 3) monitorSetup();
            else if (menu_index == 5) jammerSetup();
            else if (menu_index == 6) totalJammerSetup();
            else if (menu_index == 7) btJammerSetup();
            else if (menu_index == 12) ipScannerSetup();
            else if (menu_index == 13) slaveControlSetup(); 
            else if (menu_index == 14) startWebServer();
            else if (menu_index == 17) resetSnake();
            delay(300);
        }
    } else {
        // --- EJECUCIÓN ---
        if (menu_index == 0) wifiscanLoop(); 
        else if (menu_index == 1) spectrographLoop(); 
        else if (menu_index == 2) btscanLoop(); 
        else if (menu_index == 3) monitorLoop();
        else if (menu_index == 4) centinelaLoop(); 
        else if (menu_index == 5) jammerLoop(); 
        else if (menu_index == 6) totalJammerLoop(); 
        else if (menu_index == 7) btJammerLoop(); 
        else if (menu_index == 8) beaconSpamLoop(); 
        else if (menu_index == 9) bleSpamLoop(); 
        else if (menu_index == 10) hybridAttackLoop(); 
        else if (menu_index == 11) evilPortalLoop();
        else if (menu_index == 12) ipScannerLoop();
        else if (menu_index == 13) slaveControlLoop(); 
        else if (menu_index == 14) webDashboardLoop(); 
        else if (menu_index == 15) btRemoteLoop(); 
        else if (menu_index == 16) logViewerLoop(); 
        else if (menu_index == 17) gamesLoop(); 
        else if (menu_index == 18) aboutLoop();

        // --- SALIDA ---
        if (digitalRead(BTN_BACK) == LOW && menu_index != 12) {
            runningApp = false; 
            esp_wifi_set_promiscuous(false); 
            WiFi.mode(WIFI_OFF);
            jam1.stopConstCarrier(); jam2.stopConstCarrier(); 
            if(menu_index == 14) asyncServer.end();
            delay(300);
            
            // Reinicio para limpiar RAM en módulos de radio
            if(menu_index == 15 || menu_index == 2 || menu_index == 9 || menu_index == 13) {
                ESP.restart();
            }
        }
    }
}