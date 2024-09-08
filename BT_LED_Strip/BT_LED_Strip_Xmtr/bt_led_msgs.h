#include <avr/pgmspace.h>
#include <stdint.h>
#ifndef __BT_LED_MSGs__
#define __BT_LED_MSGs__

#define CONN_REQ_SZ 20
// Dedicated BLE HM10 module
// change for your specific module
const char RCVR_CONN_REQ[CONN_REQ_SZ] PROGMEM = "AT+CON0035FF0D419B";
const unsigned long _bt_conn_timeout PROGMEM = 100000;
bool activeConnection = false;
unsigned long _last_bt_resp = 0;

typedef enum {
  _SOLID = 0x00,
  _PULSE = 0x01,
  _FADER = 0x02,
  _RNBOW = 0x03,
}Patterns;

typedef enum {
  _DFLT_BT_PING = 0x81,
  _DFLT_BT_RESP = 0x7E,
  _DFLT_BT_COLR = 0x65,
}HeadByte;

typedef enum {
  _OFF = 0x00,
  _WHITE = 0x01,
  _RED = 0x02,
  _ROSE = 0x03,
  _MAGENTA = 0x04,
  _VIOLET = 0x05,
  _BLUE = 0x06,
  _AZURE = 0x07,
  _CYAN = 0x08,
  _SPRING_GREEN = 0x09,
  _GREEN = 0x0A,
  _CHARTREUSE = 0x0B,
  _YELLOW = 0x0C,
  _ORANGE = 0x0D,
}DfltColrByte;


typedef union {
  unsigned char byte0:8;
  unsigned char byte1:8;
  unsigned char byte2:8;
  unsigned char byte3:8;
}BtPkt;

BtPkt *INC_BT_PKT;
BtPkt *OUT_BT_PKT;

#endif