#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <SoftwareSerial.h>
#define TEST_LED 5
#define RX 6// Connect to HM10 TX for both AT and normal modes
#define TX 7  // Connect to HM10 RX for both AT and normal modes
SoftwareSerial HM10(RX,TX);

#include "control_menu.h"
MENU_STATE CURRENT_MENU_STATE = S_MAIN;
MAIN_MENU_OPT MAIN_MENU_SEL = M_OPT_COLOR;

#include "bt_led_msgs.h"

// Rotary Encoder Pins
#define ENC_A 3
#define ENC_B 2
#define SEL_C 4

unsigned long _lastIncReadTime = micros();
unsigned long _lastDecReadTime = micros();
unsigned long _lastSelectTime = micros();
const uint16_t _pauseLength = 25000;

volatile int8_t counter = 0;
volatile int8_t lastCounter = counter;
// Interrupt_Routine to process encode transitions
void read_encoder() {
  static uint8_t old_AB = 3;  // Lookup Table Index
  static int8_t encval = 0;   // encode value
  static const int8_t enc_states[] = {
     0, -1,  1,  0,  
     1,  0,  0, -1, 
    -1,  0,  0,  1,
     0,  1, -1,  0
  };

  old_AB <<= 2;   // Remember Previous State

  if (digitalRead(ENC_A)) old_AB |= 0x02;   // Add current state of pin A
  if (digitalRead(ENC_B)) old_AB |= 0x01;   // Add current state of pin B

  encval += enc_states[( old_AB & 0x0F )];

  // update counter if encoder has rotated a full indent
  if ( encval > 3 || encval < -3) {
    int changevalue = (encval > 0) - (encval < 0);
    if((micros() - _lastIncReadTime) < _pauseLength) {
      changevalue = 10*changevalue;
    } 
    _lastIncReadTime = micros();
    counter = counter + changevalue;
    encval = 0;
  }

}

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };



int8_t y_pos[] = {0,24,48};

String items[] = {
  "Color",
  "Pattern",
  "Brightness",
};


void shift_arr(bool shiftUp=true) {
  int8_t frst = 0;
  int8_t last = 2;
  int8_t incr = 1;
  if (shiftUp) { 
    frst = 2;
    last = 0;
    incr = -1;
    MAIN_MENU_SEL = (MAIN_MENU_SEL + 1) % 3;
   } else {
    MAIN_MENU_SEL = (MAIN_MENU_SEL - 1) % 3;
   }

  for (int8_t i = frst; i != last; i+=incr) {
    int8_t x = y_pos[i];
    y_pos[i] = y_pos[i+incr];
    y_pos[i+incr] = x;
  }
} 


void setup() {
  
  Serial.begin(9600);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  testdrawbitmap();
  delay(500);

  // Encoder Pins
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), read_encoder, CHANGE);

  pinMode(SEL_C, INPUT_PULLUP);

  
}
void testdrawbitmap() {
  display.clearDisplay();
  display.drawBitmap(
    3,
    y_pos[0],
    led_color_icon, 16, 16, 1);
  display.setTextSize(1.5);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(29,y_pos[0]+2);             // Start at top-left corner
  display.println(items[0]);

  display.drawBitmap(
    3,
    y_pos[1],
    led_pattern_icon, 16, 16, 1);
  
  display.setCursor(29,y_pos[1]+2);             // Start at top-left corner
  display.println(items[1]);

  display.drawBitmap(
    3,
    y_pos[2],
    led_brightness_icon, 16, 16, 1);
  display.setCursor(29,y_pos[2]+2);             // Start at top-left corner
  display.println(items[2]);

  display.display();
}
void loop() {
  if (counter > lastCounter) {
    shift_arr();
    testdrawbitmap();
    lastCounter = counter;
  } else if  (counter < lastCounter) {
    shift_arr(false);
    testdrawbitmap();
    lastCounter = counter;
  }

  if (!digitalRead(SEL_C) & millis() - _lastSelectTime > _pauseLength/100) {
    Serial.println("selection");
    // Serial.println(MENU_OPT_TITLES[MAIN_MENU_SEL]);
    // testFxn();
    _lastSelectTime = millis();

  } 
}
