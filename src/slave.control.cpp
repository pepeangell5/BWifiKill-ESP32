#include "slave_control.h"
#include <esp_now.h>
#include <WiFi.h>
#include <Preferences.h>
#include <U8g2lib.h>

// Referencias externas al main
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern bool runningApp;

// Variables privadas del módulo
uint8_t macEsclavo[] = {0x00, 0x70, 0x07, 0x26, 0x6E, 0xE8}; 
esp_now_peer_info_t peerInfo;
bool estadoEsclavoActual = true;
Preferences prefsMaestro;

void slaveControlSetup() {
    // 1. Cargar estado guardado
    prefsMaestro.begin("master_cfg", false);
    estadoEsclavoActual = prefsMaestro.getBool("esc_state", true);
    prefsMaestro.end();

    // 2. Configurar ESP-NOW
    WiFi.mode(WIFI_AP_STA); 
    if (esp_now_init() == ESP_OK) {
        memset(&peerInfo, 0, sizeof(peerInfo));
        memcpy(peerInfo.peer_addr, macEsclavo, 6);
        peerInfo.channel = 1; 
        peerInfo.encrypt = false;
        esp_now_add_peer(&peerInfo);
    }
}

void slaveControlLoop() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(12, 15, "MODO ESCLAVO (E8)");
    u8g2.drawHLine(0, 18, 128);
    
    u8g2.setCursor(15, 38); 
    u8g2.print("ESTADO: ");
    u8g2.print(estadoEsclavoActual ? "ACTIVO" : "APAGADO");
    
    u8g2.drawStr(10, 56, "OK: CAMBIAR | BK: SALIR");
    u8g2.sendBuffer();

    // Cambiar estado con OK (32)
    if (digitalRead(32) == LOW) {
        estadoEsclavoActual = !estadoEsclavoActual;

        // Guardar en memoria
        prefsMaestro.begin("master_cfg", false);
        prefsMaestro.putBool("esc_state", estadoEsclavoActual);
        prefsMaestro.end();

        // Enviar comando al esclavo
        struct_comando cmd; 
        cmd.estado = estadoEsclavoActual;
        
        // Enviamos ráfaga para asegurar recepción
        for(int i=0; i<5; i++) {
            esp_now_send(macEsclavo, (uint8_t *) &cmd, sizeof(cmd));
            delay(10);
        }
        delay(300); 
    }

    // Salir con BACK (25)
    if (digitalRead(25) == LOW) {
        runningApp = false;
        delay(300);
    }
}