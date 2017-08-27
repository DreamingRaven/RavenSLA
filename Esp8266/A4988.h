// A4988.h : George Onoufriou
// A4988 Stepper Driver Handler

#ifndef A4988_h
#define A4988_h

#include "Arduino.h"

class A4988
{
  public:
    A4988();
    A4988(int dirPin_t, int stepPin_t);
    ~A4988();
    void setPins(int dirPin_t, int stepPin_t);
    void getPins(int &dirPin_t, int &stepPin_t);
    void step();
    void step(int clock);
    void changeDir();
    int getPosition();
    int getDir();
  private:
    int position = 0;
    int dirPin_m = -1; // id number of pin i.e pin 2
    int dirPin_state = -1; // state of pin, on/off/unknown 1/0/-1
    int stepPin_m = -1;
    int stepPin_state = -1;
};

#endif
