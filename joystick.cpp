#include <Arduino.h>
#include "wiring_conventions.h"

int16_t horizCalibration;
int16_t vertCalibration;

#define MAX_VOLTAGE 1023
#define MAX_HORIZONTAL 127
#define MAX_VERTICAL 127

uint16_t getHorizontal() {
  return map(analogRead(JOYSTICK_HORIZ), 0, MAX_VOLTAGE, 0, MAX_HORIZONTAL);
}

uint16_t getVertical() {
  return map(analogRead(JOYSTICK_VERT), 0, MAX_VOLTAGE, 0, MAX_VERTICAL);
}

uint8_t isButtonPressed() {
  return digitalRead(JOYSTICK_BUTTON) ? 0 : 1;
}
