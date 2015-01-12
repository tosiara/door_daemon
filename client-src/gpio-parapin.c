/*  This file is part of the package "parapin".

    The parapin package is free software; you can redistribute it
    and/or modify it under the terms of the GNU Library General Public
    License (LGPL) as published by the Free Software Foundation.

    The parapin package is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with parapin; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA


    For futher information on the parapin package, please refer to the
    project information hosted on Sourceforge --

    http://sourceforge.net/projects/parapin/

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "parapin.h"

#define DOOR_PIN 17

const int GPIO_STATE_CLOSED = 0;
const int GPIO_STATE_OPENED = 1;

int eventDetected();
int insideEvent();
int eventEnded();

int main(int argc, char *argv[])
{
  if (pin_init_user(LPT1) < 0)
    exit(1);

  pin_input_mode(LP_DATA_PINS);
  pin_input_mode(LP_SWITCHABLE_PINS);

  int breakMarker = 0;
  
  /* initial state must be "0" - "door closed" */
  int lastState = GPIO_STATE_CLOSED;
  
  while (!breakMarker)
  {
    int doorState = pin_is_set(LP_PIN[DOOR_PIN]);
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

