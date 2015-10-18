#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <wiringPi.h>

#define DOOR_PIN 6

const int GPIO_STATE_CLOSED = 0;
const int GPIO_STATE_OPENED = 1;

int eventDetected();
int insideEvent();
int eventEnded();

int main(int argc, char *argv[])
{
  wiringPiSetup();
  pinMode (DOOR_PIN, INPUT);

  int breakMarker = 0;
  
  /* initial state must be "0" - "door closed" */
  int lastState = GPIO_STATE_CLOSED;
  
  while (!breakMarker)
  {
    int doorState = digitalRead (DOOR_PIN);
    if (doorState != GPIO_STATE_CLOSED) doorState = GPIO_STATE_OPENED;

    int sensorToggled = (doorState != lastState);
    if (sensorToggled && lastState == GPIO_STATE_CLOSED)
      breakMarker = eventDetected();
    else if (sensorToggled && lastState == GPIO_STATE_OPENED)
      breakMarker = eventEnded();
    else if (!sensorToggled && lastState == GPIO_STATE_OPENED)
      breakMarker = insideEvent();
    lastState = doorState;
    usleep (400000);
  }  

  return 0;
}

int eventDetected()
{
  return 0;
}

int insideEvent()
{
  return 0;
}

int eventEnded()
{
  return 0;
}

