#include <Encoder.h>
#include <I2C_LCD.h>

I2C_LCD LCD;
Encoder enc1(2, 3);
int pinBoutonA = 6;
int pinBoutonB = 8;
uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.

int W = 128, H = 64;
int Xzone = 113,
    Yzone = 49,
    edite = 0;
boolean boutonA = false,
        boutonB = false,
        oldBoutonA = false,
        oldBoutonB = false,
        cursorOn = false,
        allNor = true,
        acceuilLoad = false;
long pose1 = 0,
     deltaPose1 = 0;

boolean interrupteurs[9] = {false};
int addressIP[4] = {10, 16, 42, 1};
char tabEnigme[4][4] = {{'0', '0', '0', '\0'},
                        {'0', '0', '0', '\0'},
                        {'0', '0', '0', '\0'},
                        {'0', '0', '0', '\0'}};


void setup() {
  LCD.Init();
  Serial.begin(9600);
  Serial.println("Basic  Test:");
  LCD.CleanAll(WHITE);
  LCD.DisplayConf(AllNOR);
  LCD.CursorConf(OFF, 2);
}

void loop() {
    oldBoutonA = boutonA;
    oldBoutonB = boutonB;
    boutonA = digitalRead(pinBoutonA);
    boutonB = digitalRead(pinBoutonB);
    
    deltaPose1 = enc1.read()-pose1;
    pose1 = enc1.read();
    
    if (!acceuilLoad && !boutonA) {
      oldBoutonA = false;
      acceuilLoad = true;
      updateAcceil();
    }
    
    if (!oldBoutonB && boutonB) {
      for (int i=0; i<4; i++)
        if (Xzone == i*32+1 && Yzone == 32+1) {
          edite ++;
          if (edite > 3) {
              LCD.CursorConf(OFF, 4);
              cursorOn = false;
              edite = 0;
              if (addressIP[i] == atoi(tabEnigme[i]))
                addressIP[i] = -1;
          } else {
            LCD.CursorGotoXY(i*32-4+edite*8, 32+8, 8, 16);
            if (!cursorOn) {
                LCD.CursorConf(ON, 4);
                cursorOn = true;
            }
          }
          updateAcceil();
        }
    }
    if (edite == 0) {
        if (!oldBoutonA && boutonA) {
            int nextMenu = 0;
            for (int i=0; i<4; i++)
              if (Xzone == i*32+1 && Yzone == 32+1)
                nextMenu = i+1;
            
            allNor = !allNor || nextMenu != 0;
            if (allNor) LCD.DisplayConf(AllNOR);
            else  LCD.DisplayConf(AllREV);
            Xzone = 1+int(Xzone/4)*4;
            Yzone = 1+int(Yzone/4)*4;

            acceuilLoad = false;
            switch(nextMenu) {
              case 1: enigme_1(); break;
              case 2: enigme_2(); break;
              case 3: enigme_3(); break;
              case 4: enigme_4(); break;
              default: acceuilLoad = true;
            }
        }
        if (deltaPose1 != 0) {
            if (allNor) Xzone = int(Xzone+deltaPose1+128)%128;
            else Yzone = int(Yzone+deltaPose1+64)%64;
            updateAcceil();
        }
    } else if (abs(deltaPose1) > 0) {
        for (int i=0; i<4; i++)
          if (Xzone == i*32+1 && Yzone == 32+1) {
            if (deltaPose1 > 0) tabEnigme[i][edite-1] ++;
            if (deltaPose1 < 0) tabEnigme[i][edite-1] --;
            tabEnigme[i][edite-1] = char((tabEnigme[i][edite-1]-'0'+10)%10+'0');
            LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_BAC);
            LCD.DispStringAt(tabEnigme[i], i*32+4, 32+8);
          }
    }
}

void updateAcceil() {
    LCD.CleanAll(WHITE);
    
    LCD.DrawRectangleAt(34, 2, 60, 28, BLACK_NO_FILL);
    LCD.FontModeConf(Font_6x12, FM_ANL_AAA, BLACK_NO_BAC);
    LCD.DispStringAt("Enigme "+edite, 37, 9);
      
    LCD.DrawRectangleAt(2, 2, 28, 28, BLACK_NO_FILL);
    if (interrupteurs[0]) {
        LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_NO_BAC);
        LCD.DispStringAt("BTS", 4, 8);
    } else if (Xzone == 1 && Yzone == 1)
        interrupteurs[0] = true;
    
    LCD.DrawRectangleAt(98, 2, 28, 28, BLACK_NO_FILL);
    if (interrupteurs[1]) {
        LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC);
        LCD.DispStringAt("SN", 100, 6);
    } else if (Xzone == 97 && Yzone == 1)
        interrupteurs[1] = true;
    
    for (int i=0; i<3; i++)
      if (interrupteurs[2+i]) {
        LCD.DrawRectangleAt(31+i*32, 62, 2, 2, BLACK_FILL);
      } else if (Xzone == 17+i*32 && Yzone == 49)
          interrupteurs[2+i] = true;
          
    LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_NO_BAC);
    for (int i=0; i<4; i++) {
        int x = i*32, y = 32;
        LCD.DrawRectangleAt(x, y, 30, 30, WHITE_FILL);
        if (addressIP[i] != -1) {
            if (Xzone == x+1 && Yzone == y+1) interrupteurs[5+i] = true;
            if (interrupteurs[5+i])
              LCD.DispStringAt(tabEnigme[i], x+4, y+8);
            if (!(Xzone == x+1 && Yzone == y+1 && edite == 0)) 
              LCD.DrawRectangleAt(x+2, y+2, 28, 28, BLACK_NO_FILL);
        } else LCD.DispStringAt(atoi(tabEnigme[i]), x+4, y+8);
    }
      
    drawRectNoFill(Xzone, Yzone, 30, 30);
    drawRectNoFill(Xzone+2, Yzone+2, 26, 26);
}

void drawRectNoFill(int x, int y, int w, int h) {
    drawLine(x, y, w-1, true);
    drawLine(x, y+h-1, w-1, true);
    drawLine(x, y+1, h-3, false);
    drawLine(x+w-1, y+1, h-3, false);
}

void drawLine(int x, int y, int l, boolean horizontal) {
    if (x >= W) x -= W;
    if (y >= H) y -= H;
    if (horizontal) {
      if (x+l >= W) {
        LCD.DrawHLineAt(x, W-1, y, BLACK);
        LCD.DrawHLineAt(0, x+l-W, y, BLACK);
      } else LCD.DrawHLineAt(x, x+l, y, BLACK);
    } else {
      if (y+l >= H) {
        LCD.DrawVLineAt(y, H-1, x, BLACK);
        LCD.DrawVLineAt(0, y+l-H, x, BLACK);
      } else LCD.DrawVLineAt(y, y+l, x, BLACK);
    }
}

void enigme_1() {
    Serial.println(digitalRead(pinBoutonA), digitalRead(pinBoutonB));
    LCD.CleanAll(WHITE);
    LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC);
    LCD.DispStringAt("Enigme 1:", 4, 40);
    delay(500);
}

void enigme_2() {
    Serial.println(digitalRead(pinBoutonA), digitalRead(pinBoutonB));
    LCD.CleanAll(WHITE);
    LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC);
    LCD.DispStringAt("Enigme 1:", 4, 40);
    delay(500);
}

void enigme_3() {
    Serial.println(digitalRead(pinBoutonA), digitalRead(pinBoutonB));
    LCD.CleanAll(WHITE);
    LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC);
    LCD.DispStringAt("Enigme 1:", 4, 40);
    delay(500);
}

void enigme_4() {
    Serial.println(digitalRead(pinBoutonA), digitalRead(pinBoutonB));
    LCD.CleanAll(WHITE);
    LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC);
    LCD.DispStringAt("Enigme 1:", 4, 40);
    delay(500);
}
