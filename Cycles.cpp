/*
  Cycles.cpp - Library for creating looping cycles.
  Created by David Kadish, August 21, 2014.
  Released into the public domain.
*/

#include <Arduino.h>
#include <math.h>
#include "Cycles.h"

int SineWave::value(unsigned long time)
{
    float portion = (time % _period)/((float) _period) * 2 * M_PI; // normalize the proportion to 2PI
    return (int) ((sin(portion) + 1) * _amplitude) / 2;
}
