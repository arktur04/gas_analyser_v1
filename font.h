//------------------------------------------------------------------------------
// ׁטסעולםûי רנטפע
//------------------------------------------------------------------------------
#ifndef FONT_h
#define FONT_h

#define SMALL_FONT 0
#define BIG_FONT 1

void SetFont(char font);

char GetFontHeight(void);
char GetCharWidth(char ch);
unsigned long GetFontData(char ch, char x);

#endif
