/*
 * BtnHandler.h
 * Created by Jose Rivera, Feb 2018.
 *
 * This work is licensed under a Creative Commons Attribution 4.0 International License.
 * http://creativecommons.org/licenses/by/4.0/
 */

#include <inttypes.h>

#ifndef BTN_HANDLER_H_
#define BTN_HANDLER_H_

class BtnHandler
{
private:
  uint8_t _pin;
  uint32_t _debounce_delay = 100;
  uint32_t _long_press_delay = 500;
  uint32_t _last_time_pressed = 0;
  bool _short_pressed = false;
  bool _long_pressed = false;
  uint8_t _activate_with = 1;

  void(*_short_function_pointer)(void);
  void(*_long_function_pointer)(void);
public:
  BtnHandler(uint8_t, void (*)(void), void (*)(void));
  void activateWith(uint8_t);
  void setup(void);
  void loop(void);
  void interruption(void);
};

#endif /* BTN_HANDLER_H_ */
