/*
 * Get current horizontal position of joystick.
 * Return values go from 0 (left) to 127 (right).
 */
uint16_t getHorizontal();

/*
 * Get current horizontal position of joystick.
 * Return values go from 0 (up) to 159 (down).
 */
uint16_t getVertical();

/*
 * Return value will be true if the joystick button
 * is pressed, false otherwise.
 */
uint8_t isButtonPressed();
