#ifndef SLAVE_CONTROL_H
#define SLAVE_CONTROL_H

#include <Arduino.h>


typedef struct struct_comando {
    bool estado;
} struct_comando;

// Funciones principales
void slaveControlSetup();
void slaveControlLoop();

#endif