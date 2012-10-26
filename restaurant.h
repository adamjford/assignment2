#ifndef RESTAURANT_H
#define RESTAURANT_H

/*
 Larger restaurant structure representing an entire restaurant
 as stored on the SD card.

 This is much too large to fit all of the 1000-odd restaurants into
 the Arduino's memory at once, but will be useful for printing off
 the information of individual restaurants, and is necessary for
 loading the restaurants off of the SD card into the smaller structs.
 */

typedef struct {
  int32_t latitude_scaled;  // Scaled by 100 000
  int32_t longitude_scaled; // Scaled by 100 000
  int8_t  rating;           // Average rating, scale from 0 - 10
  char name[55];            // Always null terminated
  
} Restaurant;


#endif
