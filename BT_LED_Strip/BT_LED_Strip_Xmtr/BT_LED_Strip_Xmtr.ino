#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Rotary Encoder Pins
#define ENC_A 3
#define ENC_B 2
#define SEL_C 4

unsigned long _lastIncReadTime = micros();
unsigned long _lastDecReadTime = micros();
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

#define NUMFLAKES     10 // Number of snowflakes in the animation example

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


// 'brightness_icon', 16x16px
const unsigned char led_brightness_icon [] PROGMEM = {
	0x00, 0x00, 0x40, 0x02, 0x20, 0x04, 0x13, 0xc8, 0x04, 0x20, 0x08, 0x10, 0x10, 0x08, 0x11, 0x88, 
	0x11, 0x88, 0x10, 0x08, 0x08, 0x10, 0x04, 0x20, 0x13, 0xc8, 0x20, 0x04, 0x40, 0x02, 0x00, 0x00
};
// 'color_icon', 16x16px
const unsigned char led_color_icon [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x02, 0x40, 0x04, 0x20, 0x04, 0x20, 0x02, 0x40, 0x19, 0x98, 
	0x24, 0x24, 0x42, 0x42, 0x42, 0x42, 0x24, 0x24, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'pattern_icon', 16x16px
const unsigned char led_pattern_icon [] PROGMEM = {
	0x00, 0x00, 0x30, 0x0c, 0x4b, 0xd2, 0x48, 0x12, 0x30, 0x0c, 0x00, 0x20, 0x00, 0x40, 0x00, 0x80, 
	0x01, 0x00, 0x02, 0x00, 0x04, 0x00, 0x30, 0x0c, 0x48, 0x12, 0x4b, 0xd2, 0x30, 0x0c, 0x00, 0x00
};

// 'menu_bmp', 128x64px
const unsigned char led_menu_bmp [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x06, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x09, 0x7a, 0x40, 0x01, 0xf0, 0x00, 0x18, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x09, 0x02, 0x40, 0x03, 0xfc, 0x00, 0x18, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x06, 0x01, 0x80, 0x07, 0xfe, 0x1e, 0x1e, 0xf8, 0x7c, 0x0e, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x04, 0x00, 0x07, 0x9e, 0x7f, 0x1f, 0xfc, 0xfe, 0x1e, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x08, 0x00, 0x07, 0x0e, 0x7f, 0x9e, 0xf9, 0xfe, 0x3e, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x10, 0x00, 0x07, 0x0e, 0xf3, 0xd8, 0xe3, 0xde, 0x78, 0xf3, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x20, 0x00, 0x07, 0x9e, 0xc1, 0xd8, 0xe3, 0xbc, 0x70, 0xe1, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x40, 0x00, 0x07, 0xfc, 0xc1, 0xd8, 0xe3, 0xff, 0x70, 0xe1, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x80, 0x00, 0x07, 0xf8, 0xe3, 0xd8, 0xe3, 0xff, 0x70, 0xe1, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x06, 0x01, 0x80, 0x07, 0x70, 0xff, 0xde, 0xf9, 0xff, 0x70, 0xe1, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x09, 0x02, 0x40, 0x07, 0x00, 0x7f, 0xdf, 0x7c, 0xfe, 0x70, 0xe1, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x09, 0x7a, 0x40, 0x07, 0x00, 0x3f, 0xcf, 0x3c, 0x7c, 0x70, 0xe1, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x06, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x84, 0x00, 0x01, 0xf0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x84, 0x00, 0x02, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x48, 0x00, 0x04, 0x00, 0xf1, 0x0f, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x03, 0x33, 0x00, 0x04, 0x01, 0x09, 0x10, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x04, 0x84, 0x80, 0x04, 0x02, 0x05, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x48, 0x40, 0x04, 0x02, 0x05, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x48, 0x40, 0x04, 0x02, 0x05, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x04, 0x84, 0x80, 0x04, 0x02, 0x05, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x03, 0x03, 0x00, 0x02, 0x09, 0x09, 0x10, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0xf0, 0xf0, 0x8f, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x04, 0x00, 0x80, 0x01, 0xe0, 0x04, 0x00, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x79, 0x00, 0x02, 0x10, 0x00, 0x00, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x84, 0x00, 0x02, 0x10, 0x74, 0x3c, 0x5e, 0x38, 0xf0, 0x78, 0x7c, 0x7c, 0x00, 0x00, 0x00, 
	0x01, 0x02, 0x00, 0x03, 0xf0, 0x84, 0x42, 0x61, 0x21, 0x08, 0x84, 0x80, 0x80, 0x00, 0x00, 0x00, 
	0x02, 0x01, 0x00, 0x02, 0x09, 0x04, 0x81, 0x40, 0xa2, 0x05, 0x08, 0x80, 0x80, 0x00, 0x00, 0x00, 
	0x02, 0x31, 0x00, 0x02, 0x05, 0x04, 0x81, 0x40, 0xa2, 0x05, 0x10, 0x7c, 0x7c, 0x00, 0x00, 0x00, 
	0x02, 0x31, 0x00, 0x02, 0x05, 0x04, 0x81, 0x40, 0xa2, 0x05, 0x22, 0x02, 0x02, 0x00, 0x00, 0x00, 
	0x02, 0x01, 0x00, 0x02, 0x05, 0x04, 0x81, 0x40, 0xa2, 0x05, 0x42, 0x02, 0x02, 0x00, 0x00, 0x00, 
	0x01, 0x02, 0x00, 0x02, 0x09, 0x04, 0x43, 0x40, 0xa2, 0x04, 0x84, 0x02, 0x02, 0x00, 0x00, 0x00, 
	0x00, 0x84, 0x00, 0x03, 0xf1, 0x02, 0x3d, 0x40, 0x9a, 0x04, 0x78, 0xfc, 0xfc, 0x00, 0x00, 0x00, 
	0x02, 0x79, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x04, 0x00, 0x80, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x00, 0x40, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 1040)
// const int led_allArray_LEN = 1;
// const unsigned char* led_allArray[1] = {
// 	led_menu_bmp
// };



void setup() {
  Serial.begin(9600);

  // Encoder Pins
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), read_encoder, CHANGE);

  pinMode(SEL_C, INPUT_PULLUP);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  } else {
    Serial.println("\n\nConnected to OLED");
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  testdrawbitmap();
}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}

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
   }

  for (int8_t i = frst; i != last; i+=incr) {
    int8_t x = y_pos[i];
    y_pos[i] = y_pos[i+incr];
    y_pos[i+incr] = x;
  }
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


int8_t n = 1;

void loop() {
  // put your main code here, to run repeatedly:
  // testscrolltext();
  if (counter > lastCounter) {
    shift_arr();
    testdrawbitmap();
    lastCounter = counter;
  } else if  (counter < lastCounter) {
    shift_arr(false);
    testdrawbitmap();
    lastCounter = counter;
  }

  if (!digitalRead(SEL_C)) {
    Serial.println("selection");
  } 
}
