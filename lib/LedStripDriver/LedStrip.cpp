/*
 * LedStrip.cpp
 * Created by Jose Rivera, Feb 2018.
 *
 * This work is licensed under a Creative Commons Attribution 4.0 International License.
 * http://creativecommons.org/licenses/by/4.0/
 */
#include "LedStrip.h"
#include <Arduino.h>

/**
 * Constructor of the class.
 * @param pin Pin of exit towards the led strip
 */
LedStrip::LedStrip(uint8_t pin)
{
  this->_pin = pin;
}

/**
 * Set the controller pin as an output.
 */
void LedStrip::setup(void)
{
  pinMode(this->_pin, OUTPUT);
}

/**
 * Allows to establish if the connected LED strip is common anode or common
 * cathode. By default is common cathode (enabled = false).
 * @param enabled Set true for common anode led strip.
 */
void LedStrip::setCommonAnodeEnable(bool enabled)
{
  this->_common_anode = enabled;
}

/**
 * It allows to turn on the LEDs of the strip.
 */
void LedStrip::turnOn(void)
{
  if(this->_state == false)
  {
    if(this->_intensity == 0)
    {
      this->_intensity = 255;
    }
    if(this->_common_anode)
    {
      analogWrite(this->_pin, 255 - this->_intensity);
    }
    else
    {
      analogWrite(this->_pin, this->_intensity);
    }
    this->_state = true;
  }
}

/**
 * It allows to turn off the LEDs of the strip.
 */
void LedStrip::turnOff(void)
{
  if(this->_state)
  {
    if(this->_common_anode)
    {
      digitalWrite(this->_pin, HIGH);
    }
    else
    {
      digitalWrite(this->_pin, LOW);
    }
    this->_state = false;
  }
}

/**
 * It allows to change the status of the strip of LEDs, that is, turn off the
 * LEDs if they are turned on or turn them on if they are turned off.
 * @return  The state to which the LEDs were changed
 */
LedStripState LedStrip::toggle(void)
{
  if(this->_state)
  {
    this->turnOff();
    return LedStripState::OFF;
  }
  else
  {
    this->turnOn();
    return LedStripState::ON;
  }
}

/**
 * It allows to establish the status of the LEDs of the strip, that is, turn on
 * or turn off.
 */
void LedStrip::setState(LedStripState state)
{
  if(state == LedStripState::ON)
  {
    this->turnOn();
  }
  else
  {
    this->turnOff();
  }
}

/**
 * It allows to obtain the current status of the LEDs of the strip
 * @return  The current state
 */
LedStripState LedStrip::getState(void)
{
  return this->_state ? LedStripState::ON : LedStripState::OFF;
}

/**
 * Allows you to set the brightness intensity of the LEDs.
 * If the set intensity is equal to zero, then it goes to the off state.
 * If the intensity is greater than zero and the LEDs are in the off state,
 * then the power state is switched on.
 */
void LedStrip::setIntensity(uint8_t intensity)
{
  this->_intensity = constrain(intensity, 0, 255);
  if(intensity == 0 && this->_state)
  {
    this->turnOff();
  }
  else if(this->_state)
  {
    if(this->_common_anode)
    {
      analogWrite(this->_pin, 255 - this->_intensity);
    }
    else
    {
      analogWrite(this->_pin, this->_intensity);
    }
  }
  else
  {
    this->turnOn();
  }
}

/**
 * It allows to obtain the intensity of current brightness.
 */
uint8_t LedStrip::getIntensity(void)
{
  return this->_intensity;
}