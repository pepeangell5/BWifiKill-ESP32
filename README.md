# 🛰️ BWifiKill v3.3 - Ultimate ESP32 Network Suite & Jammer

![Version](https://img.shields.io/badge/Version-3.3-blue)
![Platform](https://img.shields.io/badge/Platform-ESP32-orange)
![License](https://img.shields.io/badge/License-MIT-green)
![Hardware](https://img.shields.io/badge/Hardware-NRF24L01-red)

**BWifiKill** es una potente suite de auditoría de redes inalámbricas y radiofrecuencia (2.4GHz) desarrollada para el **ESP32 DevKit v1**. Esta herramienta permite realizar desde análisis de espectro pasivo hasta pruebas de penetración activa y denegación de servicio (Jamming).
<p align="center">
  <img src="img/1.jpg" width="60%" alt="1">
</p>

---

## 🛠️ Requisitos Previos

Antes de flashear, asegúrate de cumplir con lo siguiente para evitar errores de compilación o fallos de hardware:

1.  **Software:** Instalar [VS Code](https://code.visualstudio.com/) + la extensión [PlatformIO](https://platformio.org/).
2.  **Drivers:** Instalar los drivers [CP210x](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) o CH340 para que tu PC reconozca el ESP32.
3.  **Hardware Pro Tip:** Los módulos **NRF24L01** son muy sensibles al ruido eléctrico. Se recomienda soldar un **condensador de 10µF a 100µF** entre los pines VCC y GND del módulo para garantizar la estabilidad durante el modo Jammer.

---

## 📡 Menú de Funciones (19 Herramientas)

1.  📡 **WIFI SCANNER:** Escaneo detallado de redes (SSID, RSSI, Canal) con función de **Clonación**.
<p align="center">
  <img src="img/2.jpg" width="60%" alt="2">
</p>

2.  📊 **ANALIZADOR:** Escáner de espectro en tiempo real para visualizar la actividad en 2.4GHz. **Incluye alertas visuales de ataque.**
<p align="center">
  <img src="img/3.jpg" width="60%" alt="3">
</p>

3.  🔍 **BT SCANNER:** Rastreador de dispositivos Bluetooth Classic y BLE.
<p align="center">
  <img src="img/4.jpg" width="60%" alt="4">
</p>

4.  📈 **PACKET MONITOR:** Monitor gráfico de tráfico WiFi y flujo de paquetes en tiempo real.
<p align="center">
  <img src="img/5.jpg" width="60%" alt="5">
</p>

5.  🛡️ **MODO CENTINELA:** Sistema de vigilancia activa que detecta ataques de desautenticación y Jammers RF en tiempo real.
<p align="center">
  <img src="img/6.jpg" width="60%" alt="6">
</p>

6.  🚫 **JAMMER CANAL:** Bloqueo de WiFi en un canal específico (Requiere NRF24L01).
<p align="center">
  <img src="img/7.jpg" width="60%" alt="7">
</p>

7.  🌊 **BARRIDO TOTAL:** Interferencia agresiva recorriendo todos los canales del espectro.
<p align="center">
  <img src="img/8.jpg" width="60%" alt="8">
</p>

8.  📵 **BT JAMMER:** Interferencia dirigida a las frecuencias de salto (hopping) de Bluetooth.
<p align="center">
  <img src="img/9.jpg" width="60%" alt="9">
</p>

9.  📣 **BEACON SPAM:** Inunda el entorno con cientos de redes WiFi falsas personalizables.
<p align="center">
  <img src="img/10.jpg" width="60%" alt="10">
</p>

10. 📱 **BLE SPAM (POP):** Envía notificaciones emergentes de emparejamiento a dispositivos iOS y Android cercanos.
<p align="center">
  <img src="img/11.jpg" width="60%" alt="11">
</p>

11. 🔄 **MODO HÍBRIDO:** Ataque combinado de desautenticación WiFi + interferencia de hardware (RF).
<p align="center">
  <img src="img/12.jpg" width="60%" alt="12">
</p>

12. 🎭 **EVIL PORTAL:** Creación de APs falsos con Portales Cautivos para pruebas de Phishing.
<p align="center">
  <img src="img/13.jpg" width="60%" alt="13">
</p>

13. 🎯 **IP SCANNER PRO:** Centro de comando local con **Ping, UDP Flood y Port Scan**.
<p align="center">
  <img src="img/14.jpg" width="60%" alt="14">
</p>

14. 🛰️ **CONTROL ESCLAVO:** Módulo para comandar otras unidades BWifiKill de forma remota.
<p align="center">
  <img src="img/15.jpg" width="60%" alt="15">
</p>

15. 🌐 **WEB DASHBOARD:** Servidor local para gestionar y descargar logs desde tu celular.
<p align="center">
  <img src="img/16.jpg" width="60%" alt="16">
</p>

16. 🎮 **BT REMOTE:** Control remoto Bluetooth para interactuar con dispositivos emparejados.
<p align="center">
  <img src="img/17.jpg" width="60%" alt="17">
</p>

17. 📂 **LEER LOGS:** Visor interno para revisar credenciales capturadas (User:Pass).
<p align="center">
  <img src="img/18.jpg" width="60%" alt="18">
</p>

18. 🕹️ **ARCADE MODE:** Submenú de entretenimiento con 5 minijuegos clásicos (Snake, Pong, Flappy, Invaders y Dino Run).
<p align="center">
  <img src="img/19.jpg" width="60%" alt="19">
</p>

19. ℹ️ **ABOUT:** Versión v3.3 con fondo animado Matrix y scroll de créditos del firmware.
<p align="center">
  <img src="img/20.jpg" width="60%" alt="20">
</p>

---

## 🎮 Arcade Mode (Juegos Incluidos)

Hemos añadido un módulo dedicado al entretenimiento con físicas mejoradas:
* 🐍 **SNAKE:** El clásico juego de la serpiente con guardado de High Score.
* 🏓 **PONG:** Enfréntate a la CPU con velocidad incremental.
* 🐦 **FLAPPY ESP:** Esquiva tuberías con micro-saltos de precisión.
* 🚀 **INVADERS:** Defiende la pantalla de hordas de naves enemigas.
* 🦖 **DINO RUN:** Inspirado en Chrome, con aves, cactus realistas, agachado y mecánica de **Fast Fall**.

---

## ⚔️ Command Center (Ataques Dirigidos)

Al seleccionar una IP en el **IP Scanner**, puedes ejecutar:
* **UDP Flood:** Saturación de ancho de banda para pruebas de denegación de servicio local.
* **Port Scan:** Identificación inteligente de dispositivos (Detecta Windows PC, iPhones, iPads, Samsung, etc.).
* **Ping Test:** Verificación de latencia y estado del host.

---

## 🔌 Configuración de Hardware (Pinout)

### 📡 Jammers (NRF24L01)
| Módulo | CE Pin | CSN Pin | Bus SPI (SCK/MISO/MOSI) |
| :--- | :--- | :--- | :--- |
| **Jammer 1** | GPIO 5 | **GPIO 17 (TX2)** | 18 / 19 / 23 |
| **Jammer 2** | **GPIO 16 (RX2)** | GPIO 4 | 18 / 19 / 23 |

<p align="center">
  <img src="img/Pines_NRF24.png" width="60%" alt="Diagrama NRF24">
</p>


### 🎮 Interfaz y Pantalla
| Elemento | Pin ESP32 | Función |
| :--- | :--- | :--- |
| **OLED SSD1306** | 21 (SDA) / 22 (SCL) | Interfaz Visual I2C |
| **Botón UP** | GPIO 26 | Navegar Arriba |
| **Botón DOWN** | GPIO 33 | Navegar Abajo |
| **Botón OK** | GPIO 32 | Confirmar / Entrar |
| **Botón BACK** | GPIO 25 | Volver / Salir |
| **Botón CLON** | GPIO 27 | Función Especial |

<p align="center">
  <img src="img/conexiones_ESP32.png" width="45%" alt="Conexiones Generales">
</p>

<p align="center">
  <img src="img/Pines_pantalla.png" width="45%" alt="Pines Pantalla">
</p>

---

## 📖 Guía de Uso Avanzado

### Modo Centinela (Novedad v3.3)
El **Modo Centinela** analiza el tráfico de gestión WiFi para detectar patrones de ataque. Si la intensidad de paquetes de desautenticación supera el umbral configurado, el sistema activará una alerta visual parpadeante. Es capaz de detectar tanto ataques lógicos como interferencia de hardware (RF).

### Evil Portal & Web Dashboard
Para capturar datos con el **Evil Portal**, el ESP32 genera una red abierta. Una vez que una víctima ingresa sus datos, estos se guardan en la memoria interna (SPIFFS/LittleFS).
1.  Entra a **Web Dashboard** en el menú principal.
2.  Conecta tu celular a la red WiFi que indica la pantalla.
3.  Accede a la IP mostrada en el navegador para ver, descargar o borrar los logs de capturas.

--------------------------------------------------------------

## 🚀 Instalación y Flasheo

Elige el método que prefieras: el **Instalador Web** (más rápido) o la **Compilación Manual** (para desarrolladores).

### 🌐 Método 1: Web Installer (Recomendado)
Puedes flashear el firmware directamente desde tu navegador sin instalar nada en tu PC.
1. Entra al instalador oficial: [**BWifiKill Web Flasher**](https://pepeangell5.github.io/BWifiKill-ESP32/)
2. Conecta tu **ESP32** al puerto USB (Usa Google Chrome o MS Edge).
3. Presiona el botón **⚡ INICIAR FLASH ⚡**.
4. **IMPORTANTE:** Si el proceso no inicia o marca error de puerto, mantén presionado el botón **BOOT** en tu placa al momento de conectar o iniciar el flasheo.

### 💻 Método 2: Compilación Manual (PlatformIO)
Si deseas modificar el código o contribuir al desarrollo:
1.  **Clona el repositorio:**
    ```bash
    git clone [https://github.com/PepeAngell5/BWifiKill-ESP32.git](https://github.com/PepeAngell/BWifiKill-ESP32.git)
    ```
2.  Abre la carpeta en **VS Code** con la extensión **PlatformIO** instalada.
3.  El archivo `platformio.ini` descargará automáticamente las dependencias: `U8g2`, `RF24`, `ESP32Ping`, `AsyncUDP` y `BleKeyboard`.
4.  Conecta tu ESP32 y presiona el icono de la flecha (**Upload**) en la barra inferior.

--------------------------------------------------------------
## ⚖️ Descargo de Responsabilidad
Este proyecto es exclusivamente para fines educativos y de auditoría de seguridad autorizada. El uso de funciones de desautenticación o interferencia de radiofrecuencia en redes o dispositivos ajenos sin permiso explícito es ilegal. El desarrollador no se hace responsable del mal uso de esta herramienta.

### 🛠️ Créditos
Desarrollado por **PepeAngell**.
* **Tutorial Base:** [FR4NK SOLO](https://www.youtube.com/@FR4NKSOLO)
* **Firmware Inspiración:** [Bruce Firmware](https://github.com/pr3y/Bruce)

