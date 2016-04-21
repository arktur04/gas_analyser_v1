#include "dict.h"
/*
static char checked_cb[8] = {0xFF, 0x99, 0xB1, 0xE1, 0xF1, 0x9D, 0x87, 0xFF};
static char cb[8] = {0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF};

void DrawCheckBox(char x, char y, char checked, char selected)
{
  if(selected)
  {
    LcdLine(x, y, x + 9, y);
    LcdLine(x + 9, y, x + 9, y + 9);
    LcdLine(x, y + 9, x + 9, y + 9);
    LcdLine(x, y, x, y + 9);
  };
  if(checked)
  {
    LcdBmp(x + 1, y + 1, 8, 8, checked_cb);
  }
  else
  {
    LcdBmp(x + 1, y + 1, 8, 8, cb);
  }    
}
*/