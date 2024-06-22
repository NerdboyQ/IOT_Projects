#ifndef __BT_LED_Comm_H__
#define __BT_LED_Comm_H__

#include <stdint.h>
// MAX Value to avoid LED burnout: 210 0xD2
// Half of the MAX would be: 105 0x69
#define MAX_LED_INTENSITY 210
#define COLOR_PALLETE_ARR_SZ 12


struct LED_Color {
    uint8_t i;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    LED_Color(uint32_t value);
    // Gets the current intensity of the LED strip 
    uint8_t get_intensity();
};

// WHITE Predefined LED_Color object
extern const LED_Color* const WHITE;
// RED Predefined LED_Color object
extern const LED_Color* const RED;
// ROSE Predefined LED_Color object
extern const LED_Color* const ROSE;
// MAGENTA Predefined LED_Color object
extern const LED_Color* const MAGENTA;
// VIOLET Predefined LED_Color object
extern const LED_Color* const VIOLET;
// BLUE Predefined LED_Color object
extern const LED_Color* const BLUE;
// AZURE Predefined LED_Color object
extern const LED_Color* const AZURE;
// CYAN Predefined LED_Color object
extern const LED_Color* const CYAN;
// SPRING_GREEN Predefined LED_Color object
extern const LED_Color* const SPRING_GREEN;
// GREEN Predefined LED_Color object
extern const LED_Color* const GREEN;
// CHARTREUSE Predefined LED_Color object
extern const LED_Color* const CHARTREUSE;
// YELLOW Predefined LED_Color object
extern const LED_Color* const YELLOW;
// ORANGE Predefined LED_Color object
extern const LED_Color* const ORANGE;

// Array of all Predefined LED_Color objects, 12 in total
extern const LED_Color* const COLOR_PALLETE[COLOR_PALLETE_ARR_SZ];
#endif 