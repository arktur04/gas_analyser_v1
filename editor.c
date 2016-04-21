//------------------------------------------------------------------------------
// Модуль реализует экран установки параметров
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "lcd.h"
#include "cursor.h"
#include "keyb.h"
#include "font.h"
#include "dict.h"
#include "messages.h"
#include "variables.h"

#define MAX_POS 8

char e_pos = 0;
char e_state = 0;
char e_type = FLOAT;
char buffer[50];

char _x1; char _y1; char _x2; char _y2; char _active =  0;

float _e_value, _e_min, _e_max;

void SetEditorMode(char _e_type, T_VAR e_init_val, T_VAR e_min, T_VAR e_max)
{
  e_type = _e_type;
  if(e_type == INT) 
  {
    sprintf(buffer, "%d", e_init_val.ival);
    _e_value = e_init_val.ival;
    _e_min = e_min.ival;
    _e_max = e_max.ival;    
  };
  if(e_type == FLOAT)
  {
    sprintf(buffer, "%f", e_init_val.fval);
    _e_value = e_init_val.ival;
    _e_min = e_min.fval;
    _e_max = e_max.fval;      
  };
};

void SetEditorCoord(char x1, char y1, char x2, char y2)
{
  _x1 = x1;
  _y1 = y1;
  _x2 = x2;
  _y2 = y2;  
}

float GetEditorValue(void)
{
  _e_value = (e_type == INT)?
  strtol(buffer, NULL, 0):
  strtod(buffer, NULL);
  return _e_value;
}

void DrawEditor(void)
{
  LcdLine(_x1, _y1, _x2, _y1);  
  LcdLine(_x1, _y2, _x2, _y2);
  LcdLine(_x1, _y1, _x1, _y2);  
  LcdLine(_x2, _y1, _x2, _y2);
  if(_active)  {
    LcdLine(_x1 + 1, _y1 + 1, _x2 - 1, _y1 + 1);  
    LcdLine(_x1 + 1, _y2 - 1, _x2 - 1, _y2 - 1);
    LcdLine(_x1 + 1, _y1 + 1, _x1 + 1, _y2 - 1);  
    LcdLine(_x2 - 1, _y1 + 1, _x2 - 1, _y2 - 1);
  };
  
  SetFont(SMALL_FONT);
  LcdText(_x1 + 3, _y1 + 3, _x2 - 3, _y2 - 3, buffer);
}

void UpdateEditor(void)
{
  DrawEditor();
  SetCursorPos(_x1 + 2 + e_pos * 5, _y1 + 3);
  LcdDraw();
};

char ValueIsValid(void)
{
  return ((_e_value < _e_max) && (_e_value > _e_min));
}

void InsertToBuffer(char ch)
{
  if((e_pos < MAX_POS) && (strlen(buffer) < MAX_POS))
  {
    for(char i = MAX_POS; i > e_pos; i--)
      buffer[i] = buffer[i - 1];
    buffer[e_pos] = ch;
    e_pos++;
    UpdateEditor();
  };
}

void ProcessEditor(void)
{
 // char curr_index;
  //curr_index = ps_focus + ps_shift + ps_base;
  switch(e_state)
  {
    case 0: //неактивное состояние
      _active = FALSE;
      if(GetMessage(MSG_EDITOR_ACTIVATE))
      {
        e_state = 1;
        e_pos = strlen(buffer);
        _active = TRUE;
        UpdateEditor();
        SendMessage(MSG_CUR_ACTIVATE);
      };
      break;
      
    case 1: //активное состояние
      /*
      if(GetMessage(MSG_USER_PASS_CHANGED))
        ParamScreenFullUpdate();
      */
      if(GetMessage(MSG_KEY_PRESSED))
        switch (GetKeyCode())
        {
        case KEY_0:
        case KEY_1:
        case KEY_2:
        case KEY_3:
        case KEY_4:
        case KEY_5:
        case KEY_6:
        case KEY_7:
        case KEY_8:
        case KEY_9:
          InsertToBuffer(GetKeyCode() - KEY_0 + '0');
          break;
        case KEY_DOT:
          InsertToBuffer('.');
          break;
        case KEY_MINUS:
          InsertToBuffer('-');
          break;  
        case KEY_LEFT:
          if(e_pos > 0)
          {
            e_pos--;
            UpdateEditor();
          };
          break;          
        case KEY_RIGHT:
          if(e_pos < MAX_POS)
          {
            e_pos++;
            UpdateEditor();
          };
          break;
        case KEY_DEL:
//          if(e_pos > 0)
          {
            for(char i = e_pos; i < MAX_POS; i++)
              buffer[i] = buffer[i + 1];
//            e_pos--;
            UpdateEditor();
          };
          break;           
        case KEY_ENTER:
          if(ValueIsValid())
          {
            SendMessage(MSG_EDITOR_FINISHED);
            SendMessage(MSG_EDITOR_DEACTIVATED);
            SendMessage(MSG_CUR_DEACTIVATE);
            e_state = 0;
          }
          else
          {
            e_state = 2;
            SendMessage(MSG_MESSAGE_SCREEN_ACTIVATE);
          };
          break;
        case KEY_ESC:
          SendMessage(MSG_EDITOR_CANCELLED);
          SendMessage(MSG_CUR_DEACTIVATE);
          SendMessage(MSG_EDITOR_DEACTIVATED);
          e_state = 0;
          break;
        case KEY_UP:
          if(ValueIsValid())
            SendMessage(MSG_EDITOR_KEY_UP);
            SendMessage(MSG_CUR_DEACTIVATE);
            SendMessage(MSG_EDITOR_DEACTIVATED);
            e_state = 0;
          break;
        case KEY_DOWN:
          SendMessage(MSG_EDITOR_KEY_DOWN);
          SendMessage(MSG_CUR_DEACTIVATE);
          SendMessage(MSG_EDITOR_DEACTIVATED);
          e_state = 0;
          break;           
        };
    case 2: //состояние "активно окно сообщений"
      if(GetMessage(MSG_MESSAGE_SCREEN_DEACTIVATED))
      {
        SendMessage(MSG_CUR_ACTIVATE);
        e_state = 1;
      };
      break;
  };
}