#ifdef __cplusplus
extern "C" {
#endif

//De ifdef __cplusplus die om deze header file heen staat is om een error tijdens het builden te voorkomen. 

#include "Arduino.h"
#include "time.h"
#include "UltrasonicHandler.h"

void startGame();
void resetTimer();
void gameLoop();
void continueGame();
void stopGame();
int calculateScore(unsigned long elapsedTime);
void checkIfUserIsHoldingStill(int cm);
int getCurrentNumber();
bool isRoundCompleted();
bool isGameCompleted();
int getScore();

#ifdef __cplusplus
}
#endif
