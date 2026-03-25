#include "jammer.h"
#include <RF24.h>
#include <U8g2lib.h>

extern RF24 jam1; 
extern RF24 jam2;
extern U8G2 u8g2;

#define BTN_UP 26
#define BTN_DOWN 33
#define BTN_OK 32
#define BTN_BACK 25

int jamChannel = 1;
bool isAttacking = false;

const byte noise_payload[] = {0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA};

void jammerSetup() {
    jam1.begin();
    jam2.begin();
    
    jam1.setAddressWidth(3);
    jam1.setRetries(0, 0);
    jam1.setDataRate(RF24_2MBPS); 
    jam1.setAutoAck(false);
    jam1.stopListening();

    jam2.setAddressWidth(3);
    jam2.setRetries(0, 0);
    jam2.setDataRate(RF24_2MBPS);
    jam2.setAutoAck(false);
    jam2.stopListening();
}

void jammerLoop() {
    if (digitalRead(BTN_BACK) == LOW) { 
        isAttacking = false; 
        jam1.stopConstCarrier(); 
        jam2.stopConstCarrier(); 
        delay(200);
        return; 
    }
    
    if (digitalRead(BTN_UP) == LOW) { 
        if(jamChannel < 14) {
            jamChannel++;
            if(isAttacking) { // Si ya estaba atacando, actualizamos frecuencia
                int freq = (jamChannel * 5) + 2;
                jam1.startConstCarrier(RF24_PA_MAX, (uint8_t)freq);
            }
        }
        delay(200); 
    }

    if (digitalRead(BTN_DOWN) == LOW) { 
        if(jamChannel > 1) {
            jamChannel--;
            if(isAttacking) {
                int freq = (jamChannel * 5) + 2;
                jam1.startConstCarrier(RF24_PA_MAX, (uint8_t)freq);
            }
        }
        delay(200); 
    }
    
    if (digitalRead(BTN_OK) == LOW) { 
        isAttacking = !isAttacking; 
        int freq = (jamChannel * 5) + 2;
        if(isAttacking) { 
            jam1.startConstCarrier(RF24_PA_MAX, (uint8_t)freq); // Activar portadora
        } else { 
            jam1.stopConstCarrier(); 
            jam2.stopConstCarrier(); 
        }
        delay(400); 
    }

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 10, "JAMMER PRO (SATURAR)");
    u8g2.setCursor(0, 25); u8g2.print("Canal WiFi: "); u8g2.print(jamChannel);

    if (isAttacking) {
        // --- ANIMACIÓN OPTIMIZADA ---
        for (int i = 20; i < 108; i += 4) { // Saltos de 4 para no saturar bus
            int h = random(5, 30); 
            u8g2.drawVLine(i, 60 - h, h);
        }
        u8g2.drawStr(30, 62, "ATAQUE ACTIVO");
    } else {
        u8g2.drawStr(0, 50, "ESTADO: LISTO");
        u8g2.drawFrame(20, 35, 88, 15);
    }
    u8g2.sendBuffer();

    if (isAttacking) {
        int freq = (jamChannel * 5) + 2;
        // Solo Radio 2 bombardea con paquetes en el loop
        // La Radio 1 ya tiene la portadora fija gracias a startConstCarrier
        jam2.setChannel(freq);
        for(int i = 0; i < 20; i++) { // Reducimos a 20 ráfagas para dar aire al OLED
            jam2.startWrite(&noise_payload, sizeof(noise_payload), true);
        }
    }
}