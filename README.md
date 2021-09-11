# ard-ps2
ard-ps2 can be used to read (write) data from (to) a PS/2 keyboard or mouse.  

---
### <p align="center">Usage</p>
---
To start, initialise the pins with the following:  
`init(<DATA PIN #>, <CLOCK PIN #>);`  
  
Now, you there are two possibilities: the arduino can act as the device (probably a keyboard or mouse), or the arduino can act as the host (communicating with a peripheral).  

#### **ARDUINO AS DEVICE**
when the arduino is acting as the device (ps2_device.ino), you can send a key make code with the following:  
`send_key_make(<KEY>);`  
  
Similarly, you can send a key break code as follows:  
`send_key_break(<KEY>);`  
  
You can also send a keypress (make, delay, then break) as follows:  
`send_key_press(<KEY>, <DELAY>);`  
  
Where `<DELAY>` is the delay between sending make and break codes for the key `<KEY>`.  
  
The value of `<KEY>` can be any of the following:  
  
|           |           |           |           |           |
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
|           |           |           |           |           |
  
You can also send data directly using the following syntax:  
`write(<BYTE>);`  
  
Where `<BYTE>` is the single byte of data to be sent to the host.

#### **ARDUINO AS HOST**
When the arduino acts as a host (ps2_host.ino) it can read data from a peripheral using the following syntax:  
`read(<PTR>);`  
  
Where `<PTR>` is a pointer to store the result (one byte). After a successful read, this function returns 0, otherwise a negative error code is returned.  
  
If the host wishes to send data to the peripheral, the following syntax is used:  
`write(<BYTE>);`  
  
Where `<BYTE>` is the single byte of data to be written. After a successful write, this function returns 0, otherwise a negative error code is returned.  
  
Using the following syntax, the host can send an echo request to the peripheral:  
`send_echo();`  
  
This function returns 1 if the peripheral successfully responded to the echo request, and 0 otherwise.  
  
Finally, the following command can be used to request that the peripheral re-administer its Basic Assurance Test (BAT):  
`probe_status();`  
  
Similarly, this function returns 1 if the BAT is successful, and 0 if either the test failed or the device failed to respond.
