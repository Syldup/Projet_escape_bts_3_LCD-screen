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
    
    
    if (poseEnc1 >= 4) deltaPose = 1;
    else if (poseEnc1 <= -4) deltaPose = -1;
    else deltaPose = 0;
    
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
    String nbCarre = "00";
    int idx = 0;
    LCD.CleanAll(WHITE);
    LCD.FontModeConf(Font_8x16_2, FM_ANL_AAA, BLACK_BAC);
    LCD.CursorGotoXY(93, 35, 8, 16);
    LCD.DispStringAt(nbCarre.c_str() ,93 ,35 );
    LCD.CursorConf(ON, 8);
    cursorOn = true;
    do {
        updateInput();
        LCD.DrawRectangleAt(0, 0, 128, 64, BLACK_NO_FILL);
        LCD.DrawRectangleAt(0, 0, 90, 64, BLACK_NO_FILL);
        LCD.DispStringAt("cb ?",93 ,10 );
        if (deltaPose != 0) {
            nbCarre[idx] = char((nbCarre[idx]+deltaPose-'0'+10)%10+'0');
            LCD.DispStringAt(nbCarre.c_str() ,93 ,35 );
        }
        if(!oldBoutonB && boutonB) {
            idx = 1-idx;
            LCD.CursorGotoXY(93+8*idx, 35, 8, 16);
            LCD.DispStringAt(nbCarre.c_str(), 93, 35 );
        }
        if(nbCarre == "24")
            LCD.DrawRectangleAt(0, 0, 50, 34, BLACK_NO_FILL);
    } while (!(boutonA && !oldBoutonA));
    LCD.CursorConf(OFF, 8);
    cursorOn = false;
    loaded = false;
}


void enigme_2() {
    int i = 0;
    int etat = 0;
    double wBarre = 1,
           hBarre = 0;
    int oldW = 0,
        oldH = 0;
    int timer = 0;
    
    LCD.FontModeConf(Font_16x32, FM_ANL_AAA, WHITE_NO_BAC);
    LCD.CleanAll(WHITE);
    do {
        timer ++;
        updateInput();
        if(boutonB && !oldBoutonB) wBarre += 10;
        if(deltaPose != 0) hBarre += abs(deltaPose)*0.5;
        
        if(timer == 10) {     
          wBarre -= 1;
          hBarre -= 1;
          timer = 0;
        }
        
        if(wBarre < 0) wBarre = 0;
        if(wBarre >= 127) {
          boutonA = true;
          oldBoutonA = false;
          tabEnigme[1] = "";
        }
        if(hBarre < 0) hBarre = 0;
        if(hBarre > 31) hBarre = 31;
        
        if (oldH != int(hBarre) || oldW != int(wBarre)) {
            if(oldH > int(hBarre))
              LCD.DrawRectangleAt(0, 32-hBarre, 1+wBarre+2, 3+hBarre*2, WHITE_FILL);
            if(oldH > int(hBarre) || oldH > int(hBarre))
              LCD.DrawRectangleAt(0, 31-hBarre, 1+wBarre, 1+hBarre*2, BLACK_FILL);
            LCD.DispStringAt("0", 4, 15);
            
            oldH = int(hBarre);
            oldW = int(wBarre);
        }
    } while (!(boutonA && !oldBoutonA));
    loaded = false;
}


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
              if (nbPaire == 0)
                tabEnigme[2] = "";
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
            } else {
              boutonA = true;
              oldBoutonA = false;
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


int nbSeq = 8,
    idxSeq = 0;

int listSeq[9] = {0, 11, 12, 21, 22, 23, 24, 25, -1};

// sData[] -> {start_x, start_y, len_sequence, pas}
// sequence[][0] -> Longueur des segments de la sequence
// sequence[][1] -> Orientation des segments de la sequence
int* sData;
int (*sequence)[2];

int s00Data[4] = {53, 30, 5, 5};
int s00[5][2] = {{1, 0}, {2, 3}, {2, 2}, {4, 1}, {3, 0}};

int s11Data[4] = {49, 22, 4, 4};
int s11[4][2] = {{4, 1}, {2, 0}, {4, 3}, {2, 2}};

int s12Data[4] = {45, 26, 4, 4};
int s12[4][2] = {{2, 1}, {4, 0}, {2, 3}, {3, 2}};

int s21Data[4] = {55, 41, 3, 3};
int s21[3][2] = {{1, 3}, {1, 2}, {2, 1}};

int s22Data[4] = {40, 44, 5, 3};
int s22[5][2] = {{9, 3}, {8, 0}, {2, 1}, {2, 2}, {2, 3}};

int s23Data[4] = {58, 29, 4, 3};
int s23[4][2] = {{5, 2}, {3, 3}, {2, 0}, {2, 1}};

int s24Data[4] = {37, 23, 9, 3};
int s24[9][2] = {{5, 1}, {1, 3}, {2, 0}, {1, 3}, {1, 0}, {6, 3}, {5, 0}, {4, 1}, {5, 2}};

int s31Data[4] = {55, 41, 3, 3};
int s31[3][2] = {{1, 3}, {1, 2}, {2, 1}};

void setSequence(int idx) {
    switch (idx) {
      case 1: sequence = s11; sData = s11Data; break;
      case 2: sequence = s12; sData = s12Data; break;
      case 3: sequence = s21; sData = s21Data; break;
      case 4: sequence = s22; sData = s22Data; break;
      case 5: sequence = s23; sData = s23Data; break;
      case 6:
      case 7: sequence = s24; sData = s24Data; break;
      default: sequence = s00; sData = s00Data; break;
    }
}

void drawSequence(int idx, bool anim) {
    if (anim) {
      bool animOld = listSeq[idx] == listSeq[idxSeq];
      LCD.DrawRectangleAt(82, 2, 44, 60, WHITE_FILL);
      switch (idx) {
        case 0: drawSequence(s00, s00Data, true, true); break;
        case 1: drawSequence(s11, s11Data, true, true); break;
        case 2:
          drawSequence(s11, s11Data, true, animOld);
          drawSequence(s12, s12Data, true, true);
          break;
        case 3: drawSequence(s21, s21Data, true, true); break;
        case 4:
          drawSequence(s21, s21Data, true, animOld);
          drawSequence(s22, s22Data, true, true);
          break;
        case 5:
          drawSequence(s21, s21Data, true, animOld);
          drawSequence(s22, s22Data, true, animOld);
          drawSequence(s23, s23Data, true, true);
          break;
        case 6:
          drawSequence(s21, s21Data, true, animOld);
          drawSequence(s22, s22Data, true, animOld);
          drawSequence(s23, s23Data, true, animOld);
          drawSequence(s24, s24Data, true, true);
          break;
        case 7:
          drawSequence(s21, s21Data, true, animOld);
          drawSequence(s22, s22Data, true, animOld);
          drawSequence(s23, s23Data, true, animOld);
          drawSequence(s24, s24Data, true, animOld);
          LCD.DrawCircleAt(55, 32, 19, BLACK_NO_FILL);
          LCD.DrawCircleAt(55+49, 32, 19, BLACK_NO_FILL);
          LCD.DispStringAt("3", 5, 1);
          tabEnigme[3] = "";
          delay(3000);
          idxSeq = -2;
          boutonA = true;
          oldBoutonA = false;
          break;
      }
    }
    LCD.DrawRectangleAt(33, 2, 44, 60, WHITE_FILL);
    switch (idx) {
      case 6: drawSequence(s23, s23Data, false, false);
      case 5: drawSequence(s22, s22Data, false, false);
      case 4: drawSequence(s21, s21Data, false, false); break;
      case 2: drawSequence(s11, s11Data, false, false); break;
    }
}

void enigme_4() {
    LCD.CleanAll(WHITE);
    delay(500);
    LCD.DrawCircleAt(15, 48, 2, BLACK_NO_FILL);
    delay(300);
    for (int i=-1; i<5; i++) {
      LCD.DrawVLineAt(48-4+abs(i), 48+4-abs(i), 15+8+i, BLACK);
      LCD.DrawHLineAt(15-4+abs(i), 15+4-abs(i), 48+8+i, BLACK);
      LCD.DrawVLineAt(48-4+abs(i), 48+4-abs(i), 15-8-i, BLACK);
      LCD.DrawHLineAt(15-4+abs(i), 15+4-abs(i), 48-8-i, BLACK);
      delay(200);
    }
    LCD.DrawRectangleAt(31, 0, 48, 64, BLACK_NO_FILL);
    LCD.DrawRectangleAt(80, 0, 48, 64, BLACK_NO_FILL);
    delay(300);
    for (int i=0; i<16; i++)
      LCD.DrawDotAt(7+i, 23-i, BLACK);
    LCD.FontModeConf(Font_8x16_2, FM_ANL_AAA, BLACK_BAC);
    LCD.DispStringAt("3", 16, 16);
    char tmp[3];
    sprintf(tmp, "%d", listSeq[idxSeq]/10);
    LCD.DispStringAt(tmp, 5, 1);
    
    setSequence(idxSeq);
    drawSequence(idxSeq, true);
    
    int x = sData[0], // Coordonner x sur l'écran
        y = sData[1], // Coordonner y sur l'écran
        idx = 0, // index du segment en coure
        d = 0, // Longueur du segment en coure
        a = 0; // Orientation de segment en coure
    
    LCD.CursorGotoXY(x, y, sData[3], sData[3]);
    LCD.CursorConf(ON, 8);
    cursorOn = true;
    
    do {
        updateInput();
        
        if (!oldBoutonB && boutonB) { // Avennce de 1 le segment
          LCD.CursorConf(OFF, 8);
          LCD.DrawRectangleAt(x, y, sData[3], sData[3], BLACK_FILL);
          x = (x + (1-a)%2*sData[3] + 128)%128;
          y = (y + (2-a)%2*sData[3] + 64)%64;
          
          d ++;
          if (d == sequence[idx][0] && a == sequence[idx][1]) { // Si fin du segment
            idx ++;
            d = 0;
            deltaPose += 4;
            if (idx >= sData[2]) {  // Si fin sequence
              idx = -1;
              char tmp[3];
              sprintf(tmp, "%d", listSeq[idxSeq+1]/10);
              LCD.DispStringAt(tmp, 5, 1);
              setSequence(idxSeq+1);
              drawSequence(idxSeq+1, true);
              idxSeq ++;
            }
          }
          // Reset affichage sequence
          if (idxSeq >= 0 && (idx == -1 || d != 0 && (d > sequence[idx][0] || a != sequence[idx][1]))) {
            x = sData[0];
            y = sData[1];
            idx = 0;
            d = 0;
            deltaPose += 4;
            drawSequence(idxSeq, false);
          }
          LCD.CursorConf(ON, 8);
          LCD.CursorGotoXY(x, y, sData[3], sData[3]);
        }
        
        if (!loaded || deltaPose != 0) {
          a = (a+deltaPose+4)%4;
          if (a == sequence[idx][1])
               LCD.DrawRectangleAt(14, 47, 3, 3, BLACK_FILL);
          else LCD.DrawRectangleAt(14, 47, 3, 3, WHITE_FILL);
          
          LCD.DrawDotAt(15+10, 48, WHITE);
          LCD.DrawDotAt(15, 48+10, WHITE);
          LCD.DrawDotAt(15-10, 48, WHITE);
          LCD.DrawDotAt(15, 48-10, WHITE);
          LCD.DrawVLineAt(47, 49, 15+9, WHITE);
          LCD.DrawHLineAt(14, 16, 48+9, WHITE);
          LCD.DrawVLineAt(47, 49, 15-9, WHITE);
          LCD.DrawHLineAt(14, 16, 48-9, WHITE);
          if (a == 0)
            LCD.DrawRectangleAt(14+10, 47, 2, 3, BLACK_NO_FILL);
          else if (a == 1)
            LCD.DrawRectangleAt(14, 47+10, 3, 2, BLACK_NO_FILL);
          else if (a == 2)
            LCD.DrawRectangleAt(15-10, 47, 2, 3, BLACK_NO_FILL);
          else if (a == 3)
            LCD.DrawRectangleAt(14, 48-10, 3, 2, BLACK_NO_FILL);
        }
        
        while (!loaded) {
          updateInput();
          loaded = !boutonA;
        }
        
    } while (!(boutonA && !oldBoutonA));
    deltaPose = 0;
    loaded = false;
    LCD.CursorConf(OFF, 8);
    cursorOn = false;
}

void drawSequence(int (*sequence)[2], int sData[4], bool translat, bool anime) {
  int x = sData[0],
      y = sData[1];
  if (translat) x += 49;
  for (int i=0; i<sData[2]; i++)
    for (int j=0; j<sequence[i][0]; j++) {
      if (anime) delay(200);
      LCD.DrawRectangleAt(x, y, sData[3], sData[3], BLACK_FILL);
      x = (x + (1-sequence[i][1])%2*sData[3] + 128)%128;
      y = (y + (2-sequence[i][1])%2*sData[3] + 64)%64;
    }
}
