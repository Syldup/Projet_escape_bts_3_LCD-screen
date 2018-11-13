
int pinBoutonA = 6;
int pinBoutonB = 8;
int barreEnigme2 = 0;
int nombreEnigme2 = 0;
boolean resultatEnigme2;
boolean boutonB = false,
     oldBoutonB = false;
int i = 0;
int etat = 0;
double barre = 0;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("Basic Test:");
    pinMode(pinBoutonA, INPUT);
    pinMode(pinBoutonB, INPUT);
}

void loop() {

    do {

    oldBoutonB = boutonB;
    boutonB = digitalRead(pinBoutonB);
    
    if(boutonB && oldBoutonB == false)
    {
      barre+=1;
    }

      if(barre > 0)
    {
     barre-=0.03;

     if(barre < 0)
      {
        barre = 0;
      }
    }      
    Serial.println(barre);  

    }while(barre != 128);

    Serial.println("GAGNER");
}
