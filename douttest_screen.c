//------------------------------------------------------------------------------
// ������ ��������� ����� ��������� ����������
//------------------------------------------------------------------------------
#include "type.h"
#include "lcd.h"
#include "font.h"
#include "bmps.h"
#include "buttons.h"
#include "checkbox.h"

char dots_focus = 5;
char dots_state = 0;
char dots_relay_states[6] = {0, 0, 1, 0, 0, 1};

static char *dots_buttons[6] = {
"���� 1", "���� 2",
"���� 3", "���� 4",
"���� 5", "���� 6",};

void DrawDOutTestScreen(void)
{
  char n;
  SetFont(SMALL_FONT);
  LcdText(1, 0, 118, 7, "���� ���������� �������");
  LcdLine(0, 8, 127, 8);  
  LcdLine(0, 9, 127, 9);
  
  for(char x = 0; x < 2; x++)
    for(char y = 0; y < 3; y++)
    {
      n = y * 2 + x;
      DrawTextButton(x * 64, y * 11 + 9, x* 64 + 52, y * 11 + 20, 
                     dots_buttons[n],
                     dots_focus == n);
      //void DrawCheckBox(char x, char y, char checked, char selected);
      DrawCheckBox(x* 64 + 53, y * 11 + 10, dots_relay_states[n], FALSE);
    };
  DrawButton(0, 53, BTN_BACK, dots_focus == 8);
  DrawButton(42, 53, BTN_HOME, dots_focus == 9);
  DrawButton(84, 53, BTN_FORWARD, dots_focus == 10);
  

  LcdDraw();
}
