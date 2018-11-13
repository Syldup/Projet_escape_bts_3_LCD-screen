int pinBoutonB;
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    pinBoutonB=10;
    pinMode(pinBoutonB,INPUT);
}

void loop() {

    int barre = 0;
   
    int nbAlea = 0;

  do {  
      if(digitalRead(pinBoutonB) == HIGH)
      {
        nbAlea=random(2);
        if(nbAlea == 0)
        {
          if(barre != 0)
          {
            barre-=1;
          }
        }

        else
        {
          if(barre != 99)
          {
            barre+=1;
          }
        }
        while(digitalRead(pinBoutonB)!= LOW)
        {
          
        }
      }  
  }while(barre != 99);

}
