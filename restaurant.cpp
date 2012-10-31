#include <SD.h>
#include "restaurant.h"

#define BLOCK_LEN 512
#define RESTAURANT_START_BLOCK 4000000

uint32_t cachedBlockNumber;
Restaurant buffer[BLOCK_LEN/sizeof(Restaurant)];

void getRestaurant(int i, Restaurant *r, Sd2Card *card) {
  uint32_t blockNumber = RESTAURANT_START_BLOCK + i/8;

  if(blockNumber != cachedBlockNumber) {
    card->readBlock(blockNumber, (uint8_t *) buffer);
    cachedBlockNumber = blockNumber;
  }

  uint32_t index = i % 8;

  (*r) = buffer[index];
}

