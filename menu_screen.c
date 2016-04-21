//------------------------------------------------------------------------------
// ������ ��������� ����� ��������� ����������
//------------------------------------------------------------------------------
#include "type.h"
#include "lcd.h"
#include "font.h"
#include "bmps.h"
#include "buttons.h"
/*
#include "pass_controller.h"
#include "message_window.h"
#include "variables.h"
#include "editor_screen.h"
*/
#include "menu_screen.h"

/*
typedef struct{
  char header[25]; //��������� ������
  char buttons[8][13]; //������� �� ������� ����
  char btn_allowed[8]; //������ ���������
  int number; //����� ������
  char back_allowed; //������ "�����" ���������
  char home_allowed; //������ "������� �����" ���������
  char forward_allowed; //������ "������" ���������
} T_MENU_DESCRIPTOR;
*/
static T_MENU_DESCRIPTOR menues[1] = {
  {"������� ����",
  {"�. ���������", "�. ���������",
   "�. ��������", "�. ��������",
   "�. ��������.", "�. ��������.",
   "������", "�����"
  },
  {1, 1, 1, 1, 1, 1, 1, 1},
  0,
  FALSE, TRUE, TRUE
  }};
/*
T_SCREEN_DESCRIPTOR screens[4] = {
  {"��������� ����� (1/2)",  SHIFT_T_L, K_CD_T_L, T_F_T_L,  ADJ_S_T_L,  0, FALSE, TRUE, TRUE},
  {"��������� ����� (2/2)",  K_P_T_L,   K_D_T_L,  T_DF_T_L, SH_CLC_T_L, 1, TRUE, TRUE, TRUE},
  {"��������� ������ (1/2)", SHIFT_T_R, K_CD_T_R, T_F_T_R,  ADJ_S_T_R,  2, TRUE, TRUE, TRUE},
  {"��������� ������ (2/2)", K_P_T_R,   K_D_T_R,  T_DF_T_R, SH_CLC_T_R, 3, TRUE, TRUE, FALSE}};
*/

char ms_focus = 5;
char ms_screen = 0;

void DrawMenuScreen(void)
{
  SetFont(SMALL_FONT);
  LcdText(1, 0, 118, 7, menues[ms_screen].header);
  LcdLine(0, 8, 127, 8);  
  LcdLine(0, 9, 127, 9);
  
  for(char x = 0; x < 2; x++)
    for(char y = 0; y < 4; y++)
      DrawTextButton(x * 64, y * 11 + 9, x* 64 + 63, y * 11 + 20, 
                     menues[0].buttons[y * 2 + x],
                     ms_focus == (y * 2 + x));

  DrawButton(0, 53, BTN_BACK, ms_focus == 8);
  DrawButton(42, 53, BTN_HOME, ms_focus == 9);
  DrawButton(84, 53, BTN_FORWARD, ms_focus == 10);
  

  LcdDraw();
}


