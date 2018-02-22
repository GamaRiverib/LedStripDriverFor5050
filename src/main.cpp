/*
 * Led Strip SMD 5050 controlled by Digispark ATTiny85
 * Created by Jose Rivera, Feb 2018.
 *
 * This work is licensed under a Creative Commons Attribution 4.0 International License.
 * http://creativecommons.org/licenses/by/4.0/
 */

/*
 * This sketch for the Digispark ATTiny85 to control a strip of leds SMD 5050
 * RGBW. It can also work without the white LED (W). The sketch considers a
 * circuit with a button that allows to change the mode of lights of the strip
 * of leds and a potentiometer to vary the brightness intensity or the speed of
 * the changes of lights.
 *
 * White light mode
 * When the driver starting to work, the led strip lights up with the white led,
 * with a brightness intensity determined by the value provided by the
 * potentiometer.
 *
 * Color light mode
 * While in the white light mode, pressing the button switches to the color mode,
 * that is, it turns off the white LED and shows the color defined by default
 * with the RGB LEDs, you can change the color of the LED strip by varying the
 * potentiometer value.
 *
 * Strobe mode
 * While in the color light mode, pressing the button switches to Strobe mode
 * using the same color as before, in the same way you can change the color with
 * the variation of the potentiometer.
 *
 * Flash mode
 * While in Strobe mode, pressing the button switches to Flash mode which
 * displays a predefined color sequence (mainly primary colors), you can change
 * the speed of the color change of the sequence, varying the speed of the
 * potentiometer.
 *
 * Fade mode
 * While in Flash mode, pressing the button switches to Fade mode which displays
 * a predefined sequence of colors, the transition between colors is more
 * gradual than in Flash mode, but its speed can be modified by varying the
 * value of the potentiometer.
 *
 * Off mode
 * If the button is held down for approximately one second, all the LEDs will
 * turn off. To turn on again you can press the button or modify the value of
 * the potentiometer.
 */

#include <Arduino.h>
#include "BtnHandler.h"
#include "LedStrip.h"
#include "LedStripRGB.h"

//uncomment this line if using a Common Anode LED
//#define COMMON_ANODE

// It allows to avoid that small variations of voltage turn on the light
#define THRESHOLD_FOR_TURN_ON 100

const uint8_t red_pin = 0; // P0
const uint8_t green_pin = 1; // P1
const uint8_t btn_mode_pin = 2; // P2
const uint8_t blue_pin = 3; // P3
const uint8_t white_pin = 4; // P4
const uint8_t pot_color_pin = 0; // A0

// Set a default color for the color mode
const uint32_t default_color = COLOR_DARKPURPLE;

// Allows validation if there is a change in voltage
uint16_t last_pot_color_value = 1;

// Instance that allows to handle the RGB leds of the strip of leds
LedStripRGB led_strip_rgb({ red_pin, green_pin, blue_pin });
// Instance that allows to handle the led of white light of the strip of leds
LedStrip led_strip_w(white_pin);

/*
 * When the mode button is pressed depending on the condition of the led strip,
 * different mode changes are made.
 *  - If the white and RGB LEDs are all off, then turn on the white LEDs.
 *  - If the RGB LEDs are off and the white LEDs are on, then turn off the white
 *    LEDs and turn on the LEDs of colors (with the last mode that has been set).
 *  - When the RGB LEDs are on and they are in Fade mode (last mode in the list),
 *    then turn off the RGB LEDs and turn on the white LEDs.
 *  - If the RGB LEDs are on and they are not in the Fade mode, then switch to
 *    the next mode in the list (NORMAL > STROBE > FLASH > FADE).
 */
void btnModeShortPressed(void)
{
  if(led_strip_w.getState() == LedStripState::OFF &&
    led_strip_rgb.getState() == LedStripState::OFF)
  {
    led_strip_w.turnOn();
  }
  else if(led_strip_rgb.getState() == LedStripState::OFF)
  {
    led_strip_w.turnOff();
    led_strip_rgb.turnOn();
  }
  else if(led_strip_rgb.getMode() == LedStripRgbMode::FADE)
  {
    led_strip_w.turnOn();
    led_strip_rgb.nextMode();
    led_strip_rgb.turnOff();
  }
  else
  {
    led_strip_rgb.nextMode();
  }
}

/*
 * When the mode button is pressed for approximately one second, then all the
 * LEDs are turned off.
 */
void btnModeLongPressed(void)
{
  led_strip_w.turnOff();
  led_strip_rgb.turnOff();
}

// Instance to handle button press events.
BtnHandler btn_mode(btn_mode_pin, btnModeShortPressed, btnModeLongPressed);

// Function to calculate a color based on an input voltage.
uint32_t color_mixer(uint16_t input_value)
{
  int32_t red = 0;
  int32_t green = 0;
  int32_t blue = 0;
  if(input_value < 341)
  {
    input_value = (input_value * 3) / 4;
    red = 256 - input_value;
    green = input_value;
    blue = 1;
  }
  else if (input_value < 682)
  {
    input_value = ((input_value - 341) * 3) / 4;
    red = 1;
    green = 256 - input_value;
    blue = input_value;
  }
  else
  {
    input_value = ((input_value - 683) * 3) / 4;
    red = input_value;
    green = 1;
    blue = 256 - input_value;
  }
  red = (red & 0xFF) << 16;
  green = (green & 0xFF) << 8;
  blue = blue & 0xFF;
  return red + green + blue;
}

/*
 * Function to read the voltage on the analog pin and based on the operating
 * mode perform an action.
 *  - When white LEDs are on, change the brightness of them.
 *  - When the RGB leds are turned on in Normal or Strobe mode, then change
 *  the color with the help of the color_mixer function.
 *  - If the RGB LEDs are on in Flash or Fade mode, then the speed of the color
 *    sequence is changed.
 */
void readPotValue(void)
{
  uint16_t new_pot_value = analogRead(pot_color_pin);
  if((new_pot_value / 4) != last_pot_color_value)
  {
    last_pot_color_value = new_pot_value / 4;
    if(led_strip_rgb.getState() == LedStripState::ON)
    {
      LedStripRgbMode mode = led_strip_rgb.getMode();
      switch (mode) {
        case LedStripRgbMode::NORMAL:
          led_strip_rgb.setColor(color_mixer(new_pot_value));
          break;
        case LedStripRgbMode::STROBE:
          led_strip_rgb.setColor(color_mixer(new_pot_value));
          break;
        case LedStripRgbMode::FLASH:
          led_strip_rgb.setSpeed(new_pot_value);
          break;
        case LedStripRgbMode::FADE:
          led_strip_rgb.setSpeed(new_pot_value);
          break;
      }
    }
    else if(led_strip_w.getState() == LedStripState::ON)
    {
      led_strip_w.setIntensity(last_pot_color_value);
    }
    else
    {
      led_strip_w.setIntensity(last_pot_color_value);
      led_strip_w.turnOn();
      if(abs((new_pot_value / 4) - last_pot_color_value) > THRESHOLD_FOR_TURN_ON)
      {
        led_strip_w.setIntensity(last_pot_color_value);
        led_strip_w.turnOn();
      }
    }
  }
}

/**
 * Function that allows to verify the correct operation of each one of the RGBW leds.
 */
void test_leds(void)
{
  led_strip_w.turnOn();
  led_strip_rgb.turnOff();
  led_strip_rgb.setMode(LedStripRgbMode::NORMAL);
  delay(500);
  led_strip_w.turnOff();
  led_strip_rgb.turnOn();
  delay(500);
  led_strip_rgb.setColor(COLOR_RED);
  led_strip_rgb.loop();
  delay(500);
  led_strip_rgb.setColor(COLOR_GREEN);
  led_strip_rgb.loop();
  delay(500);
  led_strip_rgb.setColor(COLOR_BLUE);
  led_strip_rgb.loop();
  delay(500);
}

/**
 * Set the pins for the LEDs and the button. For the ATTiny85 it is not
 * necessary to configure the analog input. Executes the function to verify the
 * operation of the RGBW LEDs and establishes the initial status of the LEDs
 * (white on, RGB off).
 */
void setup() {
  btn_mode.setup();
  led_strip_w.setup();
  led_strip_rgb.setup();

  test_leds();

  led_strip_w.turnOn();
  led_strip_rgb.turnOff();
  led_strip_rgb.setColor(default_color);
}

/**
 * In each iteration the voltage value in the analog input is read,
 * the button input and the RGB LEDs are updated (mainly by the Strobe, Flash
 * and Fade modes, which vary their color in time).
 */
void loop() {
  readPotValue();
  btn_mode.loop();
  led_strip_rgb.loop();
  delay(50);
}
