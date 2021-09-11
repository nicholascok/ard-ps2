#include <LiquidCrystal.h>

#define P_CLK 3
#define P_DTA 2

#define SEND_LOW() {dsp(P_DTA); delayMicroseconds(CLK_HAF_TIME); dsp(P_CLK); delayMicroseconds(CLK_FUL_TIME); enp(P_CLK); delayMicroseconds(CLK_HAF_TIME);}
#define SEND_HIGH() {enp(P_DTA); delayMicroseconds(CLK_HAF_TIME); dsp(P_CLK); delayMicroseconds(CLK_FUL_TIME); enp(P_CLK); delayMicroseconds(CLK_HAF_TIME);}

#define TIMEOUT(EXPRESSION, TIME, CODE) since = millis(); while (EXPRESSION) if (millis() - since > TIME) {CODE};

#define WAIT_HIGH(PIN) {since = millis(); while (digitalRead(PIN) != HIGH) if (millis() - since > WR_TIMEOUT) return -1;} /* timeout */
#define WAIT_LOW(PIN) {since = millis(); while (digitalRead(PIN) != LOW) if (millis() - since > WR_TIMEOUT) return -1;} /* timeout */
#define WAIT_FOR_NEXT_LOW() WAIT_HIGH(P_CLK) WAIT_LOW(P_CLK)

#define CLK_FUL_TIME 40
#define CLK_HAF_TIME 20

#define WRITE_DELAY 60

#define WR_TIMEOUT 20
#define RD_TIMEOUT 20

typedef unsigned char BYTE;
typedef unsigned long time_t;

// init LiquicCrystal
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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

// keys with fucked uP scancodes
enum {
  KEY_PRINT = 0xF0,
  KEY_BREAK = 0xF1
};

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

int write_as_device(BYTE _b) {
  // debug
  Serial.print("writing data: 0x");
  Serial.println(_b, HEX);
  
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

void init_keyboard(void) {
  // do not send device id - this tells the host this is an old AT keyboard
  // with uni-directional communication. This is necessary becasue recieveing
  // data from the host is not yet supported
  return;
}

void init_mouse(void) {
  // send device id as mouse with scroll wheel (device id 0x03)
  while(write_as_device(0x03));
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
      wrstat = write_as_device(0xE0);
      
      // verify write and send key code
      if (!wrstat) write_as_device(_k); else return -1;
      
      // verify write
      if (wrstat) return -1;
      break;
    case KEY_PRINT:
      // write extended prefix byte
      wrstat = write_as_device(0xE0);
      
      // verify write and send key code (part I)
      if (!wrstat) write_as_device(0x12); else return -1;
      
      // verify write and send extended prefix byte
      if (!wrstat) write_as_device(0xE0); else return -1;
      
      // verify write and send key code (part II)
      if (!wrstat) write_as_device(0x7C); else return -1;
      
      // verify write
      if (wrstat) return -1;
      break;
    case KEY_BREAK:
      // god knows
      wrstat = write_as_device(0xE1);
      
      // verify writes and send more
      if (!wrstat) write_as_device(0x14); else return -1;
      if (!wrstat) write_as_device(0x77); else return -1;
      if (!wrstat) write_as_device(0xE1); else return -1;
      if (!wrstat) write_as_device(0xF0); else return -1;
      if (!wrstat) write_as_device(0x14); else return -1;
      if (!wrstat) write_as_device(0xE0); else return -1;
      if (!wrstat) write_as_device(0x77); else return -1;
      
      // verify write
      if (wrstat) return -1;
      break;
    default:
        // send key code
        wrstat = write_as_device(_k);
        
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
      wrstat = write_as_device(0xE0);
      
      // verify write and send break prefix byte
      if (!wrstat) write_as_device(0xF0); else return -1;
      
      // verify write and send key code
      if (!wrstat) write_as_device(_k); else return -1;
      
      // verify write
      if (wrstat) return -1;
      break;
    case KEY_PRINT:
      // write extended prefix byte
      wrstat = write_as_device(0xE0);
      
      // verify write and send break prefix byte
      if (!wrstat) write_as_device(0xF0); else return -1;
      
      // verify write and send key code (part I)
      if (!wrstat) write_as_device(0x73); else return -1;
      
      // verify write and send extended prefix byte
      if (!wrstat) write_as_device(0xE0); else return -1;
      
      // verify write and send break prefix byte
      if (!wrstat) write_as_device(0xF0); else return -1;
      
      // verify write and HIGHsend key code (part II)
      if (!wrstat) write_as_device(0x12); else return -1;
      
      // verify write
      if (wrstat) return -1;
      break;
    default:
      // write break prefix byte
      wrstat = write_as_device(0xF0);
      
      // verify write and send key code
      if (!wrstat) write_as_device(_k); else return -1;
      
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

int read_as_host(BYTE* _b) {
  pinMode(P_CLK, INPUT_PULLUP);
  pinMode(P_CLK, INPUT_PULLUP);
  
  time_t since;
  bool parity = 1;
  BYTE b = 0;
  
  WAIT_LOW(P_CLK)
  
  if (digitalRead(P_DTA) != LOW) return -2; // invalid start bit recieved
  
  for (int state, i = 0; i < 8; i++) {
    WAIT_FOR_NEXT_LOW()
    state = digitalRead(P_DTA);
    
    if (state == HIGH)
      parity = !parity,
      b |= 0x01 << i;
  }
  
  WAIT_FOR_NEXT_LOW()
  
  if (digitalRead(P_DTA) != parity) return -4; // invalid parity bit recieved
  
  WAIT_FOR_NEXT_LOW()
  
  if (digitalRead(P_DTA) != HIGH) return -5; // invalid stop bit recieved
  
  *_b = b;
  
  return 0;
}

int write_as_host(BYTE _b) {
  bool parity = (
    _b & 0x80 + _b & 0x40 + _b & 0x20 +
    _b & 0x10 + _b & 0x08 + _b & 0x04 +
    _b & 0x02 + _b & 0x01) % 2;
  
  time_t since;
  
  pinMode(P_CLK, OUTPUT);
  pinMode(P_DTA, OUTPUT);
  
  // pull clock low for 100us (request to send)
  digitalWrite(P_CLK, LOW);
  delayMicroseconds(100);
  
  // pull data low and release clock
  digitalWrite(P_DTA, LOW);
  digitalWrite(P_CLK, HIGH);
  
  pinMode(P_CLK, INPUT_PULLUP);
  
  // wait for device to pull clock low (acknowledgement)
  WAIT_LOW(P_CLK)
  
  // write data bits
  for (int i = 0; i < 8; i++) {
    if (_b & (0x01 << i)) digitalWrite(P_DTA, HIGH);
    else digitalWrite(P_DTA, LOW);
    WAIT_FOR_NEXT_LOW()
  }
  
  // write parity bit
  if (parity) digitalWrite(P_DTA, HIGH);
  else digitalWrite(P_DTA, HIGH);
  WAIT_FOR_NEXT_LOW()
  
  // release data line
  digitalWrite(P_DTA, LOW);
  
  pinMode(P_DTA, INPUT_PULLUP);
  
  // wait for device to pull clock and data low
  WAIT_LOW(P_DTA)
  WAIT_LOW(P_CLK)
  
  // wait for device to release clock and data (return to idle state)
  WAIT_HIGH(P_DTA)
  WAIT_HIGH(P_CLK)

  return 0;
}

int lcd_cursor_pos = 0;
int lcd_line = 0;

void setup() {
  Serial.begin(9600);

  // init LiquidCrystal
  lcd.begin(8, 2);
  lcd.cursor();
  
  pinMode(P_DTA, INPUT_PULLUP);
  pinMode(P_CLK, INPUT_PULLUP);
  
  BYTE b = 0;
  int err_count = -1;
  time_t since;
  
  // look for BAT result from device
  Serial.println("checking result of BAT");
  
  BAT_CHECK:
  err_count++;
  
  Serial.print("NOTICE: attempt ");
  Serial.print(err_count, DEC);
  Serial.println(" out of 10.");
  
  if (err_count >= 10) {
    Serial.println("failed to recieve response from device.");
    Serial.println("attempting to continue anyway...");
    return;
  }
  
  TIMEOUT(read_as_host(&b), 1000,
    Serial.println("recieved nothing: attempting reset...");
    delay(500);
    
    TIMEOUT(write_as_host(0xFF), 1000,
      Serial.println("write timed out: attempting to continue...");
      goto BAT_CHECK_EXIT;
    )
    
    Serial.println("reset request sent: looking for acknowledgement...");
    
    TIMEOUT(read_as_host(&b), 1000,
      Serial.println("recieved nothing: attempting to continue...");
      goto BAT_CHECK_EXIT;
    )
    
    Serial.println("acknowledged...");
    
    goto BAT_CHECK;
  )
  
  Serial.print("recieved byte ");
  Serial.print(b, HEX);
  Serial.println(".");
  
  BAT_CHECK_EXIT:
  
  delay(500);
  
  switch (b) {
    case 0xAA:
      Serial.println("BAT successful!");
      break;
    case 0xFC:
    case 0xFD:
      Serial.println("BAT failed: retrying...");
      goto BAT_CHECK;
      break;
    case 0xFE:
      Serial.println("device requested resend: resetting...");
      goto BAT_CHECK;
      break;
    default:
      Serial.println("invalid response from device: resetting...");
      goto BAT_CHECK;
      break;
  }
  
  err_count = 0;
  
  // test connection
  Serial.println("sending echo request");
  
  RESPONSE_CHECK:

  TIMEOUT(write_as_host(0xEE), 1000,
    Serial.println("write timed out: retrying...");
    goto RESPONSE_CHECK;
  )
  
  TIMEOUT(read_as_host(&b), 1000,
    Serial.println("recieved nothing... ... retrying...");
    goto RESPONSE_CHECK;
  )
  
  Serial.print("got response ");
  Serial.println(b, HEX);
  
  switch (b) {
    case 0xEE:
      Serial.println("device responded successfully to echo request!");
      break;
    case 0xFE:
      err_count++;
      Serial.print("device requesting resend (");
      Serial.print(err_count, DEC);
      Serial.println(" / 10)");
      
      if (err_count >= 10) {
        Serial.println("device failed to respond to echo request.");
        Serial.println("NOTICE: device may not respond to host.");
        return;
      }
      
      delay(500);
      goto RESPONSE_CHECK;
    default:
      err_count++;
      Serial.print("device failed to respond (");
      Serial.print(err_count, DEC);
      Serial.println(" / 10)");
      
      if (err_count >= 10) {
        Serial.println("device failed to respond to echo request");
        Serial.println("NOTICE: device may not respond to host");
        return;
      }
      
      delay(500);
      goto RESPONSE_CHECK;
  }
}

char* print_key(int _k) {
  switch (_k) {
    case KEY_A:
      return "A";
      break;
    case KEY_B:
      return "B";
      break;
    case KEY_C:
      return "C";
      break;
    case KEY_D:
      return "D";
      break;
    case KEY_E:
      return "E";
      break;
    case KEY_F:
      return "F";
      break;
    case KEY_G:
      return "G";
      break;
    case KEY_H:
      return "H";
      break;
    case KEY_I:
      return "I";
      break;
    case KEY_J:
      return "J";
      break;
    case KEY_K:
      return "K";
      break;
    case KEY_L:
      return "L";
      break;
    case KEY_M:
      return "M";
      break;
    case KEY_N:
      return "N";
      break;
    case KEY_O:
      return "O";
      break;
    case KEY_P:
      return "P";
      break;
    case KEY_Q:
      return "Q";
      break;
    case KEY_R:
      return "R";
      break;
    case KEY_S:
      return "S";
      break;
    case KEY_T:
      return "T";
      break;
    case KEY_U:
      return "U";
      break;
    case KEY_V:
      return "V";
      break;
    case KEY_W:
      return "W";
      break;
    case KEY_X:
      return "X";
      break;
    case KEY_Y:
      return "Y";
      break;
    case KEY_Z:
      return "Z";
      break;
    case KEY_SPACE:
      return " ";
    case KEY_DOT:
      return ".";
      break;
    case KEY_COMMA:
      return ",";
    default:
      return "U";
      break;
  }
}

void loop() {
  BYTE b = 0;
  
  while (read_as_host(&b));
  
  switch (b) {
    case 0xF0:
      while (read_as_host(&b));
      Serial.print(print_key(b));
      Serial.println(" break");
      break;
    default:
      Serial.print(print_key(b));
      Serial.println(" make");
      if (b == KEY_BKSPC) {
        lcd_cursor_pos--;
        
        if (lcd_cursor_pos < 0) {
          lcd_cursor_pos = 0;
          if (--lcd_line < 0) lcd_line = 0;
          else lcd_cursor_pos = 15;
        }
        
        lcd.setCursor(lcd_cursor_pos, lcd_line);
        lcd.print(" ");
        lcd.setCursor(lcd_cursor_pos, lcd_line);
      } else {
        lcd.setCursor(lcd_cursor_pos, lcd_line);
        lcd.print(print_key(b));
        lcd_cursor_pos++;
      }

      if (lcd_cursor_pos > 15) {
        lcd_cursor_pos = 0;
        if (++lcd_line > 1)
          lcd_line = 0,
          lcd.clear();
      }

      break;
  }
  
  Serial.print("recieved ");
  Serial.println(b, HEX);
}
