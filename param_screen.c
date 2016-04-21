//------------------------------------------------------------------------------
// ������ ��������� ����� ��������� ����������
//------------------------------------------------------------------------------
#include "type.h"
#include "lcd.h"
#include "font.h"
#include "bmps.h"
#include "buttons.h"
#include "pass_controller.h"
#include "message_window.h"
#include "variables.h"
#include "editor_screen.h"
#include "param_screen.h"

/*
typedef struct{
  char header[25]; //��������� ������
  int var1; //��� ���������� 1
  int var2; //��� ���������� 2
  int var3; //��� ���������� 3
  int var4; //��� ���������� 4
  int number; //����� ������
  char back_allowed; //������ "�����" ���������
  char home_allowed; //������ "������� �����" ���������
  char forward_allowed; //������ "������" ���������
} T_SCREEN_DESCRIPTOR;
*/

T_SCREEN_DESCRIPTOR screens[4] = {
  {"��������� ����� (1/2)",  SHIFT_T_L, K_CD_T_L, T_F_T_L,  ADJ_S_T_L,  0, FALSE, TRUE, TRUE},
  {"��������� ����� (2/2)",  K_P_T_L,   K_D_T_L,  T_DF_T_L, SH_CLC_T_L, 1, TRUE, TRUE, TRUE},
  {"��������� ������ (1/2)", SHIFT_T_R, K_CD_T_R, T_F_T_R,  ADJ_S_T_R,  2, TRUE, TRUE, TRUE},
  {"��������� ������ (2/2)", K_P_T_R,   K_D_T_R,  T_DF_T_R, SH_CLC_T_R, 3, TRUE, TRUE, FALSE}};

char ps_focus = 5;
char ps_screen = 0;

void DrawParamScreen(void)
{
  SetFont(SMALL_FONT);
  LcdText(1, 0, 118, 7, screens[ps_screen].header);
  LcdLine(0, 8, 127, 8);  
  LcdLine(0, 9, 127, 9);
  LcdLine(0, 20, 127, 20);  
  LcdLine(0, 31, 127, 31);  
  LcdLine(0, 42, 127, 42);  
  LcdLine(0, 53, 127, 53); 
  //LcdLine(0, 54, 127, 54);
  
  LcdLine(42, 10, 42, 52);
  LcdLine(86, 10, 86, 52);
  LcdLine(119, 0, 119, 7);
  
  if(GetPasswordEntered()) LcdText(121, 1, 127, 7, "P");
  
  LcdText(1, 11, 41, 18, GetNameByTag(screens[ps_screen].var1));
  if(ps_focus == 0)
  {
    LcdSetColor(BLACK);
    LcdRect(43, 10, 85, 19);
    LcdSetColor(WHITE);
  };
  LcdText(44, 11, 85, 18, "9234.567");
  LcdSetColor(BLACK);
  LcdText(88, 11, 127, 18, GetUnitByTag(screens[ps_screen].var1));
  
  LcdText(1, 22, 41, 29, "variabl2");
  if(ps_focus == 1)
  {
    LcdSetColor(BLACK);
    LcdRect(43, 21, 85, 30);
    LcdSetColor(WHITE);
  };
  LcdText(44, 22, 85, 29, "9234.567");
  LcdSetColor(BLACK);    
  LcdText(88, 22, 127, 29, "some_tx2");

  LcdText(1, 33, 41, 40, "variabl3");
  if(ps_focus == 2)
  {
    LcdSetColor(BLACK);
    LcdRect(43, 32, 85, 41);
    LcdSetColor(WHITE);
  };
  LcdText(44, 33, 85, 40, "9234.567");
  LcdSetColor(BLACK);    
  LcdText(88, 33, 127, 40, "some_tx3");

  LcdText(1, 44, 41, 51, "variabl4");
  if(ps_focus == 3)
  {
    LcdSetColor(BLACK);
    LcdRect(43, 43, 85, 52);
    LcdSetColor(WHITE);
  };
  LcdText(44, 44, 85, 51, "9234.567");
  LcdSetColor(BLACK);  
  LcdText(88, 44, 127, 51, "some_tx4");
  
  //control buttons
  /*
  if(ps_focus == 4)
  {
    LcdSetColor(BLACK);
    LcdRect(0, 55, 41, 63);
    LcdSetColor(WHITE);
  };
  LcdText(1, 56, 41, 63, "�����");
  LcdSetColor(BLACK);  

  if(ps_focus == 5)
  {
    LcdSetColor(BLACK);
    LcdRect(43, 55, 85, 63);
    LcdSetColor(WHITE);
  };  
  LcdText(44, 56, 85, 63, "��. ���.");
  LcdSetColor(BLACK);
  
  if(ps_focus == 6)
  {
    LcdSetColor(BLACK);
    LcdRect(87, 55, 127, 63);
    LcdSetColor(WHITE);
  };
  LcdText(88, 56, 127, 63, "������");  
  LcdSetColor(BLACK);
  */
  LcdClear();
  DrawButton(0, 53, BTN_BACK, ps_focus == 4);
  DrawButton(42, 53, BTN_HOME, ps_focus == 5);
  DrawButton(84, 53, BTN_FORWARD, ps_focus == 6);
  
  DrawMessageWindow("������1", "������2", "������2", MB_OK + MB_CANCEL);
  
  DrawEditorScreen();
    
  LcdDraw();
}

/*
//------------------------------------------------------------------------------
// ������ ��������� ����� ��������� ����������
//------------------------------------------------------------------------------
#include "param_screen.h"
#include "dialog_pass.h"
#include "dialog_win.h"
#include "messages.h"
#include "lcd.h"
#include "keyb.h"
#include "help_screen.h"
#include "editor.h"
#include "variables.h"
#include "passwords.h"
#include <stdio.h>
#include <string.h>

//----------------------------
char ps_state; //���������� ��������� ��������
char ps_focus; //����� �����
char ps_shift; //�������� ������ ������������ ������ ������ �� ���������
char ps_screen; //������������� ������ (��������, �. ������� � �.�.)
char ps_channel; //����� (0 - �����, 1 - ������)
char ps_size; //������ ������� � �������
char ps_base; //�������� ������������ ������ ���������� ������� ����������

char *help_captions[4] = //��������� �������
  {"�����. ���������",
   "�����. ��",
   "�����. ����������",
   "������"
  };

//------------------------------------------------------------------------------
// ������� SetParamScreen
// ������� ������������� ��������� ������
// ������� ������ ���� ������� ����� ���������� ������
// ���������:
//   ch - ����� (0 - �����, 1 - ������)
//   screen - �����
//     SCREEN_O2 - ��������� ������ ���������
//     SCREEN_XH - ��������� ������ �����������
//     SCREEN_T - ��������� ������ �����������
//     SCREEN_X - ���������������
//------------------------------------------------------------------------------
void SetParamScreen(char ch, char screen)
{
  ps_channel = ch;
  ps_screen = screen;
  switch(ps_screen)
  {
    case SCREEN_O2:
      ps_size = SIZE_O2;
      ps_base = BASE_O2;
      break;
    case SCREEN_XH:
      ps_size = SIZE_XH;
      ps_base = BASE_XH;
      break;
    case SCREEN_T:
      ps_size = SIZE_T;
      ps_base = BASE_T;
      break;
    case SCREEN_X:
      ps_size = 0;
      ps_base = 0;
      break;
  };
}

//------------------------------------------------------------------------------
// ������� ���������� �������� ������������� ����������
// ��������:
//   index - ����� ���������� � ���������� �������
// ������������ ��������:
//   *s
//------------------------------------------------------------------------------
void GetString(char index, char *s)
{
  int val;
  if(flash_vars[index].var_type == INT)
  {
    val = GetValue(ps_channel, index);
    sprintf(s, "%d", val);
  }
  else
    sprintf(s, "%.4f", GetValue(ps_channel, index));
}

//------------------------------------------------------------------------------
// ������� ClearParamFocus ������������ ������ �������,
// �� ����������� � ������ �����
// �������� focus ���������� ����� ������ �� ������
//------------------------------------------------------------------------------
void ClearParamFocus(char focus)
{
  char s[10];
  char base_y, y1, y2, texty, curr_index;

  base_y = focus * 11;
  y1 = 11 + base_y;
  y2 = 20 + base_y;
  if(y2 > 63) y2 = 63;
  texty = 12 + base_y;

  LCD_rectinv(0, y1, 41, y2);
  LCD_rectinv(43, y1, 84, y2);
  LCD_rectinv(86, y1, 118, y2);
  if(ps_size > focus)
  {
    curr_index = focus + ps_shift + ps_base;
    LCD_textout(1, texty, flash_vars[curr_index].name);
    GetString(curr_index, s);
    LCD_textout(44, texty, s);
    LCD_textout(87, texty, flash_vars[curr_index].unit);
  };
}

//------------------------------------------------------------------------------
// ������� SetParamFocus ������������ ������ �������,
// ����������� � ������ �����
// �������� focus ���������� ����� ������ �� ������
//------------------------------------------------------------------------------
void SetParamFocus(char focus)
{
  char s[10];
  char y, y1, y2, texty, curr_index;

  y = focus * 11;
  y1 = 11 + y;
  y2 = 20 + y;
  if(y2 > 63) y2 = 63;
  texty = 12 + y;

  LCD_rect(0, y1, 41, y2);
  LCD_rect(43, y1, 84, y2);
  LCD_rect(86, y1, 118, y2);
  if(ps_size > focus)
  {
    curr_index = focus + ps_shift + ps_base;
    LCD_textoutinv(1, texty, flash_vars[curr_index].name);
    GetString(curr_index, s);
    LCD_textoutinv(44, texty, s);
    LCD_textoutinv(87, texty, flash_vars[curr_index].unit);
  };
}

//------------------------------------------------------------------------------
// ������� ps_UpdateFocus �������������� ����� �����
// � ������������ � ������� ����������
//------------------------------------------------------------------------------
void ps_UpdateFocus(void)
{
  char i;
  for(i = 0; i < ps_focus; i++) ClearParamFocus(i);
  SetParamFocus(ps_focus);
  for(i = ps_focus + 1; i < 5; i++) ClearParamFocus(i);
}

//------------------------------------------------------------------------------
// ������� ps_ScrollerUpdate �������������� ��������
// � ������������ � ������� ����������
//------------------------------------------------------------------------------
void ps_ScrollerUpdate(void)
{
  DrawScrollerBar(10, 63, ps_size, 5, ps_shift);
}

//------------------------------------------------------------------------------
// ������� ps_FocusUp ���������� ����� ����� �� ������ �����
//------------------------------------------------------------------------------
void ps_FocusUp(void)
{
  if((ps_focus == 0) && (ps_shift > 0))
  {
    ps_shift--;
    ps_UpdateFocus();
    ps_ScrollerUpdate();
  };
  if(ps_focus > 0)
  {
    ClearParamFocus(ps_focus);
    ps_focus--;
    SetParamFocus(ps_focus);
  };
}

//------------------------------------------------------------------------------
// ������� ps_FocusDown ���������� ����� ����� �� ������ ����
//------------------------------------------------------------------------------
void ps_FocusDown(void)
{
  if((ps_focus == 4) && (ps_shift < (ps_size - 5)))
  {
    ps_shift++;
    ps_UpdateFocus();
    ps_ScrollerUpdate();
  };
  if((ps_focus < 4) && (ps_focus < ps_size - 1))
  {
    ClearParamFocus(ps_focus);
    ps_focus++;
    SetParamFocus(ps_focus);
  };
}

//------------------------------------------------------------------------------
// ������� ������������ ������ ��������� ������
//------------------------------------------------------------------------------
void ParamScreenFullUpdate(void)
{
  LCD_Clear();

  LCD_horline(0, 10, 127);
  LCD_horline(0, 21, 118);
  LCD_horline(0, 32, 118);
  LCD_horline(0, 43, 118);
  LCD_horline(0, 54, 118);

  LCD_vertline(42, 0, 63);
  LCD_vertline(85, 0, 63);

  LCD_vertline(97, 0, 9);
  LCD_vertline(109, 0, 9);
  LCD_vertline(121, 0, 9);

  LCD_textout(1, 1, "���");
  LCD_textout(44, 1, "��������");
  switch(ps_screen)
  {
    case SCREEN_O2:
      LCD_textout(87, 1, "O\xBF");
      break;
    case SCREEN_XH:
      LCD_textout(87, 1, "XH");
      break;
    case SCREEN_T:
      LCD_textout(89, 1, "T");
      break;
    case SCREEN_X:
      LCD_textout(89, 1, "X");
      break;
  };

  if(PassEntered(USER_PASS))
    LCD_textout(101, 1, "M");
  else
    LCD_rectinv(98, 0, 108, 9);

  if(ps_channel == 0)
    LCD_textout(113, 1, "�");
  else
    LCD_textout(113, 1, "�");

  DrawScrollerFrame(10, 63);

  ps_ScrollerUpdate();

  ps_UpdateFocus();
  LCD_Draw();
}

//------------------------------------------------------------------------------
// Init � Process
//------------------------------------------------------------------------------
void InitParamScreen(void)
{
  ps_state = 0;
}

void ProcessParamScreen(void)
{
  char curr_index;
  curr_index = ps_focus + ps_shift + ps_base;
  switch(ps_state)
  {
    case 0: //���������� ���������
      if(GetMessage(MSG_PS_ACTIVATE))
      {
        ps_state = 1;
        ps_focus = 0;
        ps_shift = 0;
        ParamScreenFullUpdate();
      };
      break;
    case 1: //�������� ���������
      if(GetMessage(MSG_USER_PASS_CHANGED))
        ParamScreenFullUpdate();
      if(GetMessage(MSG_KEY_PRESSED))
        switch (GetKeyCode())
        {
          case KEY_3:
          case KEY_UP: //������� ����� - ����� �����
            ps_FocusUp();
            LCD_Draw();
            break;
          case KEY_0:
          case KEY_DOWN: //������� ���� - ����� ����
            ps_FocusDown();
            LCD_Draw();
            break;
          case KEY_1:
          case KEY_F1: //F1 - ����� �������
            ps_state = 2;
            SetHelpScreen(HS_PARAM, *help_captions);
            SendMessage(MSG_HELP_ACTIVATE);
            break;
          case KEY_CLEAR: //CANCEL - �����
            ps_state = 0;
            SendMessage(MSG_PS_DEACTIVATED);
            break;
          case KEY_ENTER: //ENTER - ��������� � ����� ��������������
            if(PassEntered(USER_PASS)) //���� ������ ������
            {
              ps_state = 3;
              ClearParamFocus(ps_focus);
              if(flash_vars[curr_index].var_type == INT)
                SetIntEditor(44, (ps_focus + 1) * 11 + 1, 6, 0, 0);
              else
                SetFloatEditor(44, (ps_focus + 1) * 11 + 1, 6, 0);
              SendMessage(MSG_EDITOR_ACTIVATE);
            }
            else
            {
              ps_state = 4; //���� �� ������ ������ - �������� ������ "������� ������"
              SetPassword(GetPass(USER_PASS), "������� ������", "");
              SendMessage(MSG_PASSWORD_ACTIVATE);
            }
            break;
        };
      break;
    case 2:  //������� ����� �������
      if(GetMessage(MSG_HELP_DEACTIVATED))
      {
        ps_state = 1;
        ParamScreenFullUpdate();
        LCD_Draw();
      };
      break;
    case 3:  //������� �������� ��������� �����
      if(GetMessage(MSG_EDITOR_CANCELLED)) //���� �������� ������� �������������
      {
        ps_state = 1;
        ps_UpdateFocus();
        LCD_Draw();
      };
      if(GetMessage(MSG_EDITOR_FINISHED)) //���� �������� �������� �������
      {
        ps_state = 1;
        if(flash_vars[curr_index].var_type == INT)
          SetAndSaveValue(ps_channel, curr_index, GetIntEditorValue());
        else
          SetAndSaveValue(ps_channel, curr_index, GetFloatEditorValue());
        ps_UpdateFocus();
        LCD_Draw();
      };
      if(GetMessage(MSG_EDITOR_KEY_UP))
      {//������ ������� ����� � ��������� - ���������� ����� ����� �����
        ps_state = 1;
        ps_FocusUp();
        LCD_Draw();
      };
      if(GetMessage(MSG_EDITOR_KEY_DOWN))
      {//������ ������� ���� � ��������� - ���������� ����� ����� ����
        ps_state = 1;
        ps_FocusDown();
        LCD_Draw();
      };
      break;
    case 4: //������� ������ ����� ������
      if(GetMessage(MSG_DLG_OK)) //������ ������
      {
        ps_state = 1;
        SendMessage(MSG_USER_PASS_ENTERED);
        ParamScreenFullUpdate();
      };
      if(GetMessage(MSG_DLG_CANCEL)) //������ �� ������
      {
        ps_state = 1;
        ParamScreenFullUpdate();
      };
      break;
  }
}
*/