#include <Adafruit_NeoPixel.h>
#include "bt_led_comm.h"

#define PIXEL_PIN 12
#define NUMPIXELS 300
#define DELAYVAL 500

Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

/**
* Sets the LED strip color and intensity
*
* @led_color : LED_Color object
*/
void set_strip(LED_Color* led_color) {
  Serial.print("r: ");
  Serial.print(led_color->r+0);
  Serial.print(", g: ");
  Serial.print(led_color->g+0);
  Serial.print(", b: ");
  Serial.println(led_color->b+0);

  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(led_color->r, led_color->g, led_color->b));
  }
    pixels.show();   // Send the updated pixel colors to the hardware.
}

/**
* Makes smooth transitions for adjusting light intensity
*
* @start_val : initial intensity to start with
* @final_val : final intensity to start with
*/
void adjust_intensity(uint8_t start_val, uint8_t final_val) {
  int8_t inc = 1;
  int8_t i = start_val;
  if (start_val > final_val) { inc = -1; }
  while ( i != final_val) {
    pixels.setBrightness(i);
    pixels.show(); 
    i+=inc;
  }
}

/**
* Test to loop through all predefined colors
*
*/
void loop_colors(){
  for (int8_t i = 0; i < 12; i++) {
    set_strip(COLOR_PALLETE[i]);
    delay(500);
  }
}

void setup() {
  Serial.begin(9600);
  pixels.begin();
  set_strip(VIOLET);
}
void loop() {
  adjust_intensity(100, 1);
  // adjust_intensity(1, 100);
  loop_colors();
}
