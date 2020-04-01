#include "game.h"

#define SONIC_READ_PIN  7
#define LED_PIN1  13
#define LED_PIN2  11
#define LED_PIN3   9
#define BUTTON_PIN  4

bool sameButtonPress;

//Dit is het begin van het arduino programma -- vergelijkbaar met de app_main methode in esp-idf
//Hier wordt de Serial begin methode aangeroepen en gezet op een baud rate van 115200.
//Vervolgens worden de LED pinnnen geconfigureerd op output. En de button pin op input.
//Daarna begint het spel.
void setup() {
  Serial.begin(115200);
  
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  //De startgame methode wordt aangeroepen, deze komt uit game.h.
  startGame();
  //Er volgt een kleine delay om de timer wat tijd te geven.
  delay(5);              
  //En deze wordt vervolgens gereset om het eerlijk te houden voor de gebruiker.
  resetTimer();
}

//Dit is de loop methode voor het arduino programma -- vergelijkbaar met een while(true) loop die altijd aan staat.
//In de loop wordt gekeken of het spel is afgelopen, als dat niet zo is wordt er verder gecheckt op input van de gebruiker.
//Ook word hier de button en de ultrasoon sensor uitgelezen. 
void loop() {

  //Check of de game is afgelopen, zo niet, check de game loop. 
  if(!isGameCompleted()) {
    //De game loop is de main logica van het spel, deze methode is te vinden in game.h
    gameLoop();

    //Als de gameloop bepaalt dat er een ronde is afgelopen.
    if(isRoundCompleted()) {
        //Laad de leds dan blinken om de gebruiker te laten weten dat een deel van de code is geraden.
        blinkLedsOnComplete();
        //Roep de continue game methode in game.h aan om verder te gaan met het spel.
        continueGame();
    }

    //maakt hier twee long waarden aan om de ultrasoon sensor uit te lezen, en vervolgens om te zetten naar cm afstand.
    long duration, cm;

    //Dit is de logica van de ultrasoon sensor. Deze wordt eerst op output gezet om een signaal te sturen.
    //Daarna weer op input om zijn eigen signaal uit te lezen.
    pinMode(SONIC_READ_PIN, OUTPUT);
    digitalWrite(SONIC_READ_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(SONIC_READ_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(SONIC_READ_PIN, LOW);
    pinMode(SONIC_READ_PIN, INPUT);
    
    //De tijd die tussen de pulsen zit wordt uitgelezen. 
    duration = pulseIn(SONIC_READ_PIN, HIGH);
    //Deze tijd wordt hier omgezet naar een afstand in cm. Deze methode is te vinden in UltrasonicHandler.h
    cm = microsecondsToCentimeters(duration);

    //Hierna wordt gekeken of de gebruiker de afstand juist inschat. Deze methode is te vindein in game.h
    checkIfUserIsHoldingStill(cm);
    //De leds gaan aan om de gebruiker te laten zien of hij goed zit, of dat hij nog iets ver of te dichtbij de ultrasoon sensor is.
    //Deze methode is hieronder te vinden.
    turnOnLedsToGuessCode(cm, getCurrentNumber());
  
  }
  //Als het spel is afgelopen, laat dan met behulp van de leds zien hoe goed de score was.
  else {
    //Laat de leds branden, deze methode is hieronder te vinden.
    showCompletedLeds(getScore());
  }

  //Delay 100ms om de ultrasoon sensor wat tijd te geven.
  delay(100);

  //Check voor button input, deze methode is hieronder te vinden. 
  getButton();
}

//Deze methode checkt of de button is ingedrukt. 
void getButton() {
  //Leest de waarde uit van de button pin.
  int value = digitalRead(BUTTON_PIN);
  //Als deze waarde LOW is, dan is de button ingedrukt. 
  if(value == LOW) 
  {
    //Check of de button al eens eerder was ingedrukt door de gebruiker, anders wordt deze if vaak aangeroepen terwijl de gebruiker maar 1x bedoelde. 
    if(!sameButtonPress) {
      //Deze methode regelt wat er gebeurt als de knop is ingedrukt, de methode is hieronder te vinden.
      onButtonPressed();
      //Zet deze waarde op true, zodat er maar 1x per button klik in deze methode wordt gedoken. 
      sameButtonPress = true;
    }
  }
  //Als de waarde HIGH is, dan is de button niet ingedrukt
  if(value == HIGH) {
    //Als same button press een keer is aangeroepen, zet deze dan op false zodat volgende keer weer gecheckt kan worden of de knop is ingedrukt. 
    if(sameButtonPress) {
      sameButtonPress = false;
    }
  }
}

//Deze methode wordt gebruikt voor de logica van onButtonPressed. 
void onButtonPressed() {
  //Als de game is afgelopen, start de game dan opnieuw, de startGame methode is te vinden in game.h
  if(isGameCompleted()) {
    startGame();
  }
  //Anders wordt het spel gestopt, de stopGame methode is te vinden in game.h
  else {
    stopGame();
  }
}

//Deze methode laat de ledjes branden om aan de gebruiker te laten zien of hij dichtbij is om een cijfer van de code te kraken. 
//Deze methode vraagt de afstand in cm en het huidige nummer dat gevonden moet worden. 
void turnOnLedsToGuessCode(long cm, int currentNumber) {
  //Als de ronde al afgerond is, dan doet deze methode even niets. 
  if(!isRoundCompleted()) {
      //Als de gebruiker dezelfde afstand heeft gevonden als het nummer van de code, laat dan alle ledjes branden. 
      if(cm == currentNumber) {
        digitalWrite(LED_PIN1, HIGH);
        digitalWrite(LED_PIN2, HIGH);
        digitalWrite(LED_PIN3, HIGH);
        return;
      }
      //Als de gebruiker dichtberbij de ultrasoon sensor zit dan het nummer van de code.
      //Laat het eerste led dan branden.
      else if(cm < currentNumber) {
        digitalWrite(LED_PIN1, HIGH);
        digitalWrite(LED_PIN2, LOW);
        digitalWrite(LED_PIN3, LOW);
      }
      //Als de gebruiker verder van de ultrasoon sensor zit dan het nummer van de code.
      //Laat het derde led dan branden.
      else if(cm > currentNumber) {
        digitalWrite(LED_PIN3, HIGH);
        digitalWrite(LED_PIN1, LOW);
        digitalWrite(LED_PIN2, LOW);
      }
  }
}

//Deze methode wordt aangeroepen om de gebruiker te laten weten dat de ronde is afgelopen.
//De leds gaan drie x aan en uit.
void blinkLedsOnComplete() {
    for(int i = 0; i < 3; i++) {
      digitalWrite(LED_PIN1, HIGH);
      digitalWrite(LED_PIN2, HIGH);
      digitalWrite(LED_PIN3, HIGH);
      delay(500);
      digitalWrite(LED_PIN1, LOW);
      digitalWrite(LED_PIN2, LOW);
      digitalWrite(LED_PIN3, LOW);
      delay(500);
    }
}

//Deze methode wordt gebruikt om de score van de gebruiker weer te geven. 
void showCompletedLeds(int score) {
  //Als de gebruiker het spel heeft afgerond tussen de 1 and 10 seconds.
  if(score == 1) {
    digitalWrite(LED_PIN1, HIGH);
    digitalWrite(LED_PIN2, HIGH);
    digitalWrite(LED_PIN3, HIGH);
  }
  //Als de gebruiker het spel heeft afgerond tussen de 10 and 20 seconds.
  else if(score == 2) {
    digitalWrite(LED_PIN1, HIGH);
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN3, HIGH);
  }
  //Als de gebruiker het spel heeft afgerond tussen de 20 and 30 seconds.
  else if(score == 3) {
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, HIGH);
    digitalWrite(LED_PIN3, LOW);
  }
  else {
    //Als de gebruiker het spel heeft afgerond na 30 seconds.
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN1, LOW);
  }
}
