/*
 * LedStripRGB.h
 * Created by Jose Rivera, Feb 2018.
 *
 * This work is licensed under a Creative Commons Attribution 4.0 International License.
 * http://creativecommons.org/licenses/by/4.0/
 */

#include <inttypes.h>
#include "LedStrip.h"
#include "RGBColors.h"

#ifndef LED_STRIP_RGB_H_
#define LED_STRIP_RGB_H_

enum LedStripRgbMode
{
  NORMAL,
  STROBE,
  FLASH,
  FADE
};

#define STROBE_DELAY 200
#define FLASH_DELAY 400
#define FADE_DELAY 5

class LedStripRGB
{
  private:
    RGBColor _pins;
    bool _state;
    uint32_t _color;
    uint16_t _speed;

    LedStripRgbMode _mode = LedStripRgbMode::NORMAL;
    uint32_t _last_sequence_time = 0;
    bool _strobe_state = false;
    uint32_t _flash_counter = 0;
    uint32_t _fade_counter = 0;
    uint32_t _fade_iteration = 0;

    bool _common_anode = false;

    RGBColor hex2rgb(uint32_t);
    void showColor(uint32_t);

    void strobe(void);
    void flash(void);
    void fade(void);

  public:
    LedStripRGB(RGBColor pins);
    void setup(void);
    void setCommonAnodeEnable(bool);
    void turnOn(void);
    void turnOff(void);
    LedStripState toggle(void);
    void setState(LedStripState);
    LedStripState getState(void);
    void setColor(uint32_t);
    uint32_t getColor(void);
    RGBColor getRGBColor(void);
    void setMode(LedStripRgbMode);
    LedStripRgbMode getMode(void);
    LedStripRgbMode nextMode(void);
    void setSpeed(uint16_t);
    uint16_t getSpeed(void);
    void loop(void);
};

#endif /* LED_STRIP_RGB_H_ */
