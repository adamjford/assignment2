#include <Arduino.h>

uint16_t selectPin = 2;
uint16_t horizPin = 0;
uint16_t vertPin = 1;
int16_t horizCalibration;
int16_t vertCalibration;

#define MAX_VOLTAGE 1023
#define MAX_HORIZONTAL 127
#define MAX_VERTICAL 127

uint16_t getHorizontal() {
  return map(analogRead(horizPin), 0, MAX_VOLTAGE, 0, MAX_HORIZONTAL);
}

uint16_t getVertical() {
  return map(analogRead(vertPin), 0, MAX_VOLTAGE, 0, MAX_VERTICAL);
}

uint8_t isButtonPressed() {
  return digitalRead(selectPin) ? 1 : 0;
}
