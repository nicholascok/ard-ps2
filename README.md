# Arduino PS/2

### Usage

A PS/2 keyboard input and a keyboard outputs can be initialised as follows:
```
struct ps2_device device_A = ps2_init_device(DEV1_DATA_PIN, DEV1_CLOCK_PIN);
struct ps2_device device_B = ps2_init_device(DEV2_DATA_PIN, DEV2_CLOCK_PIN);
ps2_init_host(HOST_DATA_PIN, HOST_CLOCK_PIN); /* for now only one input is possible */
```

### Functions

**Note:** In the following descriptions, *endpoint* refers to any device that is interpreting data output from the arduino, and *keyboard* refers to any PS/2 keyboard input device.

| FUNCTION | DESCRIPTION |
| -------- | ----------- |
| `int hread(BYTE* <B>)` | reads a byte of data from a keyboard |
| `int hwrite(BYTE <B>)` | writes a byte of data to a keyboard |
| `int dwrite(struct ps2_device <DEV>, BYTE <B>)` | writes a byte of data to an endpoint |
| `int dsend_key_make(struct ps2_device <DEV>, int <KEY>)` | sends a key's make code to an endpoint |
| `int dsend_key_break(struct ps2_device <DEV>, int <KEY>)` | sends a key's break code to an endpoint |
| `int dsend_key_press(struct ps2_device <DEV>, int <KEY>, int <MILLIS>)` | triggers a key press event (make, delay, break) on an endpoint |
| `int init_keyboard(struct ps2_device <DEV>)` | signals a BAT pass to an endpoint (necessary within 500ms after poweron) |
| `bool probe_status(void)` | requests a keyboard readminister its BAT and returns true if successful |
| `bool hsend_echo(void)` | sends an echo request to a keyboard and returns true if it responds |
  
The value of `<KEY>` can be any of the following:  
  
| K         | E         | Y         | S         |
| --------- | --------- | --------- | --------- |
| KEY_ESC   | KEY_6     | KEY_CAPLK | KEY_M     |
| KEY_F1    | KEY_7     | KEY_A     | KEY_COMMA |
| KEY_F2    | KEY_8     | KEY_S     | KEY_DOT   |
| KEY_F3    | KEY_9     | KEY_D     | KEY_FWSLH |
| KEY_F4    | KEY_0     | KEY_F     | KEY_RSHFT |
| KEY_F5    | KEY_BKSPC | KEY_G     | KEY_LCTRL |
| KEY_F6    | KEY_TAB   | KEY_H     | KEY_LALT  |
| KEY_F7    | KEY_Q     | KEY_J     | KEY_SPACE |
| KEY_F8    | KEY_W     | KEY_K     | KEY_NUMLK |
| KEY_F9    | KEY_E     | KEY_L     | KEY_KPMUL |
| KEY_F10   | KEY_R     | KEY_COLON | KEY_KPSUB |
| KEY_F11   | KEY_T     | KEY_QUOTE | KEY_KP7   |
| KEY_F12   | KEY_Y     | KEY_ENTER | KEY_KP8   |
| KEY_SCRLK | KEY_U     | KEY_LSHFT | KEY_KP9   |
| KEY_GRAVE | KEY_I     | KEY_Z     | KEY_KPADD |
| KEY_1     | KEY_O     | KEY_X     | KEY_KP4   |
| KEY_2     | KEY_P     | KEY_C     | KEY_KP5   |
| KEY_3     | KEY_LSQR  | KEY_V     | KEY_KP6   |
| KEY_4     | KEY_RSQR  | KEY_B     | KEY_KP1   |
| KEY_5     | KEY_BKSLH | KEY_N     | KEY_KP2   |
| KEY_BREAK | KEY_KP3   | KEY_KP0   | KEY_KPDOT |
| KEY_LWIN  | KEY_RALT  | KEY_RWIN  | KEY_MENUS |
| KEY_RCTRL | KEY_INS   | KEY_HOME  | KEY_PGUP  |
| KEY_DEL   | KEY_END   | KEY_PGDN  | KEY_UP    |
| KEY_LEFT  | KEY_DOWN  | KEY_RIGHT | KEY_KPDIV |
| KEY_PRINT |           |           |           |
