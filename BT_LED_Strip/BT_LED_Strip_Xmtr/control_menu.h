#include <stdint.h>
// #include "WString.h"
#ifndef __CTRL_MENU__
#define __CTRL_MENU__

#define MENU_OPT_N 3
#define COLOR_OPT_N 15
#define PATTERN_OPT_N 5
#define MAX_STR_SZ 13

// 'brightness_icon', 16x16px
// const unsigned char led_brightness_icon [] PROGMEM = {
// 	0x00, 0x00, 0x40, 0x02, 0x20, 0x04, 0x13, 0xc8, 0x04, 0x20, 0x08, 0x10, 0x10, 0x08, 0x11, 0x88, 
// 	0x11, 0x88, 0x10, 0x08, 0x08, 0x10, 0x04, 0x20, 0x13, 0xc8, 0x20, 0x04, 0x40, 0x02, 0x00, 0x00
// };
// // 'color_icon', 16x16px
// const unsigned char led_color_icon [] PROGMEM = {
// 	0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x02, 0x40, 0x04, 0x20, 0x04, 0x20, 0x02, 0x40, 0x19, 0x98, 
// 	0x24, 0x24, 0x42, 0x42, 0x42, 0x42, 0x24, 0x24, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
// };
// // 'pattern_icon', 16x16px
// const unsigned char led_pattern_icon [] PROGMEM = {
// 	0x00, 0x00, 0x30, 0x0c, 0x4b, 0xd2, 0x48, 0x12, 0x30, 0x0c, 0x00, 0x20, 0x00, 0x40, 0x00, 0x80, 
// 	0x01, 0x00, 0x02, 0x00, 0x04, 0x00, 0x30, 0x0c, 0x48, 0x12, 0x4b, 0xd2, 0x30, 0x0c, 0x00, 0x00
// };

// Menu Display States
typedef enum{
  DSP_COLOR,  // When in Color selection menu
  DSP_PATTERN,  // When in Pattern selection menu 
  DSP_BRIGHTNESS, // When in Brightness selection menu
}MENU_DSP_STATE;

// Menu Mode
typedef enum{
  NAVIGATION, // When navigating through adjustable menus options
  SELECT, // When in selection mode for an adjustable menu option
}MENU_MODE;

// Menu titles for the OLED display
const char menu_titles[MENU_OPT_N][MAX_STR_SZ] PROGMEM = {
  "COLOR", // Color Menu Title
  "PATTERN", // Pattern Menu Title
  "BRIGHTNESS" // Brightness Menu Title
};

// Color Option Strings
const char color_opts[COLOR_OPT_N][MAX_STR_SZ] PROGMEM = {
  "OFF",
  "WHITE",
  "RED",
  "ROSE",
  "MAGENTA",
  "VIOLET",
  "BLUE",
  "AZURE",
  "CYAN",
  "SPRING_GREEN",
  "GREEN",
  "CHARTREUSE",
  "YELLOW",
  "ORANGE",
  "CANCEL"
};

// Pattern Option Strings
const char pattern_opts[PATTERN_OPT_N][MAX_STR_SZ] PROGMEM = {
  "SOLID",
  "PULSE",
  "FADER",
  "RAINBOW",
  "CANCEL"
};

int8_t MENU_IDX_LAST = 0; // tracks last menu index
int8_t MENU_IDX = 0; // tracks current menu index
int8_t SELECTION_OPT_LAST = -1; // tracks last menu option index
int8_t SELECTION_OPT_IDX = -1; // tracks current menu option index
int8_t LED_BRIGHTNESS_LAST = 100; // tracks last lighting menu setting
int8_t LED_BRIGHTNESS = 10; // tracks current lighting menu setting

MENU_MODE CURRENT_MENU_MODE = NAVIGATION; // Tracks current menu mode
MENU_DSP_STATE CURRENT_MENU_DSP_STATE = DSP_COLOR; // Tracks the current menu display state
char* MENU_TITLE = menu_titles[MENU_IDX]; // Stores current menu display title
char* OPT_SEL; // Stores current menu selection option

#endif