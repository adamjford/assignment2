#include <Arduino.h>

/* 
 * Return the Manhattan distance between two positions.
 */
uint16_t manhattanDist(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  return abs(x2 - x1) + abs(y2 - y1);
}
