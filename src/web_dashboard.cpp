#include "web_dashboard.h"
#include <U8g2lib.h>

// Referencia externa a la pantalla
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

void webDashboardLoop() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(15, 20, "ADMIN WEB ACTIVO");
    u8g2.drawHLine(0, 25, 128);
    u8g2.setCursor(5, 40); 
    u8g2.print("SSID: BWifiKill_Admin");
    u8g2.setCursor(5, 52); 
    u8g2.print("IP: 192.168.4.1");
    u8g2.sendBuffer();
}