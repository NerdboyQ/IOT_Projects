#ifndef __BT_LED_MSGs__
#define __BT_LED_MSGs__

enum Patterns {
  _SOLID = 0x00,
  _PULSE = 0x01,
  _FADER = 0x02,
  _RNBOW = 0x03,
};

enum HeadByte {
  _DFLT_BT_PING = 0x81,
  _DFLT_BT_RESP = 0x7E,
  _DFLT_BT_COLR = 0x65,
};

enum DfltColrByte {
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
};

struct BtPkt {
  unsigned char byte0;
  unsigned char byte1;
  unsigned char byte2;
  unsigned char byte3;
};
#endif