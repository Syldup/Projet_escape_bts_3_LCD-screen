class Enigme {
    boolean trouver = false;
    int nb_enigme,
        cible,
        menu_x,
        menu_y;
    char [] nb = {'0', '0', '0'};
    
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
          LCD.DispStringAt(join(str(nb),""), menu_x+4, menu_y+8);
    }
    
    void update() {
        if (menu == 0 && d())
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
                  nb[edite-1] = char((nb[edite-1]-'0'+10)%10+'0');
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
}