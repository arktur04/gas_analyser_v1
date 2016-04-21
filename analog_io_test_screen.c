//------------------------------------------------------------------------------
// Модуль реализует экран тестов аналоговых входов
//------------------------------------------------------------------------------
#include "type.h"
#include "lcd.h"
#include "font.h"
#include "bmps.h"
#include "buttons.h"
#include "message_window.h"
#include "variables.h"
#include "pass_controller.h"
#include "editor_screen.h"
#include "param_screen.h"

char ait_focus = 5;
char ait_password_entered  = 1;

void DrawAnaInOutScreen(void)
{
  SetFont(SMALL_FONT);
  
  LcdText(1, 0, 118, 7, "Левый термостат");
  
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
 /* 
  if(GetPasswordEntered()) LcdText(121, 1, 127, 7, "P");
  
  LcdText(1, 11, 41, 18, GetNameByTag(screens[ps_screen].var1));
  if(ait_focus == 0)
  {
    LcdSetColor(BLACK);
    LcdRect(43, 10, 85, 19);
    LcdSetColor(WHITE);
  };
  LcdText(44, 11, 85, 18, "9234.567");
  LcdSetColor(BLACK);
  LcdText(88, 11, 127, 18, GetUnitByTag(screens[ps_screen].var1));
  
  LcdText(1, 22, 41, 29, "variabl2");
  if(ait_focus == 1)
  {
    LcdSetColor(BLACK);
    LcdRect(43, 21, 85, 30);
    LcdSetColor(WHITE);
  };
  LcdText(44, 22, 85, 29, "9234.567");
  LcdSetColor(BLACK);    
  LcdText(88, 22, 127, 29, "some_tx2");

  LcdText(1, 33, 41, 40, "variabl3");
  if(ait_focus == 2)
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
  */
  //control buttons
  DrawButton(84, 53, BTN_OK, ait_focus == 6);
  
 // DrawMessageWindow("строка1", "строка2", "строка2", MB_OK + MB_CANCEL);
  
  DrawEditorScreen();
    
  LcdDraw();
}