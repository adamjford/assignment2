#include <SD.h>
#include "Restaurant.h"

#define BLOCK_LEN 512
#define RESTAURANT_START_BLOCK 4000000

void getRestaurant(int i, Restaurant *r, Sd2Card *card) {
  Restaurant buffer[BLOCK_LEN/sizeof(Restaurant)];
  uint32_t block = RESTAURANT_START_BLOCK + i/8;
  uint32_t index = i % 8;

  card->readBlock(block, (uint8_t *) buffer);
  (*r) = buffer[index];
}
