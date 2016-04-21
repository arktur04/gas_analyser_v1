//------------------------------------------------------------------------------
// Модуль реализует окно сообщений
//------------------------------------------------------------------------------
#include "lcd.h"
#include "font.h"
#include "bmps.h"
#include "buttons.h"
#include "message_window.h"

char mw_focus = 0;

void DrawMessageWindow(char* message_line1, char* message_line2,
                       char* message_line3, char buttons)
{
  SetFont(SMALL_FONT);
  LcdSetColor(WHITE);
  LcdRect(5, 9, 123, 54);
  LcdSetColor(BLACK);
  
  LcdLine(6,   9,   122, 9);
  LcdLine(6,   55,  122, 55);
  LcdLine(4,   11,  4,   53);
  LcdLine(124, 11,  124, 53);
  
  LcdLine(5,   10,  5,   10);
  LcdLine(5,   54,  5,   54);
  LcdLine(123, 10,  123, 10);
  LcdLine(123, 54,  123, 54);
  
  LcdBmp(6, 26, BMP_EXLAMATION_WIDTH, BMP_EXLAMATION_HEIGHT, bmp_exlamation); 
  //LcdBmp(6, 26, BMP_KEY_INPUT_WIDTH, BMP_KEY_INPUT_HEIGHT, bmp_key_input); 
  
  LcdText(29, 12, 122, 18, message_line1);
  LcdText(29, 21, 122, 27, message_line2);
  LcdText(29, 30, 122, 36, message_line3);
  
  if(buttons & MB_OK)
    DrawButton(29, 40, BTN_OK, mw_focus == 0);
  if(buttons & MB_CANCEL)
    DrawButton(75, 40, BTN_CANCEL, mw_focus == 1);
};

