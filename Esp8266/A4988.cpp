// A4988.cpp : George Onoufriou
// A4988 Stepper Driver Handler

#include "Arduino.h"
#include "A4988.h"

A4988::A4988()
{
  // Empty constructor - special case
}

A4988::A4988(int dirPin_t, int stepPin_t)
{
  // non-empty constructor (unsafe)
  dirPin_m = dirPin_t;
  stepPin_m = stepPin_t;
  pinMode(dirPin_m, OUTPUT);
  pinMode(stepPin_m, OUTPUT);

  // setting pins low so they are in known state from now on
  digitalWrite(stepPin_m, LOW);
  stepPin_state = 0;
  digitalWrite(dirPin_m, LOW);
  dirPin_state = 0;
}

A4988::~A4988()
{
  // Empty where possible for optimisation
}

void A4988::setPins(int dirPin_t, int stepPin_t)
{
  // if pins valid

  // set pins (unsafe for speed)
  dirPin_m  = dirPin_t;
  stepPin_m = stepPin_t;
  pinMode(dirPin_m, OUTPUT);
  pinMode(stepPin_m, OUTPUT);

  // could make it return 0/1 for feedback - but wont for speed
}

void A4988::getPins(int &dirPin_t, int &stepPin_t)
{
  // if pins are !undefined

  // get pins
  dirPin_t = dirPin_m;
  stepPin_t = stepPin_m;
}

void A4988::step()
{
  digitalWrite(stepPin_m, HIGH);
  delay(2);
  digitalWrite(stepPin_m, LOW);
  delay(12); // works at 10 but high noise, smoothness ends at 20
  if(dirPin_state == 1)
  {
    position += 1;
  }
  else if(dirPin_state == 0)
  {
    position -= 1;
  }
}

void A4988::step(int clock_t)
{
  if(clock_t == dirPin_state)
  {
    step();
  }
  else
  {
    //Serial.println("switch");
    changeDir();
    step();
  }
}

void A4988::changeDir()
{
  if(dirPin_state == 1)
  {
    //Serial.print("going from 1 to 0: dir state = ");
    dirPin_state = 0;
    digitalWrite(dirPin_m, LOW);
    //Serial.println(dirPin_state);
  }
  else if(dirPin_state == 0)
  {
    //Serial.print("going from 0 to 1: dir state = ");
    dirPin_state = 1;
    digitalWrite(dirPin_m, HIGH);
    //Serial.println(dirPin_state);
  }
  delay(300);
}

int A4988::getPosition()
{
  return position;
}

int A4988::getDir()
{
  return dirPin_state;
}
