#include <Encoder.h>
#include <I2C_LCD.h>

Encoder myEnc(2, 3);
Encoder myEnc2(4, 5);
int pinBoutonA = 6;
int pinBoutonB = 8;
I2C_LCD LCD;
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

void setup() {
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");
}

long oldPosition  = -999;
long oldPosition2  = -999;

void loop() {
  oldPosition = myEnc.read();
  oldPosition2 = myEnc2.read();
  Serial.print(oldPosition);
  Serial.print(" ");
  Serial.println(oldPosition2);
}
