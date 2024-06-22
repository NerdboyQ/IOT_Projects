#include <stdint.h>
#include "bt_led_comm.h"


LED_Color::LED_Color (uint32_t value) {
  i = 0xFF & (value >> 24);
  r = 0xFF & (value >> 16);
  g = 0xFF & (value >>  8);
  b = 0xFF & value;
}

uint8_t LED_Color::get_intensity(){ return i; }
 
const LED_Color* const WHITE = new LED_Color(0x64D2D2D2);
const LED_Color* const RED = new LED_Color(0x64D20000);
const LED_Color* const ROSE = new LED_Color(0x64D20069);
const LED_Color* const MAGENTA = new LED_Color(0x64D200D2);
const LED_Color* const VIOLET = new LED_Color(0x646900D2);
const LED_Color* const BLUE = new LED_Color(0x640000D2);
const LED_Color* const AZURE = new LED_Color(0x640069D2);
const LED_Color* const CYAN = new LED_Color(0x6400D2D2);
const LED_Color* const SPRING_GREEN = new LED_Color(0x640000D2);
const LED_Color* const GREEN = new LED_Color(0x6400D200);
const LED_Color* const CHARTREUSE = new LED_Color(0x6469D200);
const LED_Color* const YELLOW = new LED_Color(0x64D2D200);
const LED_Color* const ORANGE = new LED_Color(0x64D26900);

const LED_Color* const COLOR_PALLETE[12] = {
  RED,
  ROSE,
  MAGENTA,
  VIOLET,
  BLUE,
  AZURE,
  CYAN,
  SPRING_GREEN,
  GREEN,
  CHARTREUSE,
  YELLOW,
  ORANGE
};