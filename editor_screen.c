//------------------------------------------------------------------------------
// Модуль реализует экран установки параметров
//------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "messages.h"
#include "type.h"
#include "lcd.h"
#include "font.h"
#include "bmps.h"
#include "buttons.h"
#include "message_window.h"
#include "variables.h"
#include "editor.h"
#include "editor_screen.h"

char es_focus = 0;

void DrawEditorScreen(void)
{
  char buffer1[50];
  char buffer2[9];
  char buffer3[9];
 
  //debug
  SetIntValueByTag(SHIFT_T_L, 123456);
  //-------------------------
  SetFont(SMALL_FONT);
  LcdClear();
  sprintf(buffer1, "Введите %s", GetNameByTag(SHIFT_T_L));
  LcdText(25, 0, 127, 7, buffer1);
  
//  GetStringByTag(SHIFT_T_L, ID_CURR_VAL, buffer2, 8);
  LcdText(25, 8, 127, 15, "в диапазоне");
  
  GetStringByTag(SHIFT_T_L, ID_MIN, buffer2, 8);
  GetStringByTag(SHIFT_T_L, ID_MAX, buffer3, 8);
  sprintf(buffer1,"от %s до %s", buffer2, buffer3);
  LcdText(25, 16, 127, 23, buffer1);
  
  SetEditorMode(GetTypeByTag(SHIFT_T_L), GetValueByTag(SHIFT_T_L));
  SetEditorCoord(25, 26, 80, 39);
  DrawEditor();
  
  sprintf(buffer1,", %s", GetUnitByTag(SHIFT_T_L));
  LcdText(82, 28, 127, 35, buffer1);
  
  LcdBmp(4, 20, BMP_KEY_INPUT_WIDTH, BMP_KEY_INPUT_HEIGHT, bmp_key_input); 
  
  DrawButton(0, 53, BTN_OK, es_focus == 1);
  DrawButton(42, 53, BTN_DEFAULT, es_focus == 2);
  DrawButton(84, 53, BTN_CANCEL, es_focus == 3);
  LcdDraw();
}

void ProcessEditorScreen(void)
{
  //--------------------------------------
  // es_focus
  //--------------------------------------
  //  0         editor
  //  1         OK button
  //  2         Default button
  //  3         Cancel button
 switch(es_focus)
  {
    case 0://Inactive state
      if(GetMessage(MSG_EDITOR_SCREEN_ACTIVATE))
      {
        es_focus = 0;
        DrawEditorScreen();
        SendMessage(MSG_EDITOR_ACTIVATE);
      };
      break;
   /*   
    case 1: //active state, editor is focused
      if(GetMessage(MSG_KEY_PRESSED))
        switch(GetKeyCode())
        {
          case KEY_0: //??? ??????? ????? ???????? ???????
          case KEY_1:
          case KEY_2:
          case KEY_3:
          case KEY_4:
          case KEY_5:
          case KEY_6:
          case KEY_7:
          case KEY_8:
          case KEY_9:
            if(caltime_focus == 2)
            {
              caltime_state = 2;
              LCD_rectinv(52, 33, 73, 42);
              SetIntEditor(53, 34, 4, 0, GetKeyCode() - KEY_0);
              SendMessage(MSG_EDITOR_ACTIVATE); //???????????? ???????? ?????
            };
            break;
          case KEY_LEFT: //??????? ????? - ??????? ????? ?????? ??????? ???????
            if(caltime_focus > 0)
              caltime_focus--;
            else
              caltime_focus = 2;
            UpdateCTControls();
            LCD_Draw();
            break;
          case KEY_RIGHT: //??????? ?????? - ??????? ????? ?? ??????? ???????
            if(caltime_focus < 2)
              caltime_focus++;
            else
              caltime_focus = 2;
            UpdateCTControls();
            LCD_Draw();
            break;
          case KEY_UP: //??????? ????? ??? ???? - ????? ?? ????????
          case KEY_DOWN:
            caltime_focus = 2;
            UpdateCTControls();
            LCD_Draw();
            break;
          case KEY_ENTER: //???? ?????? ENTER
            if(caltime_focus == 0) //?? ?????? "??????"
            {
              SendMessage(MSG_DLG_CANCEL); //?? ??????
              caltime_state = 0;
            }
            else //?? ?????? "??" ??? ? ?????????
              if(CalTimeIsCorrect()) //? ????? ??????? ?????
              {
                caltime_state = 0;
                SendMessage(MSG_DLG_OK); //?? ??
              };
            break;
          case KEY_CLEAR: //???? ?????? ?????? CANCEL
            SendMessage(MSG_DLG_CANCEL); //?? ??????
            caltime_state = 0;
            break;
        };
      break;
    case 2: // ??????? ???????? ?????

      if(GetMessage(MSG_EDITOR_FINISHED)) //???? ???????? ??????? ???????? ??????
      {
        if(CalTimeIsCorrect())
        {
          caltime_state = 0;
          caltime = GetIntEditorValue();
          SendMessage(MSG_DLG_OK); //?? ??
        }
        else
        {
          caltime_state = 1;
          caltime_focus = 0;
          UpdateCTControls();
          LCD_Draw();
        };
      };
      if(GetMessage(MSG_EDITOR_CANCELLED)) //???? ???? ??????? ?????????????
      {
        SendMessage(MSG_DLG_CANCEL); //?? ??????
        caltime_state = 0;
      };

      //??????? ????? ??? ???? - ????? ?? ?????? "??????"
      if(GetMessage(MSG_EDITOR_KEY_UP) || GetMessage(MSG_EDITOR_KEY_DOWN))
      {
        caltime_state = 1;
        caltime_focus = 0;
        caltime = GetIntEditorValue();
        UpdateCTControls();
        LCD_Draw();
      };
      break;
      */
  }
}