// standard U of A library settings, assuming Atmel Mega SPI pins
#define SD_CS      5  // Chip select line for SD card
#define TFT_CS     6  // Chip select line for TFT display
#define TFT_DC     7  // Data/command line for TFT
#define TFT_RST    8  // Reset line for TFT (or connect to +5V)

// Joystick pins:
#define JOYSTICK_HORIZ 0    // Analog input A0 - horizontal
#define JOYSTICK_VERT  1    // Analog input A1 - vertical
#define JOYSTICK_BUTTON 9   // Digitial input pin 9 for the button

// potentiometer and ratings leds
#define RATING_DIAL 2       // Analog input A2 - restaurant dial selector

#define RATING_LED_0 2      // rating leds 0-4
#define RATING_LED_1 3      
#define RATING_LED_2 4      
#define RATING_LED_3 10      
#define RATING_LED_4 11
