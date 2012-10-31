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

#define RESTAURANTS_COUNT 1024
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

Sd2Card card;

lcd_image_t map_image = { "yeg-sm.lcd", 128, 128 };

void loadDistances(uint16_t horiz, uint16_t vert, RestDist *distances) {

}

void sortDistances(RestDist *distances) {
  for(int i = 0; i < RESTAURANTS_COUNT - 1; i++) {
    int16_t smallesti = i;
    for(int j = i + 1; j < RESTAURANTS_COUNT; j++) {
      if(distances[j].dist < distances[smallesti].dist) {
        smallesti = j;
      }
    }

    RestDist original = distances[i];
    distances[i] = distances[smallesti];
    distances[smallesti] = original;
  }
}

void printArray(uint16_t *A, uint16_t n) {
  for(int i = 0; i < n; i++) {
    Serial.print(A[i]);
    Serial.print(", ");
  }
  Serial.println("");
}

void sortDistances(uint16_t *A, uint16_t n) {
  for(int i = 0; i < n - 1; i++) {
    int16_t smallesti = i;
    for(int j = i + 1; j < n; j++) {
      if(A[j] < A[smallesti]) {
        smallesti = j;
      }
    }

    uint16_t original = A[i];
    A[i] = A[smallesti];
    A[smallesti] = original;

    Serial.print("Iteration #");
    Serial.print(i);
    Serial.print(": ");
    printArray(A, n);
  }
}


void displayClosestRestaurants(uint16_t horiz, uint16_t vert) {
  RestDist distances[RESTAURANTS_COUNT];
  loadDistances(horiz, vert, distances);
  sortDistances(distances);
}

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
    
    uint16_t A[10] = { 16, 50, 40, 3, 11, 20, 2, 14, 8, 12 };
    uint16_t n = 10;

    Serial.print("Original array: ");
    printArray(A, n);
    sortDistances(A, 10);
}

void loop() {
  uint16_t horiz = getHorizontal();
  uint16_t vert = getVertical();
  uint8_t buttonIsPressed = isButtonPressed();

  tft.drawPixel(horiz, vert, 0);
  lcd_image_draw(&map_image, &tft, horiz, vert, horiz, vert, 1, 1);
  if(buttonIsPressed) {
    displayClosestRestaurants(horiz, vert);
  }
}

