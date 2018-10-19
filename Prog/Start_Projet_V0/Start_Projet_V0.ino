
#include <Encoder.h>
#include <Wire.h>
#include <I2C_LCD.h>

Encoder enc1(2, 3);
I2C_LCD LCD;
uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.
enum menu {accueil, enigme1, enigme2, enigme3, enigme4};

void setup(void) {
    Wire.begin();         //I2C controller initialization.
    Serial.begin(9600);
    Serial.println("Basic Encoder Test:");
    Adafruit_SSD1306 display(I2C_LCD_ADDRESS)
}

int w = 128, h = 64;
long oldPosition  = -999;

void loop(void) {
    long newPosition = enc1.read();
    if (newPosition != oldPosition) {
      oldPosition = newPosition;
      Serial.println(newPosition);
    }
  

    //8*16 font size��auto new line��black character on white back ground.
    LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_BAC); 
    LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC); 

    LCD.DispStringAt(newPosition, 0, 0);
    
    LCD.DrawRectangleAt(10, 15, 107, 33, BLACK_FILL);
    LCD.DrawDotAt(127, 63, BLACK);
    LCD.DrawDotAt(120, 50, BLACK);
    
    display()    //Clean the screen with black or white.
    delay(50);
}
