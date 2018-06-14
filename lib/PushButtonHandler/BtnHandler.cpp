/*
 * BtnHandler.cpp
 * Created by Jose Rivera, Feb 2018.
 *
 * This work is licensed under a Creative Commons Attribution 4.0 International License.
 * http://creativecommons.org/licenses/by/4.0/
 */
#include "BtnHandler.h"
#include <Arduino.h>

BtnHandler::BtnHandler(uint8_t pin, void(*shortFn)(void), void(*longFn)(void))
{
  this->_pin = pin;
  this->_short_function_pointer = shortFn;
  this->_long_function_pointer = longFn;
}

void BtnHandler::activateWith(uint8_t level)
{
  this->_activate_with = level;
}

void BtnHandler::setup(void)
{
  pinMode(this->_pin, INPUT_PULLUP);
}

void BtnHandler::loop(void)
{
  if(digitalRead(this->_pin) == this->_activate_with)
  {
    if(this->_short_pressed == false)
    {
      this->_short_pressed = true;
      this->_last_time_pressed = millis();
    }
    if((millis() - this->_last_time_pressed > this->_long_press_delay) && !this->_long_pressed)
    {
      this->_long_pressed = true;
      this->_long_function_pointer();
    }
  }
  else
  {
    if(this->_short_pressed)
    {
      if(this->_long_pressed)
      {
        this->_long_pressed = false;
      }
      else
      {
        if((millis() - this->_last_time_pressed) > this->_debounce_delay)
        {
          this->_short_function_pointer();
        }
      }
      this->_short_pressed = false;
    }
  }
}

void BtnHandler::interruption(void)
{
  if((millis() - this->_last_time_pressed) > this->_debounce_delay)
  {
    this->_short_function_pointer();
  }
}
