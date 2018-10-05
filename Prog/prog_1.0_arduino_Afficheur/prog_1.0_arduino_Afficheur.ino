#include <Wire.h>
#include <I2C_LCD.h>
I2C_LCD LCD;
uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.

const int buttonPin = 2;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status

void setup(void)
{
    pinMode(buttonPin, INPUT);
    Wire.begin();         //I2C controller initialization.
}

void loop(void)
{
    LCD.CharGotoXY(0,10);  
    buttonState = digitalRead(buttonPin);

    if (buttonState == HIGH) {
        // turn LED on:
    LCD.CleanAll(WHITE);    //Clean the screen with black or white.
    delay(1000);            //Delay for 1s. 
    LCD.print("dejssy l'abruti");

    }
  

}
