/////////// PS/2 KEYBOARD EXAMPLE ///////////////////////////
// dsc: Reads data from a PS/2 keyboard and relays info to //
// different hosts depending on who is currently selected: //
// to swap hosts, the menu key must be pressed.            //
/////////////////////////////////////////////////////////////

#include "ps2_key.h"

int DEV2_DATA = 7;
int DEV2_CLOK = 6;
int DEV1_DATA = 5;
int DEV1_CLOK = 4;
int HOST_DATA = 3;
int HOST_CLOK = 2;

const int NUM_KBDS = 2;

struct ps2_device kbds[NUM_KBDS];

int current_kbd = 0;
int swp_progress = 0;

void setup() {
  kbds[0] = ps2_init_device(DEV2_DATA, DEV2_CLOK);
  kbds[1] = ps2_init_device(DEV1_DATA, DEV1_CLOK);
  ps2_init_host(HOST_DATA, HOST_CLOK);
  
  delay(500); // delay for BAT (Basic Assurance Test)
  
  for (int i = 0; i < NUM_KBDS; i++)
    while (dwrite(kbds[i], 0xAA)); // send BAT complete signal to host
  
  return;
}

void loop() {
  BYTE b = 0;
  while (hread(&b));
  
  if (swp_progress == 1 && b == 47)
    current_kbd = (current_kbd + 1) % NUM_KBDS;
  
  else swp_progress = 0;
  
  if (swp_progress == 0 && b == 224)
    swp_progress = 1;
  
  else swp_progress = 0;
  
  dwrite(kbds[current_kbd], b);
}