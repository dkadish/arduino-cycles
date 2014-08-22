/*
  Cycles.cpp - Library for creating looping cycles.
  Created by David Kadish, August 21, 2014.
  Released into the public domain.
*/

#include <Arduino.h>
#include <math.h>
#include "Cycles.h"

/* Adjust the time for the period offset and the period length.
   Returns the time as a portion of the current period.
*/
unsigned long Cycle::adjustTime(unsigned long time){
    return (time + _period_offset) % _period;
}

void Cycle::setPeriod(unsigned long time, int period){
    unsigned long current_time = adjustTime(time);
    float expand = ((float)period)/_period;
    
    _period_offset = ((unsigned long)(current_time * expand)) - (time % period); 
    _period = period;
}

int SineWave::value(unsigned long time)
{
    float portion = adjustTime(time)/((float) _period) * 2 * M_PI; // normalize the proportion to 2PI
    return (int) ((sin(portion) + 1) * _amplitude) / 2;
}

const int N_ECG = 253;
const char ECG[253] = {27,28,42,72,113,153,194,228,255,250,216,157,96,42,13,1,0,3,7,10,13,15,17,18,19,21,21,20,21,22,22,22,22,22,22,22,24,24,23,24,26,27,27,26,25,27,29,29,28,29,31,34,35,37,37,36,37,38,41,39,41,45,47,49,50,52,54,56,60,61,61,63,65,65,66,67,68,67,66,65,65,63,61,57,54,53,52,49,47,44,41,39,38,37,35,32,31,30,30,30,30,31,30,29,27,26,26,26,27,27,27,27,26,28,29,29,30,30,30,30,31,31,31,31,30,30,30,30,31,33,33,32,29,28,27,26,26,25,24,24,24,24,24,23,23,24,24,25,24,24,25,25,24,23,24,24,24,24,24,23,23,23,23,23,23,22,22,22,23,24,24,24,23,22,23,23,23,24,24,24,23,24,24,25,27,26,24,24,24,23,24,25,26,26,25,25,27,28,27,26,25,26,26,27,28,28,29,29,29,28,24,23,26,25,25,25,26,27,27,29,29,28,27,27,28,28,28,30,31,33,34,34,31,32,35,37,36,34,33,31,29,30,31,30,30,29,28,30,30,29,29,27,21};

int Heartbeat::value(unsigned long time)
{
    float portion = adjustTime(time)/((float) _period) * N_ECG; // normalize the proportion to 2PI
    return (int)(ECG[(int)portion] * (_amplitude/255.0));
}
