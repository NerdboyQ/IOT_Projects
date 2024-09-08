#include <Adafruit_NeoPixel.h>
#include "bt_led_comm.h"

#include <SoftwareSerial.h>
#define RX 10 // Connect to HM10 TX for both AT and normal modes
#define TX 11  // Connect to HM10 RX for both AT and normal modes
SoftwareSerial HM10(RX,TX);
#include "bt_led_msgs.h"
BtPkt INC_BT_PKT;
BtPkt OUT_BT_PKT;

#define PIXEL_PIN 12
#define NUMPIXELS 300
#define DELAYVAL 500

Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

/**
* Sets the LED strip color and intensity
*
* @led_color : LED_Color object
*/
void set_strip(LED_Color led_color) {
  Serial.print(F(", r: "));
  Serial.print(led_color.r+0);
  Serial.print(F(", g: "));
  Serial.print(led_color.g+0);
  Serial.print(F(", b: "));
  Serial.println(led_color.b+0);

  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(led_color.r, led_color.g, led_color.b));
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
  // sendAtCmd("AT+AD2B0D2786E300A");  // Set Whitelisted device 2 to B0D2786E300A
  // sendAtCmd("AT+AD3B0D2786E300A");  // Set Whitelisted device 3 to B0D2786E300A
  sendAtCmd("AT+CON0B0D2786E300A");
  Serial.write("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void setup() {
  HM10.begin(9600);
  Serial.begin(9600);
  // while (!Serial) {
  //   HM10.println("Connect Serial Commnication");
  // }
  printHM10_settings();
  
  pixels.begin();
  set_strip(COLOR_PALLETE[5]);
  pixels.setBrightness(20);
}

bool newMsgRcvd = false;
uint32_t newPkt, oldPkt = 0;
char shift = 24;
#define PktSize 4
unsigned char buffer[PktSize];
char buffer_index;


void handlePing() {
  Serial.println("Handling ping");
  HM10.write(_DFLT_BT_RESP);
  buffer_index = 0;
}

uint32_t reverse_pkt(uint32_t raw_data) {
  uint32_t flipped = 0;
  for (char n = 0; n < 32; n+=8){
    uint32_t tmp = (0xFF & (raw_data >> n));
    flipped = (flipped << (24 - n)) | tmp;
  }

  return flipped;
}

void handleDfltColorMsg() {
  unsigned char intensity = INC_BT_PKT.byte1;
  unsigned char pattern = INC_BT_PKT.byte2;
  set_strip(COLOR_PALLETE[INC_BT_PKT.byte3]);
}


void loop() {
  // Always check for bt messages, then print to the screen
  while (HM10.available() ) {
    char bt_byte = HM10.read();
    if (bt_byte == _DFLT_BT_PING) handlePing();
    else {
      buffer[buffer_index] = bt_byte;
      Serial.println(buffer[buffer_index], HEX);
      buffer_index++;
      newMsgRcvd = true;
    }
  }

  if (buffer_index == PktSize) {
    // send response
    HM10.write(_DFLT_BT_RESP);
    // Serial.println(buffer[0]);
    INC_BT_PKT.byte0 = buffer[0];
    INC_BT_PKT.byte1 = buffer[1];
    INC_BT_PKT.byte2 = buffer[2];
    INC_BT_PKT.byte3 = buffer[3];
    Serial.print(F("Byte 0: "));
    Serial.print(OUT_BT_PKT.byte0, HEX);
    Serial.print(F(", Byte 1: "));
    Serial.print(OUT_BT_PKT.byte1, HEX);
    Serial.print(F(", Byte 2: "));
    Serial.print(OUT_BT_PKT.byte2, HEX);
    Serial.print(F(", Byte 3: "));
    Serial.println(OUT_BT_PKT.byte3, HEX);
    if (INC_BT_PKT.byte0 == _DFLT_BT_COLR) handleDfltColorMsg();
    // newPkt = reverse_pkt(newPkt);
    buffer_index = 0;
    // for (char i = 0; i < PktSize; i++){
    //   Serial.print("0x");
    //   Serial.print(buffer[i], HEX);
    //   Serial.print(" ");
    // }
    // Serial.println("");
    // Serial.print(newPkt, HEX);
    oldPkt = newPkt;
    newPkt = 0; // reset newPkt
    shift = 24; // reset shift
    Serial.write("\n~\n");
    // set_strip(CYAN);
    delay(1000);
    // set_strip(WHITE);
    newMsgRcvd = false;
  }
  // adjust_intensity(100, 1);
  // // adjust_intensity(1, 100);
  // loop_colors();
}
