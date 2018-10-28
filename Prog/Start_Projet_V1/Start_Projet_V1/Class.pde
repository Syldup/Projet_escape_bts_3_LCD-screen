int INPUT = 0;

int Font_6x8 = 0,
    Font_6x12 = 1,
    Font_8x16_1 = 2,
    Font_8x16_2 = 3,
    Font_10x20 = 4,
    Font_12x24 = 5,
    Font_16x32 = 6;

int FM_ANL_AAA = 0,
    FM_MNL_AAA = 1,
    FM_MNL_MAA = 2;

int WHITE_BAC = 255,
    BLACK_BAC = 1,
    WHITE_NO_BAC = 254,
    BLACK_NO_BAC = 0;

int WHITE = 255,
    BLACK = 1;


int WHITE_FILL = 255,
    BLACK_FILL = 1,
    WHITE_NO_FILL = 254,
    BLACK_NO_FILL = 0;

boolean AllREV = true,
        AllNOR = false;
        
boolean ON = true,
        OFF = false;

void draw() {
    delay(delay);
    delay = 0;
    sleepPass = 0;
    loop();
    if(delay == 0) {
      sleep(0);
      oldSleepPass = 0;
    }
}

class I2C_LCD {
    int Xfont, Yfont, Cfont;
    boolean Bfont, cursor;
    int frequence, Xcursor, Ycursor, Wcursor, Hcursor;
    
    I2C_LCD() {
      Xfont = 6;
      Yfont = 8;
      Cfont = BLACK;
      cursor = false;
      frequence = 0;
    }
    
    void FontModeConf(int font, int fm, int c) {
      switch(font) {
        case 0: // Font_6x8
          Xfont = 6;
          Yfont = 8;
          break;
        case 1: // Font_6x12
          Xfont = 6;
          Yfont = 12;
          break;
        case 2: // Font_8x16_1
        case 3: // Font_8x16_2
          Xfont = 8;
          Yfont = 16;
          break;
        case 4: // Font_10x20
          Xfont = 10;
          Yfont = 20;
          break;
        case 5: // Font_12x24
          Xfont = 12;
          Yfont = 24;
          break;
        case 6: // Font_16x32
          Xfont = 16;
          Yfont = 32;
          break;
        default:
          Xfont = fm;
          Yfont = fm;
      }
      Cfont = c;
      Bfont = c%2 == 0;
    }
    
    void DrawDotAt(int x, int y, int c) {
      fill(c);
      point(x, y);
    }
    
    void DrawLineAt(int x, int y, int x2, int y2, int c) {
      stroke(c);
      line(x, y, x2, y2);
    }
    void DrawHLineAt(int x, int y, int l, int c) {
      stroke(c);
      line(x, y, x+l, y);
    }
    void DrawVLineAt(int x, int y, int l, int c) {
      stroke(c);
      line(x, y, x, y+l);
    }
    
    void DrawCircleAt(int x, int y, int r, int c) {
      stroke(c);
      if(c%2 == 1) fill(c);
      else noFill();
      ellipse(x-r/2, y-r/2, r, r);
    }
    
    void DrawRectangleAt(int x, int y, int w, int h, int c) {
      stroke(c);
      if(c%2 == 1) fill(c);
      else noFill();
      rect(x, y, w-1, h-1);
    }
    
    void CleanAll(int c) {
      background(128);
      DrawRectangleAt(0, 0, W, H, c);
    }
    
    void DispStringAt(String txt, int x, int y) {
      if(Bfont)
        DrawRectangleAt(x, y, Xfont*txt.length(), Yfont, 255-Cfont);
      for(int i=0; i<txt.length(); i++)
        DispCharAt(txt.charAt(i), x+Xfont*i, y);
    }
    void DispStringAt(long nb, int x, int y) {
      DispStringAt(""+nb, x, y);
    }
    void DispStringAt(int nb, int x, int y) {
      DispStringAt(""+nb, x, y);
    }
    void DispCharAt(char c, int x, int y) {
        textSize(Yfont);
        fill(Cfont);
        text(c, x, y+(Yfont-textAscent()-textDescent())/2);
    }
    
    void DisplayConf(boolean conf) {
      DisplayConf = conf;
    }
    
    void CursorConf(boolean on, int freq) {
      cursor = on;
      frequence = freq;
    }
    void CursorGotoXY(int x, int y, int w, int h) {
      Xcursor = x;
      Ycursor = y;
      Wcursor = w;
      Hcursor = h;
    }
    boolean inCursor(int x, int y) {
      return cursor && (x >= Xcursor && y >= Ycursor && x < Xcursor+Wcursor && y < Ycursor+Hcursor)
             && (frequence == 0 || millis()%int(2000*(1.0/frequence)) > int(1000*(1.0/frequence)));
    }
}

void keyPressed() {
    enc1.keyPressed();
    enc2.keyPressed();
    for(_bouton b : _boutons)
      if (b != null)
        b.keyPressed();
}
void keyReleased() {
    for(_bouton b : _boutons)
      if (b != null)
        b.keyReleased();
}

class Encoder {
  char keyP,
       keyM;
  long val;
  
  Encoder(char p, char m) {
    keyP = p;
    keyM = m;
    val = 0;
  }
  
  void keyPressed() {
    if (key == keyP) val += 4;
    if (key == keyM) val -= 4;
    if (key == Character.toUpperCase(keyP)) val ++;
    if (key == Character.toUpperCase(keyM)) val --;
  }
  
  long read() {
    return val;
  }
}
boolean DisplayConf = false;

int delay = 0,
    oldSleepPass = 0,
    sleepPass = 0;
    
void sleep(int time) {
    if(delay == 0) {
      noStroke();
      for(int i=0; i<W; i++)
        for(int j=0; j<H; j++) {
          if(DisplayConf ^ LCD.inCursor(i, j)) fill(255-red(get(i, j)));
          else fill(get(i, j));
          rect(129+i*8, 1+j*8, 7, 7);
        }
      if(sleepPass == oldSleepPass) {
        oldSleepPass ++;
        delay = time;
      } else sleepPass ++;
    }
}

boolean d() {
    return sleepPass == oldSleepPass;
}

class _bouton {
    boolean on;
    char pin;
    _bouton(char c) {
      on = false;
      pin = c;
    }
    void keyPressed() {
      if (key == pin)
        on = true;
    }
    void keyReleased() {
      if (key == pin)
        on = false;
    }
}

_bouton[] _boutons = new _bouton[15];
void pinMode(char c, int i) {
    for(i = 0; i<_boutons.length; i++)
      if (_boutons[i] == null) {
        _boutons[i] =  new _bouton(c);
        break;
      }
    print(i);
}
boolean digitalRead(char c) {
    for(_bouton b : _boutons)
      if (b != null && b.pin == c)
        return b.on;
    return false;
}