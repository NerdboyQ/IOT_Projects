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

char buffer[MAX_STR_SZ];
int8_t incr;
// ==================================================================================================================
// BT Functions
// bool sendAtCmd(char* cmd) {
//   Serial.write("========================\n\n");
//   Serial.write("Cmd:");
//   Serial.write(cmd);
//   Serial.write("\n");
//   HM10.write(cmd);
  
//   if (String(cmd).indexOf("DISC") > 0) delay(3500); // default scan time is 3s
//   else delay(500);
//   while (HM10.available()) {
//     Serial.write(HM10.read());
//   }
//   Serial.write("\n");
//   return 1;
// }

/**
* Print HM10 Settings
*
*
*/
// void printHM10_settings() {
//   sendAtCmd("AT");  // Verifies solid UART communication if not actively connected, otherwise kills current connection
//   sendAtCmd("AT+NAME?");  // Gets the BT Device Advertised name
//   sendAtCmd("AT+ADDR?");  // Gets the BT Device BT Address
//   sendAtCmd("AT+BAUD?");  // Gets the Currently set baudrate 
//   sendAtCmd("AT+ROLE?");  // Gets the Currently set role (0: slave/peripheral, 1: master/central) 
//   sendAtCmd("AT+IMME1"); // Set Module Work type (only used for central/master mode) - 0: start work immediately, 1: only response to AT commands
//   sendAtCmd("AT+IMME?");
//   sendAtCmd("AT+ALLO1");  // Set Whitelist enabled mode; Enabled (1), Disable (0)
//   sendAtCmd("AT+ALLO?");  // Get Whitelist enabled mode; Enabled (1), Disable (0)
//   sendAtCmd("AT+AD10035FF0D419B");  // Set Whitelisted device 1 to 0035FF0D419B
//   sendAtCmd("AT+CON0035FF0D419B"); // attempt to connect to device 0035FF0D419B
  
//   Serial.write("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
// }

void send_bt_msg() {
  Serial.println(F("Color Msg"));
  HM10.write(OUT_BT_PKT.byte0);
  HM10.write(OUT_BT_PKT.byte1);
  HM10.write(OUT_BT_PKT.byte2);
  HM10.write(OUT_BT_PKT.byte3);
  Serial.print(F("Byte 0: "));
  Serial.print(OUT_BT_PKT.byte0, HEX);
  Serial.print(F(", Byte 1: "));
  Serial.print(OUT_BT_PKT.byte1, HEX);
  Serial.print(F(", Byte 2: "));
  Serial.print(OUT_BT_PKT.byte2, HEX);
  Serial.print(F(", Byte 3: "));
  Serial.println(OUT_BT_PKT.byte3, HEX);
  // c = (c + 1) % 14;
  delay(500);
}

void statusPing() {
  Serial.println(F("pinging rcvr"));
  HM10.write(_DFLT_BT_PING);
  delay(500);
}

void reconnect_bt() {
  display.clearDisplay();
  display.setCursor(0, 16);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.write("Connecting...");
  display.display();
  delay(5);

  Serial.println(F("Re-establishing connection to receiver."));
  char buffer[CONN_REQ_SZ];
  strcpy_P(buffer, RCVR_CONN_REQ);
  Serial.println(buffer);
  HM10.write(buffer);
  delay(2500);
  display.clearDisplay();
  display.setCursor(0, 16);
  display.write(F("CONNECTED!"));
  display.display();
  statusPing();
}

void handlePingResp() {
  Serial.println(F("Received Ping Response"));
  _last_bt_resp = millis();
  activeConnection = true;
}

// ==================================================================================================================
// Menu Control Functions

/**
* Shifts the display mode based on the
* current MENU Index
*/
void update_display_mode(){
  Serial.print(F("Menu IDX: "));
  Serial.println(MENU_IDX);
  if (!MENU_IDX) CURRENT_MENU_DSP_STATE = DSP_COLOR;
  else if (MENU_IDX == 1) CURRENT_MENU_DSP_STATE = DSP_PATTERN;
  else if (MENU_IDX == 2) CURRENT_MENU_DSP_STATE = DSP_BRIGHTNESS;
}

/**
* Updates the MENU or Selection INDEX based on
* the last encoder rotation transition
*
*/
void shift_arr(bool shiftUp=true) {
  incr = 1;
  if (shiftUp) { 
    incr = -1;
   } 
  switch(CURRENT_MENU_MODE){
    case NAVIGATION:
      MENU_IDX_LAST = MENU_IDX;
      MENU_IDX = (MENU_IDX+ incr) % MENU_OPT_N;
      // C follows the sign as a rule, so
      // we account for the negative values
      if (MENU_IDX <0) MENU_IDX+=MENU_OPT_N;
      MENU_TITLE = menu_titles[MENU_IDX];
      break;
    case SELECT:
      if (MENU_IDX < 2) {
          SELECTION_OPT_LAST = SELECTION_OPT_IDX;
          switch(CURRENT_MENU_DSP_STATE){
            case DSP_COLOR:
              SELECTION_OPT_IDX = (SELECTION_OPT_IDX+ incr) % COLOR_OPT_N;
              // C follows the sign as a rule, so
              // we account for the negative values
              if (SELECTION_OPT_IDX<0) SELECTION_OPT_IDX+=COLOR_OPT_N;
              OPT_SEL = color_opts[SELECTION_OPT_IDX];
              break;
            case DSP_PATTERN:
              SELECTION_OPT_IDX = (SELECTION_OPT_IDX+ incr) % PATTERN_OPT_N;
              // C follows the sign as a rule, so
              // we account for the negative values
              if (SELECTION_OPT_IDX<0) SELECTION_OPT_IDX+=PATTERN_OPT_N;
              OPT_SEL = pattern_opts[SELECTION_OPT_IDX];
              break;
          }
          

          Serial.println(F("Show Options Menu"));
          option_selection_update();
      } else {
        Serial.println(F("Show Brightness"));
        incr*=-10;
        if (LED_BRIGHTNESS >=0 && incr < 0) LED_BRIGHTNESS+=incr;
        else if (LED_BRIGHTNESS < 100 && incr > 0) LED_BRIGHTNESS+=incr;
        OUT_BT_PKT.byte2 = LED_BRIGHTNESS;
      }
      break;
  }
  
} 

/**
* Updates the displayed menu title
*/
void update_menu_title(){
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);
  display.setTextSize(2);
  // char buffer[MAX_STR_SZ];
  strcpy_P(buffer, MENU_TITLE);
  display.println(buffer);
  display.display();
  delay(5);
  
}

/**
* Updates the displayed Selection text
*/
void option_selection_update(){
  display.clearDisplay();
  update_menu_title();
  if (CURRENT_MENU_DSP_STATE == DSP_BRIGHTNESS){
    // TODO: implement brightness indicator
  } else {
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,18);
    display.setTextSize(2);
    // char buffer[MAX_STR_SZ];
    if (CURRENT_MENU_DSP_STATE == DSP_COLOR) strcpy_P(buffer, color_opts[SELECTION_OPT_IDX]);
    else if (CURRENT_MENU_DSP_STATE == DSP_PATTERN) strcpy_P(buffer, pattern_opts[SELECTION_OPT_IDX]);
    
    display.println(buffer);
    display.display();
    delay(5);
  }
}

void draw_brightness_bar() {
  update_menu_title();
  // draw box outline
  display.drawRect(2, 24, SCREEN_WIDTH-4, SCREEN_HEIGHT/3, SSD1306_WHITE);
  // draw level inside
  display.fillRect(8, 28, (SCREEN_WIDTH-16)*((float)LED_BRIGHTNESS/100), (SCREEN_HEIGHT/3)-8, SSD1306_INVERSE);
  display.display();
}

/**
* Changes the Selected value based on the current
* Menu & Selection index
*/
void handle_selection() {
  Serial.print(F("Currently in Selection Mode for "));
  if (MENU_IDX < 2){
    // char buffer[MAX_STR_SZ];
    strcpy_P(buffer, OPT_SEL);
    Serial.println((String)buffer);
    if ((CURRENT_MENU_DSP_STATE == DSP_COLOR & SELECTION_OPT_IDX == COLOR_OPT_N-1) || 
        (CURRENT_MENU_DSP_STATE == DSP_PATTERN & SELECTION_OPT_IDX == PATTERN_OPT_N-1)) {
      Serial.println(F("CANCELING"));
      CURRENT_MENU_MODE = NAVIGATION;
      SELECTION_OPT_IDX=0;
      update_menu_title();
    } else if(CURRENT_MENU_DSP_STATE == DSP_COLOR){
      OUT_BT_PKT.byte3 = (uint8_t)SELECTION_OPT_IDX;
      send_bt_msg();
    } else if (CURRENT_MENU_DSP_STATE == DSP_PATTERN) {
      OUT_BT_PKT.byte1 = (uint8_t)SELECTION_OPT_IDX;
      send_bt_msg();
    }
  } else {
    draw_brightness_bar();
  }
}

void menu_ctrl_logic(){
    if (MENU_IDX != MENU_IDX_LAST){
    MENU_IDX_LAST = MENU_IDX;
    update_menu_title();
    update_display_mode();
  }
  if (CURRENT_MENU_MODE == NAVIGATION & SELECTION_OPT_IDX != SELECTION_OPT_LAST){
    SELECTION_OPT_LAST = SELECTION_OPT_IDX;
    option_selection_update();
  }
  if (LED_BRIGHTNESS != LED_BRIGHTNESS_LAST) {
    LED_BRIGHTNESS_LAST = LED_BRIGHTNESS;
    draw_brightness_bar();
  }

  if (counter > lastCounter) {
    shift_arr();
    lastCounter = counter;
  } else if  (counter < lastCounter) {
    shift_arr(false);
    lastCounter = counter;
  }

  if (!digitalRead(SEL_C) & millis() - _lastSelectTime > _pauseLength/100) {
    Serial.println(F("selection"));
    if (CURRENT_MENU_MODE == SELECT){
      if (MENU_IDX < 2) handle_selection();
      else {
        Serial.println(F("CANCELING BRIGHTNESS"));
        send_bt_msg();
        CURRENT_MENU_MODE = NAVIGATION;
        update_menu_title();
      }
    } else if (CURRENT_MENU_MODE == NAVIGATION){
      Serial.print(F("Starting menu selection for: "));
      Serial.println(MENU_TITLE);
      CURRENT_MENU_MODE = SELECT;

      if (MENU_IDX < 2) {
        // Display menu options when entering SELECT mode
        SELECTION_OPT_IDX = 0;
        option_selection_update();
      } else {
        LED_BRIGHTNESS = 10;
        handle_selection();
      }
    }
    _lastSelectTime = millis();
    
  } 
}
// ==================================================================================================================

void setup() {
  
  Serial.begin(9600);
  HM10.begin(9600);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  update_menu_title();

  // Encoder Pins
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), read_encoder, CHANGE);

  pinMode(SEL_C, INPUT_PULLUP);


  while (!Serial) {
    Serial.println(F("Connect Serial Commnication"));
  }

  Serial.println(F("Booting..."));

  // Reset the HM10 BLE Module
  HM10.write("AT");
  delay(500);
  reconnect_bt();
  activeConnection = true;
  _last_bt_resp = millis();

  pinMode(TEST_LED, OUTPUT);

  update_menu_title();
  // printHM10_settings();
  
  // digitalWrite(TEST_LED, HIGH);
  // // Show initial display buffer contents on the screen --
  // // the library initializes this with an Adafruit splash screen.
  // display.display();
  // delay(2000); // Pause for 2 seconds

  // // Clear the buffer
  // display.clearDisplay();
}



// int8_t n = 1;
// bool newMsgRcvd = false;
void loop() {
  while (HM10.available()) {
    _last_bt_resp = millis();
    uint8_t bt_byte = HM10.read();
    if (bt_byte == _DFLT_BT_RESP) handlePingResp();
  }

  
  if (millis() - _last_bt_resp > _bt_conn_timeout) activeConnection = false;
  else if (millis() - _last_bt_resp > _bt_conn_timeout/2) statusPing();
  
  if(!activeConnection) reconnect_bt();
  else {
    // sendDftColor();
  }
  
  menu_ctrl_logic();
}
