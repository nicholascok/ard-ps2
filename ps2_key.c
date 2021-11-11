struct ps2_device
ps2_init_device (int _p_dta, int _p_clk) {
  struct ps2_device DEV = {_p_clk, _p_dta};
  
  pinMode(DEV.P_DTA, INPUT_PULLUP);
  pinMode(DEV.P_CLK, INPUT_PULLUP);
  
  return DEV;
}

void ps2_init_host(int _p_dta, int _p_clk) {
  H_CLK = _p_clk;
  H_DTA = _p_dta;
  
  pinMode(H_DTA, INPUT_PULLUP);
  pinMode(H_CLK, INPUT_PULLUP);
}

void enp(int _p) {
  pinMode(_p, INPUT_PULLUP);
  return;
}

void dsp(int _p) {
  digitalWrite(_p, LOW);
  pinMode(_p, OUTPUT);
  return;
}

int dwrite(struct ps2_device _dev, BYTE _b) {
  // ensure sufficient time has elapsed in order to begin writing again
  delayMicroseconds(WRITE_DELAY);
  
  // clock line is low if host is inhibiting transfer
  if (digitalRead(_dev.P_CLK) == LOW) return -1;

  // if data line is low and clock is high the host is requesting to send data
  if (digitalRead(_dev.P_DTA) == LOW) return -1;
  
  bool parity = (
    (_b & 0x80) + (_b & 0x40) + (_b & 0x20) +
    (_b & 0x10) + (_b & 0x08) + (_b & 0x04) +
    (_b & 0x02) + (_b & 0x01)) % 2;

  // send start bit
  SEND_LOW(_dev.P_DTA, _dev.P_CLK)
  
  // send data bits
  for (int i = 0; i < 8; i++) {
    if (_b & (0x01 << i)) SEND_HIGH(_dev.P_DTA, _dev.P_CLK)
    else SEND_LOW(_dev.P_DTA, _dev.P_CLK)
  }

  // send parity bit
  if (parity) SEND_LOW(_dev.P_DTA, _dev.P_CLK)
  else SEND_HIGH(_dev.P_DTA, _dev.P_CLK)
  
  // send stop bit
  SEND_HIGH(_dev.P_DTA, _dev.P_CLK)
  
  return 0;
}

int init_keyboard (struct ps2_device _dev) {
  delay(500); // delay for BAT (Basic Assurance Test)
  while (dwrite(_dev, 0xAA)); // send BAT complete signal to host
  
  // do not send device id - this tells the host this is an old AT keyboard
  // with uni-directional communication. This is necessary becasue recieveing
  // data from the host is not yet supported
  return 0;
}

int dsend_key_make(struct ps2_device _dev, int _k) {
  signed char stat;
  
  switch (_k) {
    case KEY_DEL: case KEY_RALT: case KEY_KPDIV:
    case KEY_MENUS: case KEY_END: case KEY_PGDN:
    case KEY_HOME: case KEY_LWIN: case KEY_PGUP:
    case KEY_RIGHT: case KEY_RCTRL: case KEY_UP:
    case KEY_RWIN: case KEY_LEFT: case KEY_DOWN:
    case KEY_INS:
      // write extended prefix byte
      stat = dwrite(_dev, 0xE0);
      
      // verify write and send key code
      if (!stat) dwrite(_dev, _k); else return -1;
      
      // verify write
      if (stat) return -1;
      break;
    case KEY_PRINT:
      // write extended prefix byte
      stat = dwrite(_dev, 0xE0);
      
      // verify write and send key code (part I)
      if (!stat) dwrite(_dev, 0x12); else return -1;
      
      // verify write and send extended prefix byte
      if (!stat) dwrite(_dev, 0xE0); else return -1;
      
      // verify write and send key code (part II)
      if (!stat) dwrite(_dev, 0x7C); else return -1;
      
      // verify write
      if (stat) return -1;
      break;
    case KEY_BREAK:
      // god knows
      stat = dwrite(_dev, 0xE1);
      
      // verify writes and send more
      if (!stat) dwrite(_dev, 0x14); else return -1;
      if (!stat) dwrite(_dev, 0x77); else return -1;
      if (!stat) dwrite(_dev, 0xE1); else return -1;
      if (!stat) dwrite(_dev, 0xF0); else return -1;
      if (!stat) dwrite(_dev, 0x14); else return -1;
      if (!stat) dwrite(_dev, 0xE0); else return -1;
      if (!stat) dwrite(_dev, 0x77); else return -1;
      
      // verify write
      if (stat) return -1;
      break;
    default:
        // send key code
        stat = dwrite(_dev, _k);
        
        // verify write
        if (stat) return -1;
      break;
  }

  return 0;
}

int dsend_key_break(struct ps2_device _dev, int _k) {
  int stat;
  
  switch (_k) {
    case KEY_DEL: case KEY_RALT: case KEY_KPDIV:
    case KEY_MENUS: case KEY_END: case KEY_PGDN:
    case KEY_HOME: case KEY_LWIN: case KEY_PGUP:
    case KEY_RIGHT: case KEY_RCTRL: case KEY_UP:
    case KEY_RWIN: case KEY_LEFT: case KEY_DOWN:
    case KEY_INS:
      // write extended prefix byte
      stat = dwrite(_dev, 0xE0);
      
      // verify write and send break prefix byte
      if (!stat) dwrite(_dev, 0xF0); else return -1;
      
      // verify write and send key code
      if (!stat) dwrite(_dev, _k); else return -1;
      
      // verify write
      if (stat) return -1;
      break;
    case KEY_PRINT:
      // write extended prefix byte
      stat = dwrite(_dev, 0xE0);
      
      // verify write and send break prefix byte
      if (!stat) dwrite(_dev, 0xF0); else return -1;
      
      // verify write and send key code (part I)
      if (!stat) dwrite(_dev, 0x73); else return -1;
      
      // verify write and send extended prefix byte
      if (!stat) dwrite(_dev, 0xE0); else return -1;
      
      // verify write and send break prefix byte
      if (!stat) dwrite(_dev, 0xF0); else return -1;
      
      // verify write and HIGHsend key code (part II)
      if (!stat) dwrite(_dev, 0x12); else return -1;
      
      // verify write
      if (stat) return -1;
      break;
    default:
      // write break prefix byte
      stat = dwrite(_dev, 0xF0);
      
      // verify write and send key code
      if (!stat) dwrite(_dev, _k); else return -1;
      
      // verify write
      if (stat) return -1;
      break;
  }

  return 0;
}

int dsend_key_press(struct ps2_device _dev, int _k, int _t) {
  int stat;
  
  stat = dsend_key_make(_dev, _k);
  if (stat) return -1;
  
  delay(_t);
  
  stat = dsend_key_break(_dev, _k);
  if (stat) return -1;

  return 0;
}

/* read data from periphreal, follows the following
 * proceedure:
 * 1) wait for clock LOW, or timeout after RD_TIMEOUT
 *    ms (wait for start bit).
 * 2) if (1) succeeds then wait for next time clock is
 *    LOW (host reads on clock LOW).
 * 3) read data line and set corresponding bit.
 * 4) wait for next time clock is HIGH.
 * 5) repeat 2-4 untill all eight data bits and parity
 *    bit is recieved.
 * 6) wait until clokc is LOW again and check that the
 *    data line is HIGH (stop bit) */
int hread(BYTE* _b) {
  pinMode(H_CLK, INPUT_PULLUP);
  pinMode(H_CLK, INPUT_PULLUP);
  
  time_t since;
  BYTE parity = 1;
  BYTE b = 0;
  
  // wait for start bit
  WAIT_LOW(H_CLK, RD_TIMEOUT)
  
  if (digitalRead(H_DTA) != LOW) return -2; // invalid start bit recieved
  
  // read data bits
  for (int state, i = 0; i < 8; i++) {
    WAIT_FOR_NEXT_LOW(CLK_TIMEOUT, H_DTA, H_CLK)
    state = digitalRead(H_DTA);
    
    if (state == HIGH)
      parity = !parity,
      b |= 0x01 << i;
  }
  
  // read parity bit
  WAIT_FOR_NEXT_LOW(CLK_TIMEOUT, H_DTA, H_CLK)
  
  // check parity
  if (digitalRead(H_DTA) != parity) return -4; // invalid parity bit recieved
  
  // read stop bit
  WAIT_FOR_NEXT_LOW(CLK_TIMEOUT, H_DTA, H_CLK)
  
  if (digitalRead(H_DTA) != HIGH) return -5; // invalid stop bit recieved
  
  *_b = b;
  
  return 0;
}

/* write data to peripheral, folows the following
 * proceedure:
 * 1) pull clock line for 100us (request to send).
 * 2) pull data low and release clock line.
 * 3) wait for device to acknowledge the request by
 *    pulling clock low (this acts as start bit).
 * 4) write data bits when clock is LOW. NOTE:
 *    bits are read by device when clock is HIGH
 *    and clock is generated by device.
 * 5) write parity bit, as before
 * 6) wait for device to pull clock and data low,
 *    then release clock and data (this acts as
 *    stop bit). */
int hwrite(BYTE _b) {
  BYTE parity = (
    (_b & 0x80) + (_b & 0x40) + (_b & 0x20) +
    (_b & 0x10) + (_b & 0x08) + (_b & 0x04) +
    (_b & 0x02) + (_b & 0x01)) % 2;
  
  time_t since;
  
  pinMode(H_CLK, OUTPUT);
  pinMode(H_DTA, OUTPUT);
  
  // pull clock low for 100us
  digitalWrite(H_CLK, LOW);
  delayMicroseconds(100);
  
  // pull data low and release clock
  digitalWrite(H_DTA, LOW);
  digitalWrite(H_CLK, HIGH);
  
  pinMode(H_CLK, INPUT_PULLUP);
  
  // wait for device to pull clock low
  WAIT_LOW(H_CLK, WR_TIMEOUT)
  
  // write data bits
  for (int i = 0; i < 8; i++) {
    if (_b & (0x01 << i)) digitalWrite(H_DTA, HIGH);
    else digitalWrite(H_DTA, LOW);
    WAIT_FOR_NEXT_LOW(CLK_TIMEOUT, H_DTA, H_CLK)
  }
  
  // write parity bit
  if (parity) digitalWrite(H_DTA, HIGH);
  else digitalWrite(H_DTA, HIGH);
  WAIT_FOR_NEXT_LOW(CLK_TIMEOUT, H_DTA, H_CLK)
  
  // release data line
  digitalWrite(H_DTA, LOW);
  
  pinMode(H_DTA, INPUT_PULLUP);
  
  // wait for device to pull clock and data low
  WAIT_LOW(H_DTA, WR_TIMEOUT)
  WAIT_LOW(H_CLK, WR_TIMEOUT)
  
  // wait for device to release clock and data (return to idle state)
  WAIT_HIGH(H_DTA, WR_TIMEOUT)
  WAIT_HIGH(H_CLK, WR_TIMEOUT)

  return 0;
}

bool probe_status(void) {
  time_t since;
  BYTE b = 0;
  
  // send reset request
  TIMEOUT(hwrite(0xFF), 1000, return 0; /* write timeout */)
  
  // look for acknowledgement
  TIMEOUT(hread(&b), 1000, return 0; /* read timeout */)
  
  if (b != 0xFA) return 0;
  
  // read response
  TIMEOUT(hread(&b), 1000, return 0; /* read timeout */)
  
  if (b != 0xAA) return 0;
  return 1;
}

bool hsend_echo(void) {
  time_t since;
  BYTE b = 0;
  
  // send echo request
  TIMEOUT(hwrite(0xEE), 1000, return 0; /* write timeout */)
  
  // look for response
  TIMEOUT(hread(&b), 1000, return 0; /* read timeout */)
  
  if (b != 0xEE) return 0;
  return 1;
}