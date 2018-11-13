#include <Encoder.h>
#include <I2C_LCD.h>
#include "Enigme.h"

I2C_LCD LCD;
Encoder enc1(2, 3);
Encoder enc2(4, 5);
int pinBoutonA = 6;
int pinBoutonB = 8;
uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.

int W = 128, H = 64;
int menu = 0,
    Xzone = 113,
    Yzone = 49,
    edite = 0;
boolean boutonA = false,
        boutonB = false,
        oldBoutonA = false,
        oldBoutonB = false,
        cursorOn = false;
long pose1 = 0,
     pose2 = 0,
     deltaPose1 = 0,
     deltaPose2 = 0;
boolean interrupteurs[5] = {false, false, false, false, false};

int 

Enigme enigme1(1, 192, 0, 32, &Xzone, &Yzone, &edite);
Enigme enigme2(2, 1, 32, 32, &Xzone, &Yzone, &edite);
Enigme enigme3(3, 43, 64, 32, &Xzone, &Yzone, &edite);
Enigme enigme4(4, 1, 96, 32, &Xzone, &Yzone, &edite);

void setup() {
  LCD.Init();
  Serial.begin(9600);
  Serial.println("Basic  Test:");
}

long oldPosition  = -999;

void loop() {
    oldBoutonA = boutonA;
    oldBoutonB = boutonB;
    boutonA = digitalRead(pinBoutonA);
    boutonB = digitalRead(pinBoutonB);
    
    deltaPose1 = enc1.read()-pose1;
    deltaPose2 = enc2.read()-pose2;
    pose1 = enc1.read();
    pose2 = enc2.read();
    
    if (menu == -1 && !boutonA) {
      oldBoutonA = false;
      menu = 0;
    }
    if (boutonA) Serial.print("true ");
    else Serial.print("false ");
    if (boutonB) Serial.print("true ");
    else Serial.print("false ");
    Serial.print(deltaPose1);
    Serial.print(" ");
    Serial.print(deltaPose2);
    Serial.print(" ");
    Serial.print(pose1);
    Serial.print(" ");
    Serial.println(pose2);
}
