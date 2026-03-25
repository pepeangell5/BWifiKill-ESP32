#include <Arduino.h>
#include <BleKeyboard.h>
#include <U8g2lib.h>

// MUY IMPORTANTE: 
extern BleKeyboard bleKeyboard; 
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

void type(const char* text, int wait = 800) {
    for(int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        if (c == '-') {
            bleKeyboard.write('/'); 
        } 
        else if (c == '/') {
            bleKeyboard.press(KEY_LEFT_SHIFT);
            bleKeyboard.write('7');
            bleKeyboard.releaseAll();
        }
        else {
            bleKeyboard.write(c);
        }
        delay(35);
    }
    bleKeyboard.write(KEY_RETURN);
    delay(wait);
}

void runPepeScript() {
    u8g2.clearBuffer();
    u8g2.drawStr(10, 32, "INYECTANDO...");
    u8g2.sendBuffer();

    delay(3000); 

    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.press('r');
    delay(200); 
    bleKeyboard.releaseAll();
    delay(1200);

    type("notepad", 1500);
    type("Control total activado.", 1000);

    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.press('r');
    delay(200); 
    bleKeyboard.releaseAll();
    delay(1000);
    
    type("powershell", 1500);
    type("ping 8.8.8.8 -t", 2000);
}