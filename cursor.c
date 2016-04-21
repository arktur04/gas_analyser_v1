//------------------------------------------------------------------------------
// ������ ��������� ������� ���������� ��������
//------------------------------------------------------------------------------
#include "cursor.h"
#include "LCD.h"
#include "timers.h"
#include "messages.h"

char cur_x, cur_y, cur_is_active, cur_is_visible;
char cur_state; //���������� ��������� ��������

//------------------------------------------------------------------------------
// ������� ClearCursor
// ������� ������� ����� �������
//------------------------------------------------------------------------------
void ClearCursor(void)
{
  LcdSetColor(WHITE);
  LcdLine(cur_x, cur_y, cur_x, cur_y + 7);
  LcdSetColor(BLACK);
}

//------------------------------------------------------------------------------
// ������� DrawCursor
// ������� ������ ����� �������
//------------------------------------------------------------------------------
void DrawCursor(void)
{
  LcdLine(cur_x, cur_y, cur_x, cur_y + 7);
}

//------------------------------------------------------------------------------
// ������� SetCursorPos
// ������� ������������� ������� �������
// ���������
// x, y - ���������� �������
//------------------------------------------------------------------------------
void SetCursorPos(char x, char y)
{
  if(cur_is_active) ClearCursor();
  cur_x = x;
  cur_y = y;
  if(cur_is_active) DrawCursor();
}

//------------------------------------------------------------------------------
// Init � Process
//------------------------------------------------------------------------------
void InitCursor(void)
{
  cur_is_active = 0;
  cur_state = 0;
}

void ProcessCursor(void)
{
  switch(cur_state)
  {
    case 0:  //���������� ���������
      if(GetMessage(MSG_CUR_ACTIVATE))
      {
        cur_state = 1;
        cur_is_visible = 1;
        cur_is_active = 1;
        DrawCursor();
        LcdDraw();
        ResetTimer(CURSOR_TIMER);
      };
      break;
    case 1:  //������ �������
      if(GetMessage(MSG_CUR_DEACTIVATE))
      {
        //���������� �������
        cur_is_visible = 0;
        ClearCursor();    
        LcdDraw();
        cur_state = 0;
        cur_is_active = 0;
      };
      //������ ������ � �������� cur_period
      if (GetTimer(CURSOR_TIMER) > (cur_period / 2))
      {
        if(cur_is_visible)
        {
          cur_is_visible = 0;
          ClearCursor();
        }
        else
        {
          cur_is_visible = 1;
          DrawCursor();
        };
      LcdDraw();
      ResetTimer(CURSOR_TIMER);
      };
      break;
  }
}
