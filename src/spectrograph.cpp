#include "spectrograph.h"
#include <RF24.h>
#include <U8g2lib.h>

extern RF24 jam1;
extern RF24 jam2;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

#define BTN_BACK 25
#define UMBRAL_ENERGIA_TOTAL 800 // Umbral de energía acumulada en todo el espectro
#define ALERT_DURATION 2500     

uint8_t peak_heights[125];
unsigned long lastPeakReset = 0;
unsigned long alertStartTime = 0;

void spectrographSetup() {
    jam1.begin();
    jam1.setPALevel(RF24_PA_MAX);
    jam1.setDataRate(RF24_2MBPS);
    jam1.startListening();

    jam2.begin();
    jam2.setPALevel(RF24_PA_MAX);
    jam2.setDataRate(RF24_2MBPS);
    jam2.startListening();

    memset(peak_heights, 0, sizeof(peak_heights));
}

void spectrographLoop() {
    u8g2.clearBuffer();
    long totalEnergy = 0; // Sumatoria de toda la actividad detectada

    for (int i = 0; i < 62; i++) {
        int ch1 = i;
        int ch2 = i + 63;

        jam1.setChannel(ch1);
        jam2.setChannel(ch2);
        delayMicroseconds(100);
        
        int s1 = 0, s2 = 0;
        for(int s=0; s<40; s++) {
            if(jam1.testCarrier()) s1++;
            if(jam2.testCarrier()) s2++;
        }
        
        // Suma de la fuerza de cada canal al total
        totalEnergy += s1;
        totalEnergy += s2;

        // Dibujo de barrass
        if (s1 > 0) {
            u8g2.drawVLine(ch1, 64 - s1, s1);
            if (s1 > peak_heights[ch1]) peak_heights[ch1] = s1;
        }
        u8g2.drawPixel(ch1, 64 - peak_heights[ch1]);

        if (s2 > 0) {
            u8g2.drawVLine(ch2, 64 - s2, s2);
            if (s2 > peak_heights[ch2]) peak_heights[ch2] = s2;
        }
        u8g2.drawPixel(ch2, 64 - peak_heights[ch2]);
    }

    // --- Lógica ---
    // Si la suma de energía de todos los canales es alta, hay un jammer barriendo
    if (totalEnergy > UMBRAL_ENERGIA_TOTAL) {
        alertStartTime = millis();
    }

    if (millis() - alertStartTime < ALERT_DURATION) {
        u8g2.setDrawColor(1);
        u8g2.drawBox(0, 15, 128, 18); 
        u8g2.setDrawColor(0);
        u8g2.setFont(u8g2_font_7x14_tf); 
        u8g2.drawStr(8, 29, "ATAQUE DETECTADO");
        u8g2.setDrawColor(1);
    } else {
        u8g2.setFont(u8g2_font_5x7_tr);
        u8g2.drawStr(0, 7, "ESPECTROMETRO 2.4ghz");
        // u8g2.setCursor(80, 7); u8g2.print(totalEnergy); 
    }

    if (millis() - lastPeakReset > 4000) {
        for(int p=0; p<125; p++) if(peak_heights[p] > 0) peak_heights[p]--;
        lastPeakReset = millis();
    }

    u8g2.sendBuffer();
}