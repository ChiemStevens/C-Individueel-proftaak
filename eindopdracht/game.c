#include "game.h"

#define HOLD_STILL_TIME   3000
#define LENGTH_OF_CODE    3

int num[3] = {0,0,0};
unsigned long startTime;
int currentNumber;
bool gameCompleted;
uint32_t holdStillTimer;
bool roundCompleted;       
double diff = 0.0;
int score;

//Deze methode wordt gebruikt om het spel te starten. 
void startGame() {
  //Reset de starttimer, zodat achteraf de score kan worden berekend.
  startTime = millis();

  //Stel de code in, bij deze nog hardcoded maar kan in de toekomst nog random gemaakt worden. 
  num[0] = 4;
  num[1] = 12;
  num[2] = 5;

  //Stel huidige nummer (van de array) in.
  currentNumber = 0;
  gameCompleted = false;

  //Reset de timer -- deze methode is hieronder te vinden. 
  resetTimer();
}

//Deze methode reset de hold still timer als de gebruiker niet de juiste afstand van de ultrasoon sensor heeft gevonden. 
void resetTimer() {
  holdStillTimer = millis();
}

//Deze methode wordt aangeroepen in de loop methode in de eindopdracht.ino file. 
//Deze methode dhect of de gebruiker zicht voor 3 seconde heeft weten still te houden.
void gameLoop() {
  if(!gameCompleted) {
    //Check of holdstillTimer wel is geinitialiseerd.
    if (holdStillTimer != 0) {
       //Als dat wel zo is, check dan of de gebruiker 3 seconden de juiste afstand heeft gehouden. 
       //Als dat zo is, dan is de ronde voorbij. 
       if ((millis() - holdStillTimer) > HOLD_STILL_TIME ) {
          roundCompleted = true;
       }
    }
  }
}

//Deze methode wordt gebruikt om te checken of het spel nog verder gaat.
void continueGame() {
  //Het huidige nummer van de code wordt verhoogd. 
  currentNumber++;

  //Als dit groter of gelijk is dan de lengte van de array, dan wordt het spel gestopt. 
  if(currentNumber >= LENGTH_OF_CODE) {
    stopGame();
  }
  //Als dat niet zo is, dan gaat het spel verder. 
  else {
    roundCompleted = false;
  }
}

//Deze methode stop het spel. 
void stopGame() {
    gameCompleted = true;

    //De huidige tijd in millis wordt opgevraagd.
    unsigned long currentTime = millis();
    //De start tijd wordt van de huidige tijd afgehaalt, hier komt dan de tijd uit die is gepaseerd om het spel te halen. 
    unsigned long elapsedTime = currentTime - startTime;

    //De score wordt berekend aan he hand van de gepaseerde tijd.  
    score = calculateScore(elapsedTime);
}

//De methode berekend de score aan de hand van de gepaseerde tijd.
int calculateScore(unsigned long elapsedTime) {
    //Between 1 and 10 seconds.
    if(elapsedTime <= 10000) {
      return 1; 
    }
    //Between 10 and 20 seconds.
    else if(elapsedTime <= 20000) {
      return 2;
    }
    //Between 20 and 30 seconds.
    else if(elapsedTime <= 30000) {
      return 3;
    }
    return 4;
}

//Deze methode checkt of de gebruiker de juiste afstand heeft tussen de ultrasoon sensor. 
void checkIfUserIsHoldingStill(int cm) {
  if(!roundCompleted) {
      if(cm == num[currentNumber]) {
        return;
      }
      resetTimer();
  }
}


//Hieronder staan de getters van de c file, zodat er in eindopdracht.ino gebruik van kan worden gemaakt.
int getCurrentNumber() {
  return num[currentNumber];
}

bool isRoundCompleted() {
  return roundCompleted;
}

bool isGameCompleted() {
  return gameCompleted;
}

int getScore() {
  return score;
}
