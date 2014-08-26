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

void Cycle::setAmplitude(int amplitude){
    _amplitude = amplitude;
}

/* Changes the amplitude of the system, but fades this change in over time.
   fade is the time to fade in millis.
*/
void Cycle::setAmplitude(unsigned long time, int amplitude, int fade_time){
    _start_amplitude = _amplitude;
    _target_amplitude = amplitude;
    _amplitude_fade_start = time;
    _amplitude_fade_end = time + fade_time;
}

void Cycle::fadeAmplitude(unsigned long time){
    if( time < _amplitude_fade_end ){
        int amp_diff = _target_amplitude - _start_amplitude;
        float time_frac = ((float)(time - _amplitude_fade_start))/(_amplitude_fade_end - _amplitude_fade_start);
        _amplitude = _start_amplitude + ((int)(amp_diff * time_frac));
    }
}

int SineWave::value(unsigned long time)
{
    fadeAmplitude(time);
    
    float portion = adjustTime(time)/((float) _period) * 2 * M_PI; // normalize the proportion to 2PI
    return (int) ((sin(portion) + 1) * _amplitude) / 2;
}

int SquareWave::value(unsigned long time)
{
    fadeAmplitude(time);
    
    float portion = adjustTime(time)/((float) _period); // normalize the proportion to 2PI
    if( portion > 0.5 ){
        return _amplitude;
    }
    
    return 0;
}

const int N_ECG = 253;
const char ECG[253] = {49,55,74,111,152,191,222,244,255,255,240,195,131,71,28,6,0,4,12,18,24,28,32,34,37,39,41,41,41,43,43,43,43,43,43,45,45,47,47,47,51,53,53,51,51,53,55,57,57,57,61,65,69,71,74,74,74,78,78,80,84,88,94,98,100,104,108,115,119,123,125,127,129,131,133,135,135,135,133,131,129,127,121,115,111,106,102,98,94,88,82,78,76,74,69,65,61,59,59,59,59,59,59,57,53,51,51,51,53,53,53,53,53,55,57,57,59,59,59,59,61,61,61,61,59,59,59,59,61,63,65,61,59,55,53,51,51,49,47,47,47,47,47,45,45,47,47,47,47,47,49,49,47,47,47,47,47,47,47,45,45,45,45,45,45,43,43,43,45,47,47,47,45,45,45,45,45,47,47,47,47,47,47,49,51,51,49,47,47,47,47,49,51,51,49,51,53,53,53,51,51,51,51,53,55,55,57,57,57,53,49,47,49,49,49,49,51,53,55,55,57,55,53,53,55,55,57,59,61,65,67,65,63,65,69,71,71,67,65,61,59,59,59,59,59,57,57,57,59,57,55,51,49};

void Heartbeat::setAveraging(int averaging){
    _averaging = averaging;
}

int Heartbeat::value(unsigned long time)
{
    fadeAmplitude(time);    
    float portion = adjustTime(time)/((float) _period) * N_ECG; // normalize the proportion to 2PI
    
    if( _averaging == 0 ){
        return (int)(ECG[(int)portion] * (_amplitude/255.0));
    } else {
        int v = ECG[(int)portion];
        for(int i = 1; i <= _averaging; i++){
            v = ((2*(i-1)+1) * v + ECG[((int)portion+i) % N_ECG] + ECG[((int)portion-i) % N_ECG])/(2*i+1);
        }
        return (int)(v * (_amplitude/255.0));
    }
}
