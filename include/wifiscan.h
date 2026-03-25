#ifndef WIFISCAN_H
#define WIFISCAN_H

#include <Arduino.h>

// --- FUNCIONES PRINCIPALES ---
void wifiscanSetup();
void wifiscanLoop();

void attemptConnection(String ssid);

#endif