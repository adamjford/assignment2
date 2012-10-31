#include <Adafruit_GFX.h>      // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include "mem_syms.h"

#include "lcd_image.h"
#include "restaurant.h"
#include "joystick.h"

// standard U of A library settings, assuming Atmel Mega SPI pins
#define SD_CS      5  // Chip select line for SD card
#define TFT_CS     6  // Chip select line for TFT display
#define TFT_DC     7  // Data/command line for TFT
#define TFT_RST    8  // Reset line for TFT (or connect to +5V)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

Sd2Card card;

lcd_image_t map_image = { "yeg-sm.lcd", 128, 128 };

void setup(void) {
    Serial.begin(9600);

    tft.initR(INITR_REDTAB);

    Serial.print("Initializing SD card...");
    if (!SD.begin(SD_CS)) {
      Serial.println("failed!");
      return;
    }
    Serial.println("OK!");

    // clear to yellow
    tft.fillScreen(tft.Color565(0xff, 0xff, 0x00));

    lcd_image_draw(&map_image, &tft, 0, 0, 0, 0, 128, 128);

    // test out reading blocks from the SD card
    if (!card.init(SPI_HALF_SPEED, SD_CS)) {
        Serial.println("Raw SD Initialization has failed");
        while (1) {};  // Just wait, stuff exploded.
        }

    Restaurant r;
    getRestaurant(0, &r, &card);
    Serial.println(r.name);
}

void loop() {
  uint16_t horiz = getHorizontal();
  uint16_t vert = getVertical();

  tft.drawPixel(horiz, vert, 0);
  lcd_image_draw(&map_image, &tft, horiz, vert, horiz, vert, 1, 1);
}

