//------------------------------------------------------------------------------
// Модуль реализует отрисовку кнопок
//------------------------------------------------------------------------------
#ifndef BUTTONS_h
#define BUTTONS_h

#define BTN_OK      0
#define BTN_CANCEL  1
#define BTN_DEFAULT 2
#define BTN_FORWARD 3
#define BTN_BACK    4
#define BTN_HOME    5

void DrawTextButton(char x1, char y1, char x2, char y2, char *text, char active);

void DrawButton(char x, char y, char btn_id, char active);

#endif
