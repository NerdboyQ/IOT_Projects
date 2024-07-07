#include <Adafruit_NeoPixel.h>
#include "bt_led_comm.h"

#include <SoftwareSerial.h>
#define RX 10 // Connect to HM10 TX for both AT and normal modes
#define TX 11  // Connect to HM10 RX for both AT and normal modes
SoftwareSerial HM10(RX,TX);

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

/**
* Sends AT commads for the HM10 BT Module
*
*/
bool sendAtCmd(char* cmd) {
  Serial.write("========================\n\n");
  Serial.write("Cmd:");
  Serial.write(cmd);
  Serial.write("\n");
  HM10.write(cmd);
  
  if (String(cmd).indexOf("DISC") > 0) delay(3500); // default scan time is 3s
  else delay(500);
  while (HM10.available()) {
    Serial.write(HM10.read());
  }
  Serial.write("\n");
  return 1;
}

/**
* Print HM10 Settings
*
*
*/
void printHM10_settings() {
  sendAtCmd("AT");  // Verifies solid UART communication if not actively connected, otherwise kills current connection
  sendAtCmd("AT+NAME?");  // Gets the BT Device Advertised name
  sendAtCmd("AT+BAUD?");  // Gets the Currently set baudrate 
  sendAtCmd("AT+ADDR?");  // Gets the BT Device BT Address
  sendAtCmd("AT+ROLE?");  // Gets the Currently set role (0: slave/peripheral, 1: master/central) 
  sendAtCmd("AT+ALLO1");  // Set Whitelist enabled mode; Enabled (1), Disable (0)
  sendAtCmd("AT+ALLO?");  // Get Whitelist enabled mode; Enabled (1), Disable (0)
  sendAtCmd("AT+AD1B0D2786E300A");  // Set Whitelisted device 1 to B0D2786E300A
  sendAtCmd("AT+AD2B0D2786E300A");  // Set Whitelisted device 2 to B0D2786E300A
  sendAtCmd("AT+AD3B0D2786E300A");  // Set Whitelisted device 3 to B0D2786E300A
  
  Serial.write("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void setup() {
  HM10.begin(9600);
  Serial.begin(9600);
  while (!Serial) {
    HM10.println("Connect Serial Commnication");
  }
  printHM10_settings();
  
  pixels.begin();
  set_strip(VIOLET);
}

bool newMsgRcvd = false;
void loop() {
  // Always check for bt messages, then print to the screen
  while (HM10.available()) {
    Serial.write(HM10.read()+0);
    newMsgRcvd = true;
  }

  if (newMsgRcvd) {
    Serial.write("\n~\n");
    set_strip(GREEN);
    adjust_intensity(0, 50);
    delay(1000);
    set_strip(WHITE);
    // adjust_intensity(0, 50);
    newMsgRcvd = false;
  }
  // adjust_intensity(100, 1);
  // // adjust_intensity(1, 100);
  // loop_colors();
}
