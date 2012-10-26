/* Simple Image Drawing
 *
 * Draws an image to the screen.    The image is stored in "parrot.lcd" on 
 * the SD card.    The image file contains only raw pixel byte-pairs.
 */

#include <Adafruit_GFX.h>      // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include "mem_syms.h"

#include "lcd_image.h"
#include "restaurant.h"

// standard U of A library settings, assuming Atmel Mega SPI pins
#define SD_CS      5  // Chip select line for SD card
#define TFT_CS     6  // Chip select line for TFT display
#define TFT_DC     7  // Data/command line for TFT
#define TFT_RST    8  // Reset line for TFT (or connect to +5V)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

lcd_image_t map_image = { "yeg-sm.lcd", 128, 128 };

void dump_block(uint8_t* buf, size_t len) {
    for (size_t i =0; i < len; i++) {
        Serial.print(i);
        Serial.print(":");
        Serial.print(buf[i], DEC);
        Serial.print(" 0x");
        Serial.print(buf[i], HEX);
        Serial.print(" '");
        Serial.print( (char) buf[i]);
        Serial.print("'");
        Serial.println();
    }
}

#define BLOCK_LEN 512
Restaurant block_buf[BLOCK_LEN/sizeof(Restaurant)];

Sd2Card card;

#define RESTAURANT_START_BLOCK 4000000

void get_restaurant(int i, Restaurant *r) {
  Restaurant buffer[BLOCK_LEN/sizeof(Restaurant)];
  uint32_t block = RESTAURANT_START_BLOCK + i/8;
  uint32_t index = i % 8;

  card.readBlock(block, (uint8_t *) buffer);
  (*r) = buffer[index];
}

void printRest(uint32_t restaurantNumber) {
  Restaurant r;
  get_restaurant(restaurantNumber, &r);
  Serial.print("Restaurant #");
  Serial.print(restaurantNumber);
  Serial.print(" Name: ");
  Serial.println(r.name);
}

void setup(void) {
    Serial.begin(9600);

    // If your TFT's plastic wrap has a Red Tab, use the following:
    tft.initR(INITR_REDTAB);   // initialize a ST7735R chip, red tab
    // If your TFT's plastic wrap has a Green Tab, use the following:
    //tft.initR(INITR_GREENTAB); // initialize a ST7735R chip, green tab

    // how much memory have we got left at this point?
    Serial.print("Avail mem (bytes):");
    Serial.println(AVAIL_MEM);

    Serial.print("Initializing SD card...");
    if (!SD.begin(SD_CS)) {
      Serial.println("failed!");
      return;
    }
    Serial.println("OK!");

    // clear to yellow
    tft.fillScreen(tft.Color565(0xff, 0xff, 0x00));

    lcd_image_draw(&map_image, &tft, 0, 0, 0, 0, 128, 128);

    // how much memory have we got left at this point?
    Serial.print("Avail mem (bytes):");
    Serial.println(AVAIL_MEM);

    // test out reading blocks from the SD card
    if (!card.init(SPI_HALF_SPEED, SD_CS)) {
        Serial.println("Raw SD Initialization has failed");
        while (1) {};  // Just wait, stuff exploded.
        }

    // how much memory have we got left at this point?
    Serial.print("Avail mem (bytes):");
    Serial.println(AVAIL_MEM);
 
    uint32_t block_num = 4000000;
    uint32_t start = millis();
    for (int l=0; l<135; l++) {
        card.readBlock( block_num, (uint8_t *) block_buf);
        // Serial.println(block_buf[1].name);
        }
    uint32_t stop = millis();
    //Serial.println(stop - start);
    
    //dump_block((uint8_t *) block_buf, BLOCK_LEN);
    // Serial.println(block_buf[1].name);

    for(int i = 0; i < 1066; i++) {
      printRest(i);
    }
}


void loop() {
}
