#include <Adafruit_GFX.h>      // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include "mem_syms.h"

#include "wiring_conventions.h"

#include "lcd_image.h"
#include "restaurant.h"
#include "joystick.h"
#include "conversion.h"
#include "distance.h"

#define RESTAURANTS_COUNT 1024
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

Sd2Card card;

lcd_image_t map_image = { "yeg-sm.lcd", 128, 128 };
uint16_t verticalMidpoint;

void loadDistances(uint16_t horiz, uint16_t vert, RestDist *distances) {
  for(int i = 0; i < RESTAURANTS_COUNT; i++) {
    Restaurant rest;
    getRestaurant(i, &rest, &card);
    uint16_t rest_horiz = lon_to_x(rest.longitude_scaled);
    uint16_t rest_vert = lat_to_y(rest.latitude_scaled);

    uint16_t distance = manhattanDist(horiz, vert, rest_horiz, rest_vert);

    RestDist restDist = { i, distance };
    distances[i] = restDist;
  }
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

void writeOutRestaurants(uint16_t startingIndex, RestDist *distances) {
  tft.fillScreen(0);
  tft.setCursor(0, 0);
  tft.setTextColor(0xFFFF);
  tft.setTextWrap(false);
  
  for (int i=startingIndex; i < 20 + startingIndex; i++) {
    Restaurant r;
    getRestaurant(distances[i].index, &r, &card);
    tft.print(i + 1);
    tft.print(". ");
    tft.print(r.name);
    tft.print("\n");
  }
  tft.print("\n");
}

void displayClosestRestaurants(uint16_t horiz, uint16_t vert) {
  RestDist distances[RESTAURANTS_COUNT];
  loadDistances(horiz, vert, distances);
  sortDistances(distances);

  int16_t startingIndex = 0;

  writeOutRestaurants(startingIndex, distances);

  // wait for button to be pressed
  while(!isButtonPressed()) { 
    uint16_t currentVertical = getVertical();
    int16_t shiftValue = 0;
    if(currentVertical > verticalMidpoint) {
      shiftValue = 10;
    } else if (currentVertical < verticalMidpoint) {
      shiftValue = -10;
    }

    if(shiftValue) {
      startingIndex = startingIndex + shiftValue;
      if(startingIndex < 0) {
        startingIndex = 0;
      } else {
      writeOutRestaurants((uint16_t) startingIndex, distances);
      }
    }

    delay(100);
  }

  // wait for button to be released
  while(isButtonPressed()) { }
}

void drawMap() {
    // clear to yellow
    tft.fillScreen(tft.Color565(0xff, 0xff, 0x00));

    lcd_image_draw(&map_image, &tft, 0, 0, 0, 0, 128, 128);
}

void setup(void) {
  Serial.begin(9600);
  pinMode(JOYSTICK_BUTTON, INPUT);
  pinMode(RATING_LED_0, OUTPUT);
  pinMode(RATING_LED_1, OUTPUT);
  pinMode(RATING_LED_2, OUTPUT);
  pinMode(RATING_LED_3, OUTPUT);
  pinMode(RATING_LED_4, OUTPUT);

  digitalWrite(JOYSTICK_BUTTON, HIGH);

  tft.initR(INITR_REDTAB);

  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed!");
    return;
  }
  Serial.println("OK!");

  drawMap();

  // test out reading blocks from the SD card
  if (!card.init(SPI_HALF_SPEED, SD_CS)) {
      Serial.println("Raw SD Initialization has failed");
      while (1) {};  // Just wait, stuff exploded.
      }

  verticalMidpoint = getVertical();
}

uint8_t getRatingFilter() {
  uint16_t input = analogRead(RATING_DIAL);
  return map(input, 0, 1023, 0, 5);
}

void lightUpRatingLEDs(uint8_t ratingFilter) {
  digitalWrite(RATING_LED_0, LOW);
  digitalWrite(RATING_LED_1, LOW);
  digitalWrite(RATING_LED_2, LOW);
  digitalWrite(RATING_LED_3, LOW);
  digitalWrite(RATING_LED_4, LOW);

  if(ratingFilter >= 1) {
    digitalWrite(RATING_LED_0, HIGH);
  }

  if(ratingFilter >= 2) {
    digitalWrite(RATING_LED_1, HIGH);
  }

  if(ratingFilter >= 3) {
    digitalWrite(RATING_LED_2, HIGH);
  }

  if(ratingFilter >= 4) {
    digitalWrite(RATING_LED_3, HIGH);
  }

  if(ratingFilter >= 5) {
    digitalWrite(RATING_LED_4, HIGH);
  }

}

void loop() {
  uint8_t ratingFilter = getRatingFilter();
  uint16_t horiz = getHorizontal();
  uint16_t vert = getVertical();
  lightUpRatingLEDs(ratingFilter);

  tft.drawPixel(horiz, vert, 0);
  if(isButtonPressed()) {
    // wait for button to be released
    while(isButtonPressed()) { }
    displayClosestRestaurants(horiz, vert);
    drawMap();
  }
  lcd_image_draw(&map_image, &tft, horiz, vert, horiz, vert, 1, 1);
}

