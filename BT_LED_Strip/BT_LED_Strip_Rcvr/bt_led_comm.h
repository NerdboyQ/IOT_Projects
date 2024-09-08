#ifndef __BT_LED_Comm_H__
#define __BT_LED_Comm_H__

#include <stdint.h>
// MAX Value to avoid LED burnout: 210 0xD2
// Half of the MAX would be: 105 0x69
#define MAX_LED_INTENSITY 210
#define COLOR_PALLETE_ARR_SZ 14


typedef union {
    uint8_t i:8;
    uint8_t r:8;
    uint8_t g:8;
    uint8_t b:8;
}LED_Color;

const LED_Color* OFF = 0x00000000;
const LED_Color* WHITE = 0x64D2D2D2;
const LED_Color* RED = 0x64D20000;
const LED_Color* ROSE = 0x64D20069;
const LED_Color* MAGENTA = 0x64D200D2;
const LED_Color* VIOLET = 0x646900D2;
const LED_Color* BLUE = 0x640000D2;
const LED_Color* AZURE = 0x640069D2;
const LED_Color* CYAN = 0x6400D2D2;
const LED_Color* SPRING_GREEN = 0x640069D2;
const LED_Color* GREEN = 0x6400D200;
const LED_Color* CHARTREUSE = 0x6469D200;
const LED_Color* YELLOW = 0x64D2D200;
const LED_Color* ORANGE = 0x64D26900;

const LED_Color* const COLOR_PALLETE[COLOR_PALLETE_ARR_SZ] = {
  OFF,
  WHITE,
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

// Array of all Predefined LED_Color objects, 12 in total
#endif 