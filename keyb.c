#include "keyb.h"
//#include "pio.h"
#include "board.h"
#include "messages.h"
#include "timers.h"

char key_code, _key_code;
char key_state, _key_state;
char l_encoder_rotor, r_encoder_rotor, _l_encoder_rotor, _r_encoder_rotor;
char l_encoder_button, r_encoder_button, _l_encoder_button, _r_encoder_button;
char deb_state1 = 0, deb_state2 = 0;
//unsigned long encoders_code_ = 0xFF;

//----------------------------------------
// Encoder states
#define ES_00    0
#define ES_01CW  1
#define ES_01CCW 2
#define ES_10CW  3
#define ES_10CCW 4
#define ES_11    5
//----------------------------------------

char l_encoder_state = ES_00, r_encoder_state = ES_00;

#define NUM_CODES 20

static unsigned int key_codes[NUM_CODES] = {0xDFFF, 0xFFFE, 0xFFFD, 0xFFFB,
  0xFFEF, 0xFFDF, 0xFFBF, 0xFEFF, 0xFDFF, 0xFBFF, 0xBFFF, 0xF7FF, 0xFF7F,
  0xEFFF, 0xFFF7, 0x7FFD, 0x7DFF, 0x7FEF, 0x7FBF, 0x6FFF};

void kbd_CfgLines(char line)
{
  static unsigned long key_rows[4] = {KEY_ROW0, KEY_ROW1, KEY_ROW2, KEY_ROW3};
  for (char i = 0; i < 4; i++)
    if (i == line)
    {
      OutputClr(key_rows[i]);
    }
    else
    {
      OutputSet(key_rows[i]);
    };
}

void InitKeyb(void)
{
  kbd_CfgLines(0);
  key_code = 0;
  key_state = 0;

  ResetTimer(LAST_KEY_TIMER);
}

char GetKeyCode(void)
{
  return key_code;
}

char KeyCodeToASCII(char code)
{
  if((code >= KEY_0) && (code <= KEY_9))
    return (0x30 + code - KEY_0);
  if(code == KEY_MINUS)
    return '-';
  if(code == KEY_DOT)
    return '.';
  return 0;
}
/*
int GetScanCode(void)
{
  return _scan_code;
}
*/
void ProcessKeyFSM(void)
{
  switch (key_state)
  {
    case 0: //no pressed keys
      if (key_code > 0)
      {
        _key_code = key_code;
        key_state = 1;
      };
    break;
    case 1: //debounce delay
      if (GetTimer(KEYB_TIMER) > debounce) 
        key_state = 2;
    break;
    case 2: //if pressed, send message
      if (key_code == _key_code)
      {
        ResetTimer(LAST_KEY_TIMER);
        SendMessage(MSG_KEY_PRESSED);
        key_state = 3;
      }
      else
        key_state = 0;
    break;
    case 3: //if released, send message, or wait for first repeating
      if (key_code == _key_code)
      {
        if (GetTimer(KEYB_TIMER) >= first_delay)
        {
          ResetTimer(LAST_KEY_TIMER);
          SendMessage(MSG_KEY_PRESSED);
          key_state = 4;
        };
      }
      else
        key_state = 0;
    break;
    case 4: //if released, send message, or wait for autorepeating
      if (key_code == _key_code)
      {
        if (GetTimer(KEYB_TIMER) >= auto_repeat)
        {
          ResetTimer(KEYB_TIMER); // we remain in this state;
          ResetTimer(LAST_KEY_TIMER);
          SendMessage(MSG_KEY_PRESSED);
        };
      }
      else
        key_state = 0;
    break;
  };
  if (key_state != _key_state) ResetTimer(KEYB_TIMER);
  _key_state = key_state;
}

void ProcessEncoderDebounce(char *state, char pressed, char timer, char msg)
{
    switch (*state)
  {
    case 0: //no pressed keys
      if (pressed)
        *state = 1;
    break;
    case 1: //debounce delay
      if (GetTimer(timer) > debounce) 
        *state = 2;
    break;
    case 2: //if pressed, send message
      if (pressed)
      {
        SendMessage(msg);
        *state = 3;
      }
      else
        *state = 0;
    break;
    case 3: //wait until release
      if (!pressed)
        *state = 0;
    break;
  }
}

void ProcessEncoderFSM(char *encoder_state, char encoder_rotor, char msg_cw, char msg_ccw)
{
    switch(*encoder_state)
  {
  case ES_00:
    switch(encoder_rotor)
    {
    case 0x10: //
      *encoder_state = ES_01CW;
      break;
    case 0x20:
      *encoder_state = ES_10CCW;
      break;
    };
    break;
    //--------------------------------------
  case ES_01CW:
    switch(encoder_rotor)
    {
    case 0x00: 
      *encoder_state = ES_00;
      break;
    case 0x30:
      *encoder_state = ES_11;
      SendMessage(msg_cw);
      break;
    };
    break;
    //--------------------------------------------
  case ES_01CCW:
    switch(encoder_rotor)
    {
    case 0x00: 
      *encoder_state = ES_00;
      SendMessage(msg_ccw);
      break;
    case 0x30:
      *encoder_state = ES_11;
      break;
    };    
    break;
    //-------------------------------------
  case ES_10CW:
    switch(encoder_rotor)
    {
    case 0x00: 
      SendMessage(msg_cw);
      *encoder_state = ES_00;
      break;
    case 0x30:
      *encoder_state = ES_11;
      break;
    };    
    break;
    //------------------------------------
  case ES_10CCW:
    switch(encoder_rotor)
    {
    case 0x00: 
      *encoder_state = ES_00;
      break;
    case 0x30:
      SendMessage(msg_ccw);
      *encoder_state = ES_11;
      break;
    };     
    break;
    //----------------------------------------
  case ES_11:
    switch(encoder_rotor)
    {
    case 0x10: //
      *encoder_state = ES_01CCW;
      break;
    case 0x20: // 
      *encoder_state = ES_10CW;
      break;
    };     
    break;
  };
}

void ProcessKeyb(void)
{
  char input;
  unsigned long scan_code = 0;
  
  for(char key_count = 0; key_count < 4; key_count++)
  {
    kbd_CfgLines(key_count);
    input = GetKbdBus();   
    scan_code |= (input & 0x0F) << (4 * key_count);
    if (key_count == 0)
    {
      l_encoder_button = ~input & 0x20;
      r_encoder_button = ~input & 0x10;
    };
    if (key_count == 1)
    {
      l_encoder_rotor = (~input & 0x20) >> 1;
      r_encoder_rotor = ~input & 0x10;
    };
    if (key_count == 2)
    {
      l_encoder_rotor |= ~input & 0x20;
      r_encoder_rotor |= (~input & 0x10) << 1;
    };
  };     

  OutputSet(KEY_ROW0 | KEY_ROW1 | KEY_ROW2 | KEY_ROW3);
  
  ProcessEncoderFSM(&l_encoder_state, l_encoder_rotor, MSG_L_ENCODER_CW, 
                    MSG_L_ENCODER_CCW);
  ProcessEncoderFSM(&r_encoder_state, r_encoder_rotor, MSG_R_ENCODER_CW, 
                    MSG_R_ENCODER_CCW);
  ProcessEncoderDebounce(&deb_state1, l_encoder_button, DEB_TIMER1,
                         MSG_L_ENCODER_PRESSED);
  ProcessEncoderDebounce(&deb_state2, r_encoder_button, DEB_TIMER2,
                         MSG_R_ENCODER_PRESSED);
  
  key_code = 0;
  for (char i = 0; i < NUM_CODES; i++)
  if (key_codes[i] == scan_code)
    key_code = i + 1;
  
  ProcessKeyFSM();
}
