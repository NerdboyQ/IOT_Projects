#ifndef __BT_LED_MSGs__
#define __BT_LED_MSGs__

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

BtPkt INC_BT_PKT = {_DFLT_BT_COLR, _SOLID, 0x14, _VIOLET}; // Placeholder for incoming BT Message Packet
#endif