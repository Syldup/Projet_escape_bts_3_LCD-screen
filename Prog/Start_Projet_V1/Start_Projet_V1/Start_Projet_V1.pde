/*
#include <Encoder.h>
#include <I2C_LCD.h>

Encoder enc1(2, 3);
Encoder enc2(4, 5);
int pinBoutonA = 6;
int pinBoutonB = 8;
I2C_LCD LCD;
uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.

Enigme enigme1(1, 192, 0, 32);
Enigme enigme2(2, 1, 32, 32);
Enigme enigme3(3, 43, 64, 32);
Enigme enigme4(4, 1, 96, 32);
*/

int W = 128, H = 64;
int menu = 3,
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
boolean[] interrupteurs = {false, false, false, false, false};


Encoder enc1 = new Encoder('a', 'q');
Encoder enc2 = new Encoder('z', 's');
I2C_LCD LCD = new I2C_LCD();
char pinBoutonA = 'w';
char pinBoutonB = 'x';

Enigme enigme1 = new Enigme(1, 192, 0, 32);
Enigme enigme2 = new Enigme(2, 1, 32, 32);
Enigme enigme3 = new Enigme(3, 43, 64, 32);
Enigme enigme4 = new Enigme(4, 1, 96, 32);
// */
void setup() {
    /*
    LCD.Init();
    //Wire.setClock(10000);
    Serial.begin(9600);
    Serial.println("Basic Test:");
    */
    background(128);
    size(1153, 513);
    textAlign(LEFT, TOP);
    noSmooth();
    strokeWeight(1);
    // */
    pinMode(pinBoutonA, INPUT);
    pinMode(pinBoutonB, INPUT);
}

void loop() {
    if (d()) {
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
    }
    switch(menu) {
      case 1: enigme_1(); break;
      case 2: enigme_2(); break;
      case 3: enigme_3(); break;
      case 4: enigme_4(); break;
      default: accueil();
    }
}

void accueil() {
    if (d() && edite == 0) {
        Xzone = int(Xzone+deltaPose1+128)%128;
        Yzone = int(Yzone+deltaPose2+64)%64;
    }
    LCD.CleanAll(WHITE);
      
    LCD.DrawRectangleAt(2, 2, 28, 28, BLACK_NO_FILL);
    if (interrupteurs[0]) {
        LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_NO_BAC);
        LCD.DispStringAt("BTS", 4, 8);
    } else if (Xzone == 1 && Yzone == 1)
        interrupteurs[0] = true;
    
    LCD.DrawRectangleAt(34, 2, 60, 28, BLACK_NO_FILL);
    if (boutonA) LCD.DispStringAt(Xzone+" "+Yzone, 36, 8);
    else {
      LCD.FontModeConf(Font_6x12, FM_ANL_AAA, BLACK_NO_BAC);
      LCD.DispStringAt("Enigme "+edite, 37, 9);
    }
    
    LCD.DrawRectangleAt(98, 2, 28, 28, BLACK_NO_FILL);
    if (interrupteurs[1]) {
        LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC);
        LCD.DispStringAt("SN", 100, 6);
    } else if (Xzone == 97 && Yzone == 1)
        interrupteurs[1] = true;
    
    
    if (interrupteurs[2]) {
        LCD.DrawRectangleAt(31, 62, 2, 2, BLACK_FILL);
    } else if (Xzone == 17 && Yzone == 49)
        interrupteurs[2] = true;
        
    if (interrupteurs[3]) {
        LCD.DrawRectangleAt(63, 62, 2, 2, BLACK_FILL);
    } else if (Xzone == 49 && Yzone == 49)
        interrupteurs[3] = true;
        
    if (interrupteurs[4]) {
        LCD.DrawRectangleAt(95, 62, 2, 2, BLACK_FILL);
    } else if (Xzone == 81 && Yzone == 49)
        interrupteurs[4] = true;
    
    LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_NO_BAC);
    enigme1.draw();
    enigme2.draw();
    enigme3.draw();
    enigme4.draw();
    
    enigme1.update();
    enigme2.update();
    enigme3.update();
    enigme4.update();
    
    LCD.DrawRectangleAt(Xzone, Yzone, 30, 30, BLACK_NO_FILL);
    LCD.DrawRectangleAt(Xzone+2, Yzone+2, 26, 26, BLACK_NO_FILL);
    if (Xzone > W-30) {
      LCD.DrawRectangleAt(Xzone-W, Yzone, 30, 30, BLACK_NO_FILL);
      LCD.DrawRectangleAt(Xzone+2-W, Yzone+2, 26, 26, BLACK_NO_FILL);
    }
    if (Yzone > H-30) {
      LCD.DrawRectangleAt(Xzone, Yzone-H, 30, 30, BLACK_NO_FILL);
      LCD.DrawRectangleAt(Xzone+2, Yzone+2-H, 26, 26, BLACK_NO_FILL);
    }
    if (Xzone > W-30 && Yzone > H-30) {
      LCD.DrawRectangleAt(Xzone-W, Yzone-H, 30, 30, BLACK_NO_FILL);
      LCD.DrawRectangleAt(Xzone+2-W, Yzone+2-H, 26, 26, BLACK_NO_FILL);
    }
    sleep(50);
}


void enigme_1() {
    if (digitalRead(pinBoutonA)) menu = -1;
    LCD.CleanAll(WHITE);
    LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC);
    LCD.DispStringAt(millis(), 0, 0);
    LCD.DispStringAt("Enigme 1:", 4, 40);
    sleep(50);
}


void enigme_2() {
    if (digitalRead(pinBoutonA)) menu = -1;
    LCD.CleanAll(WHITE);
    LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC);
    LCD.DispStringAt(millis(), 0, 0);
    LCD.DispStringAt("Enigme 2:", 4, 40);
    sleep(50);
}
/*
int [][] tab_carte = {{197,  19, 112, 179},
                      {253, 197,  59,  19},
                      {247,  43, 179, 187},
                      {109, 109, 187, 247},
                      { -1, 253,  59, 112}};/*/
int [][] tab_carte = {{42, 42, 42, 42},
                      {42, 42, 42, 42},
                      {42, 43, 42, 42},
                      {42, 42, 42, 42},
                      {-1, 42, 42, 42}};// */
int carte = 0,
    carteSelect = -1,
    nbPaire = 9;

void enigme_3() {
    if (d()) {
      int delta = 0;
      if (deltaPose1 > 0) delta = 1;
      else if (deltaPose1 < 0) delta = -1;
      if (deltaPose2 > 0) delta = 5;
      else if (deltaPose2 < 0) delta = -5;
      
      do {
        if (delta == 0) delta = 5;
        else if (carte == 19 && delta > 0) carte = 0;
        else if (carte == 0  && delta < 0) carte = 19;
        else carte += delta;
        
        if (carte <  0) carte += 19;
        if (carte > 19) carte -= 19;
      } while(tab_carte[carte%5][carte/5] < 0 || (carte == carteSelect && nbPaire != 0));
      
      LCD.CursorGotoXY(1+(carte%5)*26, 2+(carte/5)*16, 22, 12);
      
      if (!cursorOn) {
        LCD.CursorConf(ON, 2);
        cursorOn = true;
      }
    }
    
    LCD.CleanAll(WHITE);
    LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_NO_BAC);
    LCD.DispStringAt(nbPaire, 103, 0);
    LCD.DrawRectangleAt(114, 6, 10, 6, BLACK_NO_FILL);
    LCD.DrawRectangleAt(116, 4, 10, 6, BLACK_NO_FILL);
    LCD.DrawRectangleAt(117, 5, 8, 4, WHITE_FILL);
    LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_NO_BAC);
    for (int i=0; i<5; i++)
      for (int j=0; j<4; j++)
        draw_carte(i, j, false);
    
    if (!oldBoutonB && boutonB) {
      if (carteSelect == -1) { if (d()) carteSelect = carte;
      } else if (nbPaire != 0) {
        if (d()) {
          LCD.CursorConf(OFF, 2);
          cursorOn = false;
        }
        draw_carte(carteSelect%5, carteSelect/5, true);
        draw_carte(carte%5, carte/5, true);
        sleep(2000);
        if (d() && tab_carte[carteSelect%5][carteSelect/5] == tab_carte[carte%5][carte/5]) {
          tab_carte[carteSelect%5][carteSelect/5] = -1;
          tab_carte[carte%5][carte/5] = -1;
          nbPaire --;
        }
        draw_carte(carteSelect%5, carteSelect/5, true);
        draw_carte(carte%5, carte/5, true);
        if (d()) {
          carteSelect = -1;
          LCD.CursorConf(ON, 2);
          cursorOn = true;
        }
      }
    }
    
    if (digitalRead(pinBoutonA)) {
      LCD.CursorConf(OFF, 2);
      cursorOn = false;
      menu = -1;
    }
}

  
void draw_carte(int x, int y, boolean showNB) {
    int l = x*26,
        h = y*16;
    if (tab_carte[x][y] > 0) {
        LCD.DrawHLineAt(1+l, 1+h, 21, BLACK);
        LCD.DrawVLineAt(23+l, 2+h, 11, BLACK);
        LCD.DrawHLineAt(1+l, 14+h, 21, BLACK);
        LCD.DrawVLineAt(0+l, 2+h, 11, BLACK);
        LCD.DrawRectangleAt(1+l, 2+h, 22, 12, WHITE_FILL);
        if (showNB || carteSelect == x+y*5) {
          LCD.DispStringAt(tab_carte[x][y], 3+l, 4+h);
        } else {
          LCD.DrawLineAt(1+l, 2+h, 22+l, 13+h, BLACK);
          LCD.DrawLineAt(22+l, 2+h, 1+l, 13+h, BLACK);
          LCD.DrawLineAt(8+l, 2+h, 15+l, 13+h, BLACK);
          LCD.DrawLineAt(15+l, 2+h, 8+l, 13+h, BLACK);
          LCD.DrawRectangleAt(7+l, 5+h, 10, 6, BLACK_NO_FILL);
          LCD.DrawRectangleAt(10+l, 6+h, 4, 4, WHITE_NO_FILL);
        }
    } else if (showNB)
      LCD.DrawRectangleAt(l, 1+h, 24, 14, WHITE_FILL);
}


void enigme_4() {
    if (digitalRead(pinBoutonA)) menu = -1;
    LCD.CleanAll(WHITE);
    LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC);
    LCD.DispStringAt(millis(), 0, 0);
    LCD.DispStringAt("Enigme 4:", 4, 40);
    sleep(50);
}