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
    void setPeriod(unsigned long time, int period);
    void setAmplitude(int amplitude);
    void setAmplitude(unsigned long time, int amplitude, int fade_time);
  protected:
    int _period; // The period in milliseconds
    int _amplitude; // The amplitude in units
    
    unsigned long adjustTime(unsigned long time); // Adjust for time offsets to retain smooth
    void fadeAmplitude(unsigned long time); // Adjust for time offsets to retain smooth transitions
  private:
    int _period_offset = 0;
    
    // Amplitude
    int _start_amplitude = 0;
    int _target_amplitude = 0;
    unsigned long _amplitude_fade_start = 0;
    unsigned long _amplitude_fade_end = 0;
};

class SineWave : public Cycle {
  public:
    SineWave(int period, int amplitude): Cycle( period, amplitude ) {};
    int value(unsigned long time);
};

class SquareWave : public Cycle {
  public:
    SquareWave(int period, int amplitude): Cycle( period, amplitude ) {};
    int value(unsigned long time);
};

class Heartbeat : public Cycle {
  public:
    Heartbeat(int period, int amplitude): Cycle( period, amplitude ){};
    int value(unsigned long time);
    void setAveraging(int averaging);
  private:
    int _averaging = 0;
};

#endif
