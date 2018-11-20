#include <Encoder.h>
#include <I2C_LCD.h>

I2C_LCD LCD;
extern GUI_Bitmap_t bmCarte;
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
        loaded = false;
long poseEnc1 = 0,
     deltaPose = 0;

boolean interrupteurs[9] = {false};
String addressIP[4] = {"10", "16", "42", "1"};
String tabEnigme[4] = {"000", "000", "000", "000"};

void setup() {
  LCD.Init();
  Serial.begin(9600);
  Serial.println("Basic  Test:");
  LCD.CleanAll(WHITE);
  LCD.DisplayConf(AllNOR);
  LCD.CursorConf(OFF, 6);
}

void updateInput() {
    delay(1);
    oldBoutonA = boutonA;
    oldBoutonB = boutonB;
    boutonA = digitalRead(pinBoutonA);
    boutonB = digitalRead(pinBoutonB);
    
    poseEnc1 = enc1.read();
    
    deltaPose = poseEnc1/4;
    if (deltaPose != 0)
        enc1.write(0);
}

void loop() {
    updateInput();
    if (!loaded && !boutonA) {
      oldBoutonA = false;
      loaded = true;
      updateAcceil();
    }
    
    if (!oldBoutonB && boutonB) {
      for (int i=0; i<4; i++)
        if (Xzone == i*32+1 && Yzone == 32+1 && tabEnigme[i].length() != 0) {
          edite ++;
          if (edite > 3) {
              LCD.CursorConf(OFF, 8);
              cursorOn = false;
              edite = 0;
              if (addressIP[i].toInt() == tabEnigme[i].toInt())
                tabEnigme[i] = "";
          } else {
            LCD.CursorGotoXY(i*32-4+edite*8, 32+8, 8, 16);
            if (!cursorOn) {
                LCD.CursorConf(ON, 8);
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
              if (Xzone == i*32+1 && Yzone == 32+1 && tabEnigme[i].length() != 0)
                nextMenu = i+1;
            
            allNor = !allNor || nextMenu != 0;
            if (allNor) LCD.DisplayConf(AllNOR);
            else  LCD.DisplayConf(AllREV);

            loaded = false;
            switch(nextMenu) {
              case 1: enigme_1(); break;
              case 2: enigme_2(); break;
              case 3: enigme_3(); break;
              case 4: enigme_4(); break;
              default: loaded = true;
            }
        }
        if (deltaPose != 0) {
            if (allNor) Xzone = int(Xzone+deltaPose*4+128)%128;
            else Yzone = int(Yzone+deltaPose*4+64)%64;
            updateAcceil();
        }
    } else if (deltaPose != 0) {
        for (int i=0; i<4; i++)
          if (Xzone == i*32+1 && Yzone == 32+1) {
            tabEnigme[i][edite-1] = char((tabEnigme[i][edite-1]+deltaPose-'0'+10)%10+'0');
            LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_BAC);
            LCD.DispStringAt(tabEnigme[i].c_str(), i*32+4, 32+8);
          }
    }
    if (Yzone == 32+1)
      for (int i=0; i<4; i++)
        if (Xzone == i*32+1 && tabEnigme[i].length() != 0)
          if ((millis()/1000)%2 == 0)
               LCD.DrawRectangleAt(i*32+2, 32+2, 28, 28, BLACK_NO_FILL);
          else LCD.DrawRectangleAt(i*32+2, 32+2, 28, 28, WHITE_NO_FILL);
}

void updateAcceil() {
    LCD.CleanAll(WHITE);
    
    LCD.DrawRectangleAt(34, 2, 60, 28, BLACK_NO_FILL);
    LCD.FontModeConf(Font_6x12, FM_ANL_AAA, BLACK_NO_BAC);
    char tmp[25];
    sprintf(tmp, "Enigme %d", edite);
    LCD.DispStringAt(tmp, 37, 9);
      
    LCD.DrawRectangleAt(2, 2, 28, 28, BLACK_NO_FILL);
    if (Xzone == 1 && Yzone == 1)
        interrupteurs[0] = true;
    if (interrupteurs[0]) {
        LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_NO_BAC);
        LCD.DispStringAt("BTS", 4, 8);
    }
    
    LCD.DrawRectangleAt(98, 2, 28, 28, BLACK_NO_FILL);
    if (Xzone == 97 && Yzone == 1)
        interrupteurs[1] = true;
    if (interrupteurs[1]) {
        LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_NO_BAC);
        LCD.DispStringAt("SN", 102, 6);
    }
    
    for (int i=0; i<3; i++) {
      if (Xzone == 17+i*32 && Yzone == 49)
          interrupteurs[2+i] = true;
      if (interrupteurs[2+i])
        LCD.DrawRectangleAt(31+i*32, 62, 2, 2, BLACK_FILL);
    }
          
    LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_NO_BAC);
    for (int i=0; i<4; i++) {
        int x = i*32, y = 32;
        //LCD.DrawRectangleAt(x, y, 30, 30, WHITE_FILL);
        if (tabEnigme[i].length() != 0) {
            if (Xzone == x+1 && Yzone == y+1) interrupteurs[5+i] = true;
            if (interrupteurs[5+i])
              LCD.DispStringAt(tabEnigme[i].c_str(), x+4, y+8);
            if (!(Xzone == x+1 && Yzone == y+1 && edite == 0)) 
              LCD.DrawRectangleAt(x+2, y+2, 28, 28, BLACK_NO_FILL);
        } else LCD.DispStringAt(addressIP[i].c_str(), x+4, y+8);
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
    LCD.CleanAll(WHITE);
    delay(1000);
}

void enigme_2() {
    int i = 0;
    int etat = 0;
    double barre = 0;
    
    LCD.CleanAll(WHITE);
    do {
        if (barre < 128) {
            updateInput();
            
            if(boutonB && !oldBoutonB)
                barre += 1;
        
            if(barre > 0)
            {
               barre -= 0.03;
               if(barre < 0)
                   barre = 0;
            }
            Serial.println(barre);
        } else {
            Serial.println("GAGNER");
        }
    } while (!(boutonA && !oldBoutonA));
}

/*
int tab_carte [5][4] = {{197,  19, 112, 179},
                       {253, 197,  59,  19},
                       {247,  42, 179, 187},
                       {109, 109, 187, 247},
                       { -1, 253,  59, 112}};/*/
int tab_carte [5][4] = {{43, 43, 43, 43},
                       {43, 43, 43, 43},
                       {43, 42, 43, 43},
                       {43, 43, 43, 43},
                       {-1, 43, 43, 43}};// */
int nbPaire = 9;

void enigme_3() {
    int carte = 0,
        oldCarte = -1,
        carteSelect = -1;
    LCD.CleanAll(WHITE);
    LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_NO_BAC);
    for (int i=0; i<5; i++)
      for (int j=3; j>=0; j--)
        if (tab_carte[i][j] >= 0) {
          draw_carte(i, j, false);
          delay(250);
        }
    LCD.DrawRectangleAt(114, 6, 10, 6, BLACK_NO_FILL);
    LCD.DrawRectangleAt(116, 4, 10, 6, BLACK_NO_FILL);
    LCD.DrawRectangleAt(117, 5, 8, 4, WHITE_FILL);
    LCD.FontModeConf(Font_8x16_2, FM_ANL_AAA, BLACK_BAC);
    char tmp[4];
    sprintf(tmp, "%d", nbPaire);
    LCD.DispStringAt(tmp, 104, 1);

    do {
        updateInput();
        oldCarte = carte;
        
        do {
            if (deltaPose == 0) deltaPose = 1;
            else carte = (carte+deltaPose+20)%20;
        } while(tab_carte[carte%5][carte/5] < 0 || (carte == carteSelect && nbPaire != 0));
        
        if (oldCarte != carte || !loaded) {
          LCD.CursorGotoXY(1+(carte%5)*26, 2+(carte/5)*16, 22, 12);
          draw_carte(oldCarte%5, oldCarte/5, oldCarte == carteSelect);
        }
        if (!cursorOn) {
          LCD.CursorConf(ON, 6);
          cursorOn = true;
        }
                
        if (!oldBoutonB && boutonB) {
            if (carteSelect == -1) {
              carteSelect = carte;
              draw_carte(carteSelect%5, carteSelect/5, true);
            } else if (nbPaire != 0) {
              LCD.CursorConf(OFF, 6);
              cursorOn = false;
              
              draw_carte(carteSelect%5, carteSelect/5, true);
              draw_carte(carte%5, carte/5, true);
              delay(200);
              if (tab_carte[carteSelect%5][carteSelect/5] == tab_carte[carte%5][carte/5]) {
                tab_carte[carteSelect%5][carteSelect/5] = -1;
                tab_carte[carte%5][carte/5] = -1;
                nbPaire --;
                LCD.FontModeConf(Font_8x16_2, FM_ANL_AAA, BLACK_BAC);
                char tmp[4];
                sprintf(tmp, "%d", nbPaire);
                LCD.DispStringAt(tmp, 104, 1);
              }
              draw_carte(carteSelect%5, carteSelect/5, false);
              draw_carte(carte%5, carte/5, false);
              carteSelect = -1;
              LCD.CursorConf(ON, 6);
              cursorOn = true;
            }
        }
      
        while (!loaded) {
          updateInput();
          loaded = !boutonA;
        }
    } while (!(boutonA && !oldBoutonA));
    loaded = false;
    LCD.CursorConf(OFF, 6);
    cursorOn = false;
    enc1.write(0);
    deltaPose = 0;
}

void draw_carte(int x, int y, boolean showNB) {
    int i = x*26,
        j = y*16+1;
    if (tab_carte[x][y] > 0) {
        LCD.WorkingModeConf(ON, ON, WM_BitmapMode);
        LCD.DrawScreenAreaAt(&bmCarte, i, j);
        LCD.WorkingModeConf(ON, ON, WM_CharMode);
        if (showNB) {
          LCD.DrawRectangleAt(i+1, j+1, 22, 12, WHITE_FILL);
          LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_NO_BAC);
          char tmp[4];
          sprintf(tmp, "%d", tab_carte[x][y]);
          LCD.DispStringAt(tmp, i+3, j+3);
        }
    } else LCD.DrawRectangleAt(i, j, 24, 14, WHITE_FILL);
}

void enigme_4() {
    LCD.CleanAll(WHITE);
    delay(1000);
}
