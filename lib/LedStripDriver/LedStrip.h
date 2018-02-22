/*
 * LedStrip.h
 * Created by Jose Rivera, Feb 2018.
 *
 * This work is licensed under a Creative Commons Attribution 4.0 International License.
 * http://creativecommons.org/licenses/by/4.0/
 */

#include <inttypes.h>

#ifndef LED_STRIP_H_
#define LED_STRIP_H_

/**
 * It allows to indicate if the leds of the strip are turned on or turned off.
 */
enum LedStripState
{
  ON,
  OFF
};

#define TURN_ON true
#define TURN_OFF false

/**
 * LedStrip allows to handle the output to a led strip of a single pin.
 * Its main functions are to turn on or turn off the LEDs and change the
 * intensity of brightness.
 */
class LedStrip
{
  private:
    uint8_t _pin;
    bool _state = false;
    uint8_t _intensity = 255;
    bool _common_anode = false;

  public:
    LedStrip(uint8_t pin);
    void setup(void);
    void setCommonAnodeEnable(bool);
    void turnOn(void);
    void turnOff(void);
    LedStripState toggle(void);
    void setState(LedStripState);
    LedStripState getState(void);
    void setIntensity(uint8_t);
    uint8_t getIntensity(void);
};

#endif /* LED_STRIP_H_ */