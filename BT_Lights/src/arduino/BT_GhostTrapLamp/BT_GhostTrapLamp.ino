#include <SoftwareSerial.h>
// Arduino being used is the Arduino Pro Mini 
// Set up a new SoftwareSerial object
SoftwareSerial BtSerial (2, 3);

#include <Servo.h>

#define ServoPin 5
#define ClosedPos 90
#define OpenedPos 180
Servo TrapDoor;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN    6
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 12

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:

// Using a Union for the BT Messaging. This is convenient as it allocates 
// contiguous memory, but certain bit ranges can be directly read/written
// for simple readability and control.
union BT_MSG{
  struct trap_door{
    bool isOpen :1;
  }trap_door;
  struct led_color{
    byte intensity :8;
    byte red :8;
    byte green :8;
    byte blue :8;
  }led_color;
};

const byte byte_cnt = 5;
byte holder_byte;
int16_t i = 0;
int16_t n = 0;
BT_MSG ctl_cmd, old_cmd;

void setup() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  // Set the baud rate for the SoftwareSerial object
  Serial.begin(9600);
  BtSerial.begin(9600);
  colorWipe(strip.Color(  0, 0, 255), 50); // Blue
  Serial.println("starting bt module test...");

  TrapDoor.attach(ServoPin);
  old_cmd.trap_door.isOpen = 0;
  TrapDoor.write(ClosedPos);
}

void loop() {  
  if (BtSerial.available() > 0) {
    Serial.println("cmd received...");
    holder_byte = BtSerial.read();
    switch (i) {
      case 4:
        ctl_cmd.trap_door.isOpen = holder_byte & 0x1;
        break; 
      case 3:
        ctl_cmd.led_color.intensity = holder_byte;
        break; 
      case 2:
        ctl_cmd.led_color.red = holder_byte;
        break; 
      case 1:
        ctl_cmd.led_color.green = holder_byte;
        break; 
      case 0:
        ctl_cmd.led_color.blue = holder_byte;
        break; 
    }

    Serial.print(i);
    Serial.print(" 0x");
    Serial.println(holder_byte, HEX);
    i+=1;
  }
  
  if (i == byte_cnt) {
    Serial.print("intensity: 0x");
    Serial.print(ctl_cmd.led_color.intensity, HEX);
    Serial.print(", red: 0x");
    Serial.print(ctl_cmd.led_color.red, HEX);
    Serial.print(", green: 0x");
    Serial.print(ctl_cmd.led_color.green, HEX);
    Serial.print(", blue: 0x");
    Serial.println(ctl_cmd.led_color.blue, HEX);
    colorWipe(strip.Color( ctl_cmd.led_color.red, ctl_cmd.led_color.green, ctl_cmd.led_color.blue), 1);
    if (ctl_cmd.trap_door.isOpen & !old_cmd.trap_door.isOpen){
      Serial.println("Opening trap door.");
      TrapDoor.write(OpenedPos);
    } else if (!ctl_cmd.trap_door.isOpen & old_cmd.trap_door.isOpen){
      Serial.println("Closing trap door.");
      TrapDoor.write(ClosedPos);
    }
    i = 0;
    // storing last control command value
    old_cmd = ctl_cmd;
    Serial.println("=======================================");
  }
  //colorWipe(strip.Color(  0, 0, 255), 50); // Blue
  // Fill along the length of the strip in various colors...
  /*
  colorWipe(strip.Color(255,   0,   0), 50); // Red
  colorWipe(strip.Color(  0, 255,   0), 50); // Green
  colorWipe(strip.Color(  0,   0, 255), 50); // Blue

  // Do a theater marquee effect in various colors...
  theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
  theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness

  rainbow(10);             // Flowing rainbow cycle along the whole strip
  theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant*/
}


// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  strip.setBrightness(ctl_cmd.led_color.intensity);
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}