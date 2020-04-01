#include "UltrasonicHandler.h"
#include <stdlib.h>
#include <Arduino.h>

//Deze methode zet de tijd in microseconde om naar cm.
//De afstand in cm wordt dan teruggegeven. 
long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
