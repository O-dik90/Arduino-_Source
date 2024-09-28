#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 is the I2C address, 20x4 LCD

void setup() {
    lcd.init();                      // initialize the lcd 
    lcd.backlight();                 // Turn on the backlight
    //lcd.setCursor(0, 0);             // Set cursor to first column, first row
    //lcd.print("Hello, World!");      // Print something to the LCD
}

void loop() {
    // Your code here
    lcd.setCursor(0, 0);
    lcd.print("on");
    delay(500);
    lcd.setCursor(0, 0);
    lcd.print("off");
    delay(500);

    lcd.clear();
}
