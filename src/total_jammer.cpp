#include "total_jammer.h"
#include <RF24.h>
#include <U8g2lib.h>

extern RF24 jam1; 
extern RF24 jam2;
extern U8G2 u8g2;
extern bool isTotalAttacking;

#define BTN_OK 32
#define BTN_BACK 25

byte full_hop_list[] = { 32, 34, 46, 48, 50, 52, 0, 1, 2, 4, 6, 8, 22, 24, 26, 28, 30, 74, 76, 78, 80, 82, 84, 86, 5, 30, 55, 10, 35, 60, 15, 40, 65 };
int total_chans = sizeof(full_hop_list);

void totalJammerSetup() {
    jam1.begin(); jam2.begin();
    jam1.setAutoAck(false); 
    jam1.setDataRate(RF24_2MBPS); 
    jam1.setCRCLength(RF24_CRC_DISABLED);
    jam2.setAutoAck(false); 
    jam2.setDataRate(RF24_2MBPS); 
    jam2.setCRCLength(RF24_CRC_DISABLED);
}

void totalJammerLoop() {
    if (digitalRead(BTN_OK) == LOW) { 
        isTotalAttacking = !isTotalAttacking; 
        if(!isTotalAttacking) { 
            jam1.stopConstCarrier(); 
            jam2.stopConstCarrier(); 
        } else { 
            // Iniciamos el ataque en canales base
            jam1.startConstCarrier(RF24_PA_MAX, 45); 
            jam2.startConstCarrier(RF24_PA_MAX, 45); 
        }
        delay(400); 
    }

    if (digitalRead(BTN_BACK) == LOW) { 
        isTotalAttacking = false; 
        jam1.stopConstCarrier(); 
        jam2.stopConstCarrier(); 
        return; 
    }

    if (isTotalAttacking) {
        // --- BUCLE DE ATAQUE CRÍTICO ---
        while(isTotalAttacking) {
            // Saltamos canales muchas veces antes de dibujar
            for (int r = 0; r < 50; r++) { // 50 vueltas de ataque puro
                for (int i = 0; i < total_chans; i++) {
                    jam1.setChannel(full_hop_list[i]);
                    jam2.setChannel(full_hop_list[total_chans - 1 - i]);
                }
                
                if (digitalRead(BTN_OK) == LOW || digitalRead(BTN_BACK) == LOW) {
                    isTotalAttacking = false;
                    jam1.stopConstCarrier();
                    jam2.stopConstCarrier();
                    return;
                }
            }

            // --- REFRESCAR PANTALLA.
            u8g2.clearBuffer();
            // Efecto de estática
            for (int i = 0; i < 128; i += 8) {
                u8g2.drawVLine(i + random(0,4), random(0,20), random(5,15));
                u8g2.drawVLine(i + random(0,4), 45, random(5,15));
            }
            u8g2.setDrawColor(0); u8g2.drawBox(5, 25, 118, 15); u8g2.setDrawColor(1);
            u8g2.drawFrame(5, 25, 118, 15);
            u8g2.setFont(u8g2_font_6x12_tr);
            u8g2.drawStr(12, 36, "!!! ESPECTRO CAIDO !!!");
            u8g2.sendBuffer(); 
        }
    } else {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_6x10_tr);
        u8g2.drawStr(0, 10, "BARRIDO TOTAL (W+B)");
        u8g2.drawStr(10, 40, "OK: ATAQUE TOTAL");
        u8g2.sendBuffer();
    }
}