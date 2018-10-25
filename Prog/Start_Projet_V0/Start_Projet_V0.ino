#include <Encoder.h>
#include <I2C_LCD.h>

Encoder enc1(2, 3);
Encoder enc2(4, 5);
int pinBoutonA = 6;
int pinBoutonB = 8;
I2C_LCD LCD;
uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.


int W = 128, H = 64;
int nb = 0;
int menu = 0,
    Xzone = 113,
    Yzone = 49,
    edite = 0;
boolean boutonA = false,
        boutonB = false,
        oldBoutonA = false,
        oldBoutonB = false;
long pose1 = 0,
     pose2 = 0,
     deltaPose1 = 0,
     deltaPose2 = 0;
boolean interrupteurs[5] = {false, false, false, false, false};


class Enigme {
  public:
    boolean trouver = false;
    int nb_enigme,
        cible,
        menu_x,
        menu_y;
    char nb[3] = {'0', '0', '0'};
    
    Enigme(int nb, int c, int x, int y) {
      nb_enigme = nb;
      cible = c;
      menu_x = x;
      menu_y = y;
    }
    
    void draw() {
        if (!(Xzone == menu_x+1 && Yzone == menu_y+1) || (edite==0 && (millis()/1000)%2 == 0))
            LCD.DrawRectangleAt(menu_x+2, menu_y+2, 28, 28, BLACK_NO_FILL);
        
        if (trouver)
          LCD.DispStringAt(nb, menu_x+4, menu_y+8);
    }
    
    void update() {
        if (menu == 0)
          if (Xzone == menu_x+1 && Yzone == menu_y+1) {
            if (trouver) {
              if (edite != 0) {
                if (abs(deltaPose1) > 3) {
                  if (deltaPose1 > 0) edite ++;
                  if (deltaPose1 < 0) edite --;
                  edite = (edite+2)%3+1;
                  LCD.CursorGotoXY(menu_x-4+edite*8, menu_y+8, 8, 16);
                }
                if (abs(deltaPose2) > 3) {
                  if (deltaPose2 > 0) nb[edite-1] ++;
                  if (deltaPose2 < 0) nb[edite-1] --;
                  nb[edite-1] = (nb[edite-1]-'0'+10)%10+'0';
                }
              } else
                if (oldBoutonA && !boutonA)
                  menu = nb_enigme;
              
              if (oldBoutonB && !boutonB) {
                  LCD.CursorGotoXY(menu_x+4, menu_y+8, 8, 16);
                  if (edite == 0) {
                      LCD.CursorConf(ON, 2);
                      edite = 1;
                  } else {
                      LCD.CursorConf(OFF, 2);
                      edite = 0;
                  }
              }
            } else trouver = true;
          }
    }
};

Enigme enigme1(1, 192, 0, 32);
Enigme enigme2(2, 1, 32, 32);
Enigme enigme3(3, 1, 64, 32);
Enigme enigme4(4, 1, 96, 32);

void setup() {
    LCD.Init();
    //Wire.setClock(10000);
    Serial.begin(9600);
    Serial.println("Basic Encoder Test:");
    pinMode(pinBoutonA, INPUT);
    pinMode(pinBoutonB, INPUT);
}

int w = 128, h = 64;
long oldPosition  = -999;

void loop() {
    nb ++;
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
    
    switch(menu) {
      case 1: enigme_1(); break;
      case 2: enigme_2(); break;
      case 3: enigme_3(); break;
      case 4: enigme_4(); break;
      default: accueil();
    }
}

void accueil() {
    if (edite == 0) {
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
    delay(50);
}


void enigme_1() {
    if (digitalRead(pinBoutonA)) menu = -1;
    Serial.println(digitalRead(pinBoutonA), digitalRead(pinBoutonB));
    LCD.CleanAll(WHITE);
    LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC);
    LCD.DispStringAt(nb, 0, 0);
    LCD.DispStringAt("Enigme 1:", 4, 40);

    LCD.DrawRectangleAt(10, 15, 107, 33, BLACK_FILL);
    LCD.DrawDotAt(127, 63, BLACK);
    LCD.DrawDotAt(120, 50, BLACK);
    
    delay(500);
}


void enigme_2() {
    if (digitalRead(pinBoutonA)) menu = -1;
    LCD.CleanAll(WHITE);
    LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC);
    LCD.DispStringAt(nb, 0, 0);
    LCD.DispStringAt("Enigme 2:", 4, 40);
    delay(500);
}


void enigme_3() {
    if (digitalRead(pinBoutonA)) menu = -1;
    LCD.CleanAll(WHITE);
    LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC);
    LCD.DispStringAt(nb, 0, 0);
    LCD.DispStringAt("Enigme 3:", 4, 40);
    delay(500);
}


void enigme_4() {
    if (digitalRead(pinBoutonA)) menu = -1;
    LCD.CleanAll(WHITE);
    LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC);
    LCD.DispStringAt(nb, 0, 0);
    LCD.DispStringAt("Enigme 4:", 4, 40);
    delay(500);
}
