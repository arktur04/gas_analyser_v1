#ifndef KEYB_h
#define KEYB_h

//-------------------------------------------------
// Коды кнопок
//-------------------------------------------------

#define KEY_0     1
#define KEY_1     2
#define KEY_2     3
#define KEY_3     4
#define KEY_4     5
#define KEY_5     6
#define KEY_6     7
#define KEY_7     8
#define KEY_8     9
#define KEY_9     10
#define KEY_ENTER 11
#define KEY_MINUS 12
#define KEY_DOT   13
#define KEY_ESC   14
#define Key_MENU  15

#define KEY_UP    16
#define KEY_DOWN  17
#define KEY_LEFT  18
#define KEY_RIGHT 19

#define KEY_DEL   20 // Shift + Esc

#define debounce 10
#define first_delay 50
#define auto_repeat 10

void InitKeyb(void);
void ProcessKeyb(void);
char GetKeyCode(void);
char KeyCodeToASCII(char code);

//int GetScanCode(void);
#endif
