#ifndef __BT_LED_Comm_H__
#define __BT_LED_Comm_H__

#include <stdint.h>
// MAX Value to avoid LED burnout: 210 0xD2
// Half of the MAX would be: 105 0x69
#define MAX_LED_INTENSITY 210
#define COLOR_PALLETE_ARR_SZ 14


typedef struct {
    uint8_t i:8;
    uint8_t r:8;
    uint8_t g:8;
    uint8_t b:8;
}LED_Color;

const LED_Color OFF {0x00,0x00,0x00,0x00};
const LED_Color WHITE {0x64,0xD2,0xD2,0xD2};
const LED_Color RED {0x64,0xD2,0x00,0x00};
const LED_Color ROSE {0x64,0xD2,0x00,0x69};
const LED_Color MAGENTA {0x64,0xD2,0x00,0xD2};
const LED_Color VIOLET {0x64,0x69,0x00,0xD2};
const LED_Color BLUE {0x64,0x00,0x00,0xD2};
const LED_Color AZURE {0x64,0x00,0x69,0xD2};
const LED_Color CYAN {0x64,0x00,0xD2,0xD2};
const LED_Color SPRING_GREEN {0x64,0x00,0x69,0xD2};
const LED_Color GREEN {0x64,0x00,0xD2,0x00};
const LED_Color CHARTREUSE {0x64,0x69,0xD2,0x00};
const LED_Color YELLOW {0x64,0xD2,0xD2,0x00};
const LED_Color ORANGE {0x64,0xD2,0x69,0x00};

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

// Array of all Predefined LED_Color objects, 12 in total
#endif 