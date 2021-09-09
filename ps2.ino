#define P_CLK 3
#define P_DTA 2

typedef unsigned char BYTE;
typedef unsigned long time_t;

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
  KEY_BCKSP = 0x66,
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

// keys with fucked uP scancodes
enum {
  KEY_PRINT = 0xF0,
  KEY_BREAK = 0xF1
};

#define SEND_LOW() {dsp(P_DTA); delayMicroseconds(CLK_HAF_TIME); dsp(P_CLK); delayMicroseconds(CLK_FUL_TIME); enp(P_CLK); delayMicroseconds(CLK_HAF_TIME);}
#define SEND_HIGH() {enp(P_DTA); delayMicroseconds(CLK_HAF_TIME); dsp(P_CLK); delayMicroseconds(CLK_FUL_TIME); enp(P_CLK); delayMicroseconds(CLK_HAF_TIME);}

#define CLK_FUL_TIME 40
#define CLK_HAF_TIME 20

#define WRITE_DELAY 60

#define TIMEOUT 500

time_t write_last;

void enp(int _p) {
  pinMode(_p, INPUT);
  digitalWrite(_p, HIGH);
}

void dsp(int _p) {
  //pinMode(_p, INPUT);
  digitalWrite(_p, LOW);
  pinMode(_p, OUTPUT);
}

int write(BYTE _b) {
  // debug
  Serial.print("writing data: 0x");
  Serial.println(_b, HEX);
  
  // ensure sufficient time has elapsed in order to begin writing again
  delayMicroseconds(WRITE_DELAY);
  
  // clock line is low if host is inhibiting transfer
  if (digitalRead(P_CLK) == LOW) return -1;

  // if data line is low and clock is high the host is requesting to send data
  if (digitalRead(P_DTA) == LOW) return -1;
  
  bool parity =
    _b & 0x80 + _b & 0x40 + _b & 0x20 +
    _b & 0x10 + _b & 0x08 + _b & 0x04 +
    _b & 0x02 + _b & 0x01;

  // send start bit
  SEND_LOW()
  
  // send data bits
  for (int i = 0; i < 8; i++) {
    if (_b & (0x01 << i)) SEND_HIGH()
    else SEND_LOW()
  }

  // send parity bit
  if (parity) SEND_HIGH()
  else SEND_LOW()
  
  // send stop bit
  SEND_HIGH()
  
  return 0;
}

void init_keyboard(void) {
  // do not send device id - this tells the host this is an old AT keyboard
  // with uni-directional communication. This is necessary becasue recieveing
  // data from the host is not yet supported
  return;
}

void init_mouse(void) {
  // send device id as mouse with scroll wheel (device id 0x03)
  while(write(0x03));
  return;
}

int send_key_make(int _k) {
  signed char wrstat;
  
  Serial.print("sending key");
  Serial.print(_k);
  Serial.println(" make");
  
  switch (_k) {
    case KEY_DEL: case KEY_RALT: case KEY_KPDIV:
    case KEY_MENUS: case KEY_END: case KEY_PGDN:
    case KEY_HOME: case KEY_LWIN: case KEY_PGUP:
    case KEY_RIGHT: case KEY_RCTRL: case KEY_UP:
    case KEY_RWIN: case KEY_LEFT: case KEY_DOWN:
    case KEY_INS:
      // write extended prefix byte
      wrstat = write(0xE0);
      
      // verify write and send key code
      if (!wrstat) write(_k); else return -1;
      
      // verify write
      if (wrstat) return -1;
      break;
    case KEY_PRINT:
      // write extended prefix byte
      wrstat = write(0xE0);
      
      // verify write and send key code (part I)
      if (!wrstat) write(0x12); else return -1;
      
      // verify write and send extended prefix byte
      if (!wrstat) write(0xE0); else return -1;
      
      // verify write and send key code (part II)
      if (!wrstat) write(0x7C); else return -1;
      
      // verify write
      if (wrstat) return -1;
      break;
    case KEY_BREAK:
      // god knows
      wrstat = write(0xE1);
      
      // verify writes and send more
      if (!wrstat) write(0x14); else return -1;
      if (!wrstat) write(0x77); else return -1;
      if (!wrstat) write(0xE1); else return -1;
      if (!wrstat) write(0xF0); else return -1;
      if (!wrstat) write(0x14); else return -1;
      if (!wrstat) write(0xE0); else return -1;
      if (!wrstat) write(0x77); else return -1;
      
      // verify write
      if (wrstat) return -1;
      break;
    default:
        // send key code
        wrstat = write(_k);
        
        // verify write
        if (wrstat) return -1;
      break;
  }

  return 0;
}

int send_key_break(int _k) {
  int wrstat;
  
  // debug
  Serial.print("sending key");
  Serial.print(_k);
  Serial.println(" break");
  
  switch (_k) {
    case KEY_DEL: case KEY_RALT: case KEY_KPDIV:
    case KEY_MENUS: case KEY_END: case KEY_PGDN:
    case KEY_HOME: case KEY_LWIN: case KEY_PGUP:
    case KEY_RIGHT: case KEY_RCTRL: case KEY_UP:
    case KEY_RWIN: case KEY_LEFT: case KEY_DOWN:
    case KEY_INS:
      // write extended prefix byte
      wrstat = write(0xE0);
      
      // verify write and send break prefix byte
      if (!wrstat) write(0xF0); else return -1;
      
      // verify write and send key code
      if (!wrstat) write(_k); else return -1;
      
      // verify write
      if (wrstat) return -1;
      break;
    case KEY_PRINT:
      // write extended prefix byte
      wrstat = write(0xE0);
      
      // verify write and send break prefix byte
      if (!wrstat) write(0xF0); else return -1;
      
      // verify write and send key code (part I)
      if (!wrstat) write(0x73); else return -1;
      
      // verify write and send extended prefix byte
      if (!wrstat) write(0xE0); else return -1;
      
      // verify write and send break prefix byte
      if (!wrstat) write(0xF0); else return -1;
      
      // verify write and send key code (part II)
      if (!wrstat) write(0x12); else return -1;
      
      // verify write
      if (wrstat) return -1;
      break;
    default:
      // write break prefix byte
      wrstat = write(0xF0);
      
      // verify write and send key code
      if (!wrstat) write(_k); else return -1;
      
      // verify write
      if (wrstat) return -1;
      break;
  }

  return 0;
}

int send_key_press(int _k, int _t) {
  int wrstat;
  
  wrstat = send_key_make(_k);
  if (wrstat) return -3;
  
  delay(_t);
  
  wrstat = send_key_break(_k);
  if (wrstat) return -4;

  return 0;
}

void setup() {
  Serial.begin(9600);
  pinMode(P_DTA, OUTPUT);
  pinMode(P_CLK, OUTPUT);

  digitalWrite(P_CLK, HIGH);
  digitalWrite(P_DTA, HIGH);

  // set initial write time
  write_last = micros();
  
  delay(500); // dealy for BAT (Basic Assurance Test) - not needed here
  while (write(0xAA)); // send BAT complete signal to host

  init_keyboard();
  
  delay(TIMEOUT);
  
  while (send_key_press(KEY_F, 50));
  delay(90);
  while (send_key_press(KEY_O, 50));
  delay(90);
  while (send_key_press(KEY_X, 50));
  delay(90);
  while (send_key_press(KEY_LWIN, 50));
  
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
}
