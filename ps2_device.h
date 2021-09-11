#ifndef __ARD_PS2_DEVICE_H__
#define __ARD_PS2_DEVICE_H__

int P_CLK;
int P_DTA;

typedef unsigned char BYTE;
typedef unsigned long time_t;

#define WRITE_DELAY 60

#define CLK_FUL_TIME 40
#define CLK_HAF_TIME 20

/* run EXPRESSION until 0 is returned, or run CODE
 * on timeout after TIME ms. NOTE: variable 'since'
 * of type time_t must be defined.*/
#define TIMEOUT(EXPRESSION, TIME, CODE)\
  since = millis();\
  while (EXPRESSION)\
    if (millis() - since > TIME) {\
      CODE\
    }

#define SEND_LOW() {\
  dsp(P_DTA);\
  delayMicroseconds(CLK_HAF_TIME);\
  dsp(P_CLK); delayMicroseconds(CLK_FUL_TIME);\
  enp(P_CLK); delayMicroseconds(CLK_HAF_TIME);\
}

#define SEND_HIGH() {\
  enp(P_DTA);\
  delayMicroseconds(CLK_HAF_TIME);\
  dsp(P_CLK);\
  delayMicroseconds(CLK_FUL_TIME);\
  enp(P_CLK);\
  delayMicroseconds(CLK_HAF_TIME);\
}

// keys with single-byte scancodes
enum {
  KEY_ESC   = 0x76,
  KEY_F1    = 0x05,
  KEY_F2    = 0x06,
  KEY_F3    = 0x04,
  KEY_F4    = 0x0C,
  key_F5    = 0x03,
  KEY_F6    = 0x0B,
  KEY_F7    = 0x83,
  KEY_F8    = 0x0A,
  KEY_F9    = 0x01,
  KEY_F10   = 0x09,
  KEY_F11   = 0x78,
  KEY_F12   = 0x07,
  KEY_SCRLK = 0x7E,
  KEY_GRAVE = 0x0E,
  KEY_1     = 0x16,
  KEY_2     = 0x1E,
  KEY_3     = 0x26,
  KEY_4     = 0x25,
  KEY_5     = 0x2E,
  KEY_6     = 0x36,
  KEY_7     = 0x3D,
  KEY_8     = 0x3E,
  KEY_9     = 0x46,
  KEY_0     = 0x45,
  KEY_BKSPC = 0x66,
  KEY_TAB   = 0x0D,
  KEY_Q     = 0x15,
  KEY_W     = 0x1D,
  KEY_E     = 0x24,
  KEY_R     = 0x2D,
  KEY_T     = 0x2C,
  KEY_Y     = 0x35,
  KEY_U     = 0x3C,
  KEY_I     = 0x43,
  KEY_O     = 0x44,
  KEY_P     = 0x4D,
  KEY_LSQR  = 0x54,
  KEY_RSQR  = 0x5B,
  KEY_BKSLH = 0x5D,
  KEY_CAPLK = 0x58,
  KEY_A     = 0x1C,
  KEY_S     = 0x1B,
  KEY_D     = 0x23,
  KEY_F     = 0x2B,
  KEY_G     = 0x34,
  KEY_H     = 0x33,
  KEY_J     = 0x3B,
  KEY_K     = 0x42,
  KEY_L     = 0x4B,
  KEY_COLON = 0x4C,
  KEY_QUOTE = 0x52,
  KEY_ENTER = 0x5A,
  KEY_LSHFT = 0x12,
  KEY_Z     = 0x1A,
  KEY_X     = 0x22,
  KEY_C     = 0x21,
  KEY_V     = 0x2A,
  KEY_B     = 0x32,
  KEY_N     = 0x31,
  KEY_M     = 0x3A,
  KEY_COMMA = 0x41,
  KEY_DOT   = 0x49,
  KEY_FWSLH = 0x4A,
  KEY_RSHFT = 0x59,
  KEY_LCTRL = 0x14,
  KEY_LALT  = 0x11,
  KEY_SPACE = 0x29,
  KEY_NUMLK = 0x77,
  KEY_KPMUL = 0x7C,
  KEY_KPSUB = 0x7B,
  KEY_KP7   = 0x6C,
  KEY_KP8   = 0x75,
  KEY_KP9   = 0x7D,
  KEY_KPADD = 0x79,
  KEY_KP4   = 0x6B,
  KEY_KP5   = 0x73,
  KEY_KP6   = 0x74,
  KEY_KP1   = 0x69,
  KEY_KP2   = 0x72,
  KEY_KP3   = 0x7A,
  KEY_KP0   = 0x70,
  KEY_KPDOT = 0x71
};

// keys with double-byte scancodes
enum {
  KEY_LWIN  = 0x1F,
  KEY_RALT  = 0x11,
  KEY_RWIN  = 0x27,
  KEY_MENUS = 0x2F,
  KEY_RCTRL = 0x14,
  KEY_INS   = 0x70,
  KEY_HOME  = 0x6C,
  KEY_PGUP  = 0x7D,
  KEY_DEL   = 0x71,
  KEY_END   = 0x69,
  KEY_PGDN  = 0x7A,
  KEY_UP    = 0x75,
  KEY_LEFT  = 0x6B,
  KEY_DOWN  = 0x72,
  KEY_RIGHT = 0x74,
  KEY_KPDIV = 0x4A
};

// keys with fucked up scancodes
enum {
  KEY_PRINT = 0xF0,
  KEY_BREAK = 0xF1
};

#endif
