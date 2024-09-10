#ifndef __BT_LED_MSGs__
#define __BT_LED_MSGs__

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

/**
* BT Packet Data Structure
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
}BtPkt;

BtPkt INC_BT_PKT = {_DFLT_BT_COLR, _SOLID, 0x14, _VIOLET};
#endif