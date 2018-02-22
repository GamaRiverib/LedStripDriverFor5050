/*
 * LedStripRGB.cpp
 * Created by Jose Rivera, Feb 2018.
 *
 * This work is licensed under a Creative Commons Attribution 4.0 International License.
 * http://creativecommons.org/licenses/by/4.0/
 */
#include "LedStripRGB.h"
#include <Arduino.h>

LedStripRGB::LedStripRGB(RGBColor pins)
{
  this->_pins = pins;
}

RGBColor LedStripRGB::hex2rgb(uint32_t hex)
{
  RGBColor rgb = {
    static_cast<unsigned char>(((hex) >> 16) & 0xFF),
    static_cast<unsigned char>(((hex) >> 8) & 0xFF),
    static_cast<unsigned char>(hex & 0xFF)
  };
  return rgb;
}

void LedStripRGB::showColor(uint32_t color)
{
  RGBColor rgb = this->hex2rgb(color);
  if(this->_common_anode)
  {
    analogWrite(this->_pins.red, 255 - rgb.red);
    analogWrite(this->_pins.green, 255 - rgb.green);
    analogWrite(this->_pins.blue, 255 - rgb.blue);
  }
  else
  {
    analogWrite(this->_pins.red, rgb.red);
    analogWrite(this->_pins.green, rgb.green);
    analogWrite(this->_pins.blue, rgb.blue);
  }
}

void LedStripRGB::strobe(void)
{
  if((millis() - this->_last_sequence_time) > STROBE_DELAY)
  {
    this->_last_sequence_time = millis();
    if(this->_strobe_state == true)
    {
      this->showColor(this->_color);
      this->_strobe_state = false;
    }
    else
    {
      this->showColor(COLOR_BLACK);
      this->_strobe_state = true;
    }
  }
}

void LedStripRGB::flash(void)
{
  uint16_t delay = FLASH_DELAY + (600 * (this->_speed / 1024));
  if((millis() - this->_last_sequence_time) > delay)
  {
    this->_last_sequence_time = millis();
    if(this->_flash_counter >= FLASH_COLORS_SEQUENCE_LENGTH)
    {
      this->_flash_counter = 0;
    }
    this->showColor(FLASH_COLORS_SEQUENCE[this->_flash_counter++]);
  }
}

void LedStripRGB::fade(void)
{
  uint16_t delay = FADE_DELAY + (200 * (this->_speed / 1024));
  if((millis() - this->_last_sequence_time) > delay)
  {
    this->_last_sequence_time = millis();
    if(this->_fade_counter >= 6)
    {
      this->_fade_counter = 0;
    }
    switch (this->_fade_counter) {
      case 0:
        if (this->_fade_iteration < 256)
        {
          analogWrite(this->_pins.red, this->_fade_iteration++);
        }
        else
        {
          this->_fade_counter++;
        }
        break;
      case 1:
        if (this->_fade_iteration > 0)
        {
          analogWrite(this->_pins.blue, this->_fade_iteration--);
        }
        else
        {
          this->_fade_counter++;
        }
        break;
      case 2:
        if(this->_fade_iteration < 256)
        {
          analogWrite(this->_pins.green, this->_fade_iteration++);
        }
        else
        {
          this->_fade_counter++;
        }
        break;
      case 3:
        if(this->_fade_iteration > 0)
        {
          analogWrite(this->_pins.red, this->_fade_iteration--);
        }
        else
        {
          this->_fade_counter++;
        }
        break;
      case 4:
        if(this->_fade_iteration < 256)
        {
          analogWrite(this->_pins.blue, this->_fade_iteration++);
        }
        else
        {
          this->_fade_counter++;
        }
        break;
      case 5:
        if(this->_fade_iteration > 0)
        {
          analogWrite(this->_pins.green, this->_fade_iteration--);
        }
        else
        {
          this->_fade_counter++;
        }
        break;
      default:
        this->_fade_counter = 0;
        this->_fade_iteration = 0;
    }
  }
}

void LedStripRGB::setup(void)
{
  pinMode(this->_pins.red, OUTPUT);
  pinMode(this->_pins.green, OUTPUT);
  pinMode(this->_pins.blue, OUTPUT);
}

void LedStripRGB::setCommonAnodeEnable(bool enabled)
{
  this->_common_anode = enabled;
}

void LedStripRGB::turnOn(void)
{
  if(this->_state == false)
  {
    this->_state = true;
  }
}

void LedStripRGB::turnOff(void)
{
  if(this->_state)
  {
    if (this->_common_anode)
    {
      digitalWrite(this->_pins.red, HIGH);
      digitalWrite(this->_pins.green, HIGH);
      digitalWrite(this->_pins.blue, HIGH);
    }
    else
    {
      digitalWrite(this->_pins.red, LOW);
      digitalWrite(this->_pins.green, LOW);
      digitalWrite(this->_pins.blue, LOW);
    }
    this->_state = false;
  }
}

LedStripState LedStripRGB::toggle(void)
{
  this->_state = !this->_state;
  return this->_state ? LedStripState::ON : LedStripState::OFF;
}

void LedStripRGB::setState(LedStripState state)
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

LedStripState LedStripRGB::getState(void)
{
  return this->_state ? LedStripState::ON : LedStripState::OFF;
}

void LedStripRGB::setColor(uint32_t color)
{
  this->_color = color;
}

uint32_t LedStripRGB::getColor(void)
{
  return this->_color;
}

void LedStripRGB::setMode(LedStripRgbMode mode)
{
  this->_mode = mode;
}

LedStripRgbMode LedStripRGB::getMode(void)
{
  return this->_mode;
}

LedStripRgbMode LedStripRGB::nextMode(void)
{
  switch (this->_mode) {
    case LedStripRgbMode::NORMAL:
      this->_mode = LedStripRgbMode::STROBE;
      break;
    case LedStripRgbMode::STROBE:
      this->_mode = LedStripRgbMode::FLASH;
      break;
    case LedStripRgbMode::FLASH:
      this->_mode = LedStripRgbMode::FADE;
      break;
    case LedStripRgbMode::FADE:
      this->_mode = LedStripRgbMode::NORMAL;
      break;
    default:
      this->_mode = LedStripRgbMode::NORMAL;
  }
  this->_strobe_state = false;
  this->_flash_counter = 0;
  this->_fade_counter = 0;
  this->_fade_iteration = 0;
  return this->_mode;
}

uint16_t LedStripRGB::getSpeed(void)
{
  return this->_speed;
}

void LedStripRGB::setSpeed(uint16_t speed)
{
  this->_speed = constrain(speed, 0, 1024);
}

void LedStripRGB::loop(void)
{
  if(this->_state)
  {
    switch (this->_mode) {
      case LedStripRgbMode::NORMAL:
        this->showColor(this->_color);
        break;
      case LedStripRgbMode::STROBE:
        this->strobe();
        break;
      case LedStripRgbMode::FLASH:
        this->flash();
        break;
      case LedStripRgbMode::FADE:
        this->fade();
        break;
      default:
        this->showColor(this->_color);
    }
  }
}