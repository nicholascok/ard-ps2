# Arduino PS/2
### Usage
A PS/2 keyboard can be initialised with `struct ps2_device <my_device_name> = ps2_init_device(<data pin>, <clock pin>)`, and the arduino can be initialised as a host with `ps2_init_host(<data pin>, <clock pin>)`.  
  
For brevity's sake, here is a list of functions:  
  
**Note:** In the following descriptions, *device* refers to a simulated keyboard (PS/2 output from the arduino), and *keyboard* refers to the physical keyboard being read by the arduino.

| FUNCTION | DESCRIPTION |
| -------- | ----------- |
| `int hread(BYTE* <B>)` | reads a byte from the keyboard |
| `int hwrite(BYTE <B>)` | writes a byte to the keyboard |
| `int dwrite(struct ps2_device <DEV>, BYTE <B>)` | writes a byte to a device |
| `int dsend_key_make(struct ps2_device <DEV>, int <KEY>)` | sends the make code for a key to a device |
| `int dsend_key_break(struct ps2_device <DEV>, int <KEY>)` | sends the break code for a key to a device |
| `int dsend_key_press(struct ps2_device <DEV>, int <KEY>, int <MILLIS>)` | sends the make and break code for a key, with a delay to a device |
| `int init_keyboard(struct ps2_device <DEV>)` | sends 0xAA to a device to signal a Basic Assurance Test (BAT) pass (necessary within 500ms of power on for device to be recognised) |
| `bool probe_status(void)` | requests the keyboard readminister its Basic Assurance Test (BAT), and returns true if it was successful |
| `bool hsend_echo(void)` | sends an echo request to the keyboard and returns true if the device sucessfully responds |
  
The value of `<KEY>` can be any of the following:  
  
|           |           | KEYS      |           |           |
| --------- | --------- | --------- | --------- | --------- |
| KEY_ESC   | KEY_6     | KEY_CAPLK | KEY_M     | KEY_KP3   |
| KEY_F1    | KEY_7     | KEY_A     | KEY_COMMA | KEY_KP0   |
| KEY_F2    | KEY_8     | KEY_S     | KEY_DOT   | KEY_KPDOT |
| KEY_F3    | KEY_9     | KEY_D     | KEY_FWSLH | KEY_LWIN  |
| KEY_F4    | KEY_0     | KEY_F     | KEY_RSHFT | KEY_RALT  |
| KEY_F5    | KEY_BKSPC | KEY_G     | KEY_LCTRL | KEY_RWIN  |
| KEY_F6    | KEY_TAB   | KEY_H     | KEY_LALT  | KEY_MENUS |
| KEY_F7    | KEY_Q     | KEY_J     | KEY_SPACE | KEY_RCTRL |
| KEY_F8    | KEY_W     | KEY_K     | KEY_NUMLK | KEY_INS   |
| KEY_F9    | KEY_E     | KEY_L     | KEY_KPMUL | KEY_HOME  |
| KEY_F10   | KEY_R     | KEY_COLON | KEY_KPSUB | KEY_PGUP  |
| KEY_F11   | KEY_T     | KEY_QUOTE | KEY_KP7   | KEY_DEL   |
| KEY_F12   | KEY_Y     | KEY_ENTER | KEY_KP8   | KEY_END   |
| KEY_SCRLK | KEY_U     | KEY_LSHFT | KEY_KP9   | KEY_PGDN  |
| KEY_GRAVE | KEY_I     | KEY_Z     | KEY_KPADD | KEY_UP    |
| KEY_1     | KEY_O     | KEY_X     | KEY_KP4   | KEY_LEFT  |
| KEY_2     | KEY_P     | KEY_C     | KEY_KP5   | KEY_DOWN  |
| KEY_3     | KEY_LSQR  | KEY_V     | KEY_KP6   | KEY_RIGHT |
| KEY_4     | KEY_RSQR  | KEY_B     | KEY_KP1   | KEY_KPDIV |
| KEY_5     | KEY_BKSLH | KEY_N     | KEY_KP2   | KEY_PRINT |
| KEY_BREAK |           |           |           |           |
