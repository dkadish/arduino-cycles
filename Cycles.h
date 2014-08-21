/*
  Cycles.h - Library for creating looping cycles.
  Created by David Kadish, August 21, 2014.
  Released into the public domain.
*/

#ifndef Cycles_h
#define Cycles_h

#include <Arduino.h>

class Cycle
{
  public:
    Cycle(int period, int amplitude) : _period(period), _amplitude(amplitude) {};
    virtual int value(unsigned long time) = 0;
  protected:
    int _period; // The period in milliseconds
    int _amplitude; // The amplitude in units
};

class SineWave : public Cycle {
  public:
    SineWave(int period, int amplitude): Cycle( period, amplitude ) {};
    int value(unsigned long time);
};

#endif
