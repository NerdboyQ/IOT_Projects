#include <stdint.h>
#include <avr/pgmspace.h>
#ifndef __BT_LED_MSGs__
#define __BT_LED_MSGs__

#define CONN_REQ_SZ 20  // size of char array for BT Device Connection Request command
// Dedicated BLE HM10 module
// change for your specific module
const char RCVR_CONN_REQ[CONN_REQ_SZ] PROGMEM = "AT+CON0035FF0D419B"; // BT Device Connection Request command
const unsigned long _bt_conn_timeout = 20000; // 20s (20000ms) wait for no slave response, trigger to attempt reconnection
const unsigned long _bt_ping_interval = _bt_conn_timeout/4; // 5s (50000ms) ping interval to check for constant connection
bool activeConnection = false; // tracks connection status
volatile unsigned long _last_bt_resp = 0; // time of last bluetooth response from slave
volatile unsigned long _last_con_req = 0; // time of last connection request to slave
volatile unsigned long _last_png_req = 0; // time of last ping response from slave
volatile unsigned long _curr_milli_t = 0; // current time in milliseconds
volatile unsigned long _delta_milli_t = 0; // placeholder for elapsed time in milliseconds

// LED Pattern Modes
typedef enum {
  _SOLID = 0x00, // Solid LED Pattern
  _PULSE = 0x01, // Pulsing LED Pattern
  _FADER = 0x02, // Fading LED Pattern
  _RNBOW = 0x03, // Rainbow Color Pattern
}Patterns;

// HeadByte Recognized Values
typedef enum {
  _DFLT_BT_PING = 0x81, // Master ping head byte
  _DFLT_BT_RESP = 0x7E, // Slave response ack for received message head byte
  _DFLT_BT_COLR = 0x65, // Color command head byte
}HeadByte;

// Bytes for color options
typedef enum {
  _OFF = 0x00, // Off Color Option
  _WHITE = 0x01, // White Color Option
  _RED = 0x02, // Red Color Option
  _ROSE = 0x03, // Rose Color Option
  _MAGENTA = 0x04, // Magenta Color Option
  _VIOLET = 0x05, // Violet Color Option
  _BLUE = 0x06, // Blue Color Option
  _AZURE = 0x07, // Azure Color Option
  _CYAN = 0x08, // Cyan Color Option
  _SPRING_GREEN = 0x09, // Spring Green Color Option
  _GREEN = 0x0A, // Green Color Option
  _CHARTREUSE = 0x0B, // Chartreuse Color Option
  _YELLOW = 0x0C, // Yellow Color Option
  _ORANGE = 0x0D, // Orange Color Option
}DfltColrByte;

/**
* BT Packet Fields Data
*/
typedef struct {
  // Byte 0 : header byte, for message type
  unsigned char byte0:8;
  // Byte 1 : LED Pattern
  unsigned char byte1:8;
  // Byte 2 : LED Brightness
  unsigned char byte2:8;
  // Byte 3 : LED Color
  unsigned char byte3:8;
} BtPktFlds;

/**
* BT Packet Data Structure
*/
typedef union {
  BtPktFlds flds;
  uint32_t  pyld;
} BtPkt;

BtPkt INC_BT_PKT; // Placeholder for storing the BT Message Packet

BtPkt OUT_BT_PKT = {_DFLT_BT_COLR, _SOLID, 0x64, _WHITE};

#endif