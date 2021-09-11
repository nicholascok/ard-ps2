//////////////////////////////////////////////////////
// set of functions for transmitting PS/2 data from //
// an arduino.                                      //
//////////////////////////////////////////////////////

#include <Arduino.h>
#include "ps2_device.h"

void enp(int _p) {
  pinMode(_p, INPUT_PULLUP);
  return;
}

void dsp(int _p) {
  digitalWrite(_p, LOW);
  pinMode(_p, OUTPUT);
  return;
}

int write(BYTE _b) {
  // ensure sufficient time has elapsed in order to begin writing again
  delayMicroseconds(WRITE_DELAY);
  
  // clock line is low if host is inhibiting transfer
  if (digitalRead(P_CLK) == LOW) return -1;

  // if data line is low and clock is high the host is requesting to send data
  if (digitalRead(P_DTA) == LOW) return -1;
  
  bool parity = (
    _b & 0x80 + _b & 0x40 + _b & 0x20 +
    _b & 0x10 + _b & 0x08 + _b & 0x04 +
    _b & 0x02 + _b & 0x01) % 2;

  // send start bit
  SEND_LOW()
  
  // send data bits
  for (int i = 0; i < 8; i++) {
    if (_b & (0x01 << i)) SEND_HIGH()
    else SEND_LOW()
  }

  // send parity bit
  if (parity) SEND_LOW()
  else SEND_HIGH()
  
  // send stop bit
  SEND_HIGH()
  
  return 0;
}

int init_keyboard(void) {
  // do not send device id - this tells the host this is an old AT keyboard
  // with uni-directional communication. This is necessary becasue recieveing
  // data from the host is not yet supported
  return 0;
}

int init_mouse(void) {
  time_t since;
  
  // send device id as mouse with scroll wheel (device id 0x03)
  TIMEOUT(write(0x03), 1000, return -1; /* write timeout */);
  return 0;
}

int send_key_make(int _k) {
  signed char stat;
  
  switch (_k) {
    case KEY_DEL: case KEY_RALT: case KEY_KPDIV:
    case KEY_MENUS: case KEY_END: case KEY_PGDN:
    case KEY_HOME: case KEY_LWIN: case KEY_PGUP:
    case KEY_RIGHT: case KEY_RCTRL: case KEY_UP:
    case KEY_RWIN: case KEY_LEFT: case KEY_DOWN:
    case KEY_INS:
      // write extended prefix byte
      stat = write(0xE0);
      
      // verify write and send key code
      if (!stat) write(_k); else return -1;
      
      // verify write
      if (stat) return -1;
      break;
    case KEY_PRINT:
      // write extended prefix byte
      stat = write(0xE0);
      
      // verify write and send key code (part I)
      if (!stat) write(0x12); else return -1;
      
      // verify write and send extended prefix byte
      if (!stat) write(0xE0); else return -1;
      
      // verify write and send key code (part II)
      if (!stat) write(0x7C); else return -1;
      
      // verify write
      if (stat) return -1;
      break;
    case KEY_BREAK:
      // god knows
      stat = write(0xE1);
      
      // verify writes and send more
      if (!stat) write(0x14); else return -1;
      if (!stat) write(0x77); else return -1;
      if (!stat) write(0xE1); else return -1;
      if (!stat) write(0xF0); else return -1;
      if (!stat) write(0x14); else return -1;
      if (!stat) write(0xE0); else return -1;
      if (!stat) write(0x77); else return -1;
      
      // verify write
      if (stat) return -1;
      break;
    default:
        // send key code
        stat = write(_k);
        
        // verify write
        if (stat) return -1;
      break;
  }

  return 0;
}

int send_key_break(int _k) {
  int stat;
  
  switch (_k) {
    case KEY_DEL: case KEY_RALT: case KEY_KPDIV:
    case KEY_MENUS: case KEY_END: case KEY_PGDN:
    case KEY_HOME: case KEY_LWIN: case KEY_PGUP:
    case KEY_RIGHT: case KEY_RCTRL: case KEY_UP:
    case KEY_RWIN: case KEY_LEFT: case KEY_DOWN:
    case KEY_INS:
      // write extended prefix byte
      stat = write(0xE0);
      
      // verify write and send break prefix byte
      if (!stat) write(0xF0); else return -1;
      
      // verify write and send key code
      if (!stat) write(_k); else return -1;
      
      // verify write
      if (stat) return -1;
      break;
    case KEY_PRINT:
      // write extended prefix byte
      stat = write(0xE0);
      
      // verify write and send break prefix byte
      if (!stat) write(0xF0); else return -1;
      
      // verify write and send key code (part I)
      if (!stat) write(0x73); else return -1;
      
      // verify write and send extended prefix byte
      if (!stat) write(0xE0); else return -1;
      
      // verify write and send break prefix byte
      if (!stat) write(0xF0); else return -1;
      
      // verify write and HIGHsend key code (part II)
      if (!stat) write(0x12); else return -1;
      
      // verify write
      if (stat) return -1;
      break;
    default:
      // write break prefix byte
      stat = write(0xF0);
      
      // verify write and send key code
      if (!stat) write(_k); else return -1;
      
      // verify write
      if (stat) return -1;
      break;
  }

  return 0;
}

int send_key_press(int _k, int _t) {
  int stat;
  
  stat = send_key_make(_k);
  if (stat) return -1;
  
  delay(_t);
  
  stat = send_key_break(_k);
  if (stat) return -1;

  return 0;
}
