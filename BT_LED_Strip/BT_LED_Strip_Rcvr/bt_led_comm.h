#ifndef __BT_LED_Comm_H__
#define __BT_LED_Comm_H__

#include <stdint.h>
// MAX Value to avoid LED burnout: 210 0xD2
// Half of the MAX would be: 105 0x69
#define MAX_LED_INTENSITY 210
#define COLOR_PALLETE_ARR_SZ 14

// LED Color as 4 x 8 bit values
typedef struct {
    uint8_t i:8; // LED_Color intensity/brightness
    uint8_t r:8; // LED_Color red value
    uint8_t g:8; // LED_Color green value
    uint8_t b:8; // LED_Color blue value
}LED_Color;

const LED_Color OFF {0x00,0x00,0x00,0x00}; // OFF LED_Color values' struct
const LED_Color WHITE {0x64,0xD2,0xD2,0xD2}; // WHITE LED_Color values' struct
const LED_Color RED {0x64,0xD2,0x00,0x00}; // RED LED_Color values' struct
const LED_Color ROSE {0x64,0xD2,0x00,0x69}; // ROSE LED_Color values' struct
const LED_Color MAGENTA {0x64,0xD2,0x00,0xD2}; // MAGENTA LED_Color values' struct
const LED_Color VIOLET {0x64,0x69,0x00,0xD2}; // VIOLET LED_Color values' struct
const LED_Color BLUE {0x64,0x00,0x00,0xD2}; // BLUE LED_Color values' struct
const LED_Color AZURE {0x64,0x00,0x69,0xD2}; // AZURE LED_Color values' struct
const LED_Color CYAN {0x64,0x00,0xD2,0xD2}; // CYAN LED_Color values' struct
const LED_Color SPRING_GREEN {0x64,0x00,0x69,0xD2}; // SPRING GREEN LED_Color values' struct
const LED_Color GREEN {0x64,0x00,0xD2,0x00}; // GREEN LED_Color values' struct
const LED_Color CHARTREUSE {0x64,0x69,0xD2,0x00}; // CHARTREUSE LED_Color values' struct
const LED_Color YELLOW {0x64,0xD2,0xD2,0x00}; // YELLOW LED_Color values' struct
const LED_Color ORANGE {0x64,0xD2,0x69,0x00}; // ORANGE LED_Color values' struct

// Array of all Predefined LED_Color objects, 12 in total
const LED_Color COLOR_PALLETE[COLOR_PALLETE_ARR_SZ] = {
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



#endif 