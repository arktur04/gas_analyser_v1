//------------------------------------------------------------------------------
// Модуль реализует функции работы с дисплеем
//------------------------------------------------------------------------------
#include "lcd.h"
#include "lcd_low_level.h"

//#include "logo.h"
#include "font.h"
#include "string.h"
//#include "pio.h"

char curr_color = BLACK;
char curr_invert_mode = NON_INVERT_MODE;

char video_buff[128][8];
char pages[16];
//+---------+---------+
//| page 0  | page 8  |
//+---------+---------+
//   ...        ...
//+---------+---------+
//| page 7  | page 15 |
//+---------+---------+

void LcdInit(void)
{
  LcdInitModule();
}

void LcdSetColor(char color)
{
  curr_color = color;
}

void LcdSetInvertMode(char invert_mode)
{
  curr_invert_mode = invert_mode;
}

//------------------------------------------------------------------------------
// Ф-ция передает в LCD содержимое видеобуфера
//------------------------------------------------------------------------------
void LcdDraw(void)
{
  char sh1, sh2;
  for(char p = 0; p < 2; p++) 
  {
    sh1 = p * 8;
    sh2 = p * 64;
    for (char i = 0; i < 8; i++)
      if (pages[i + sh1])
      {
        LcdSetPage(p, i);
        for (char j = 0; j < 64; j++)
          LcdWriteByte(p, 1, video_buff[j + sh2][i]);
      };
    };
/*  for (i = 0; i < 8; i++)
    if (pages[i + 8])
    {
      LcdSetPage(1, i);
      for (j = 0; j < 64; j++)
        LcdWriteByte(1, 1, video_buff[j + 64][i]);
    };*/
 for (char i = 0; i < 16; i++)
   pages[i] = 0;
}

//------------------------------------------------------------------------------
// Ф-ция очищает экран
//------------------------------------------------------------------------------
void LcdClear(void)
{
  int i, j;
  for (i = 0; i < 128; i++)
    for (j = 0; j < 8; j++)
      video_buff[i][j] = 0x00;
  for (i = 0; i < 16; i++)
    pages[i] = 1;
}

//draw 8-pixel vertical stroke according to current color and invert mode
inline void LcdStroke(char page, char x, char stroke) 
{
  if (curr_color) // ^ curr_invert_mode)
  {
    video_buff[x][page] &= ~stroke;
  }
  else
  {
    video_buff[x][page] |= stroke;
  }
}
//------------------------------------------------------------------------------
// Ф-ция рисует горизонтальную линию черного цвета
// Параметры:
// x1, y1, x2 - координаты
//------------------------------------------------------------------------------
void LCD_horline(char x1, char y1, char x2)
{
  char i, page, mask;
  page = y1 / 8;
  mask = 0x01 << (y1 % 8);
  for(i = x1; i <= x2; i++)
  {
    LcdStroke(page, i, mask);
  };
  if (x1 < 64) pages[page] = 1;
  if (x2 >= 64) pages[page + 8] = 1;
}
//------------------------------------------------------------------------------
// Ф-ция рисует вертикальную линию черного цвета
// Параметры:
// x1, y1, у2 - координаты
//------------------------------------------------------------------------------
void LCD_vertline(char x1, char y1, char y2)
{
  char b, page1, page2, mask1, mask2, i;
  mask1 = 0xFF << (y1 % 8);
  mask2 = 0xFF >> (7 - (y2 % 8));
  page1 = y1 / 8;
  page2 = y2 / 8;
  if(page1 == page2)
  {
    LcdStroke(page1, x1, mask1 & mask2);
 /*   
    b = video_buff[x1][page1];
    mask1 &= mask2;
    b |= mask1;
    video_buff[x1][page1] = b;
    */
  }
  else
  {

    LcdStroke(page1, x1, mask1);
    //video_buff[x1][page1] = b;
    for(i = page1 + 1; i < page2; i++)
      LcdStroke(i, x1, 0xFF);
      //video_buff[x1][i] = 0xFF;
    LcdStroke(page2, x1, mask2);
   // video_buff[x1][page2] = b;
  };
  for (i = page1; i <= page2; i++)
    if (x1 < 64)
      pages[i] = 1;
    else
      pages[i + 8] = 1;
};
//------------------------------------------------------------------------------
// Ф-ция рисует прямоугольник черного цвета
// Параметры:
// x1, y1, х2, у2 - координаты
//------------------------------------------------------------------------------
void LCD_rect(char x1, char y1, char x2, char y2)
{
  char b, page1, page2, mask1, mask2, i, j;
  mask1 = 0xFF << (y1 % 8);
  mask2 = 0xFF >> (7 - (y2 % 8));
  page1 = y1 / 8;
  page2 = y2 / 8;
  if(page1 == page2)
  {
    mask1 &= mask2;
    for(i = x1; i <= x2; i++)
    {
     // video_buff[i][page1] = b;
      LcdStroke(page1, i, mask1);
    }
  }
  else
  {
    for(i = x1; i <= x2; i++)
    {
      LcdStroke(page1, i, mask1);
    };
    for(j = page1 + 1; j < page2; j++)
      for(i = x1; i <= x2; i++)
        LcdStroke(j, i, 0xFF);
        //video_buff[i][j] = 0xFF;
    for(i = x1; i <= x2; i++)
    {
      LcdStroke(page2, i, mask2);
      //video_buff[i][page2] = b;
    };
  };
  for (i = page1; i <= page2; i++)
  {
    if (x1 < 64) pages[i] = 1;
    if (x2 >= 64) pages[i + 8] = 1;
  };
};
/*
//------------------------------------------------------------------------------
// Ф-ция выводит на экран изображение
// Параметры:
// x1, y1 - координаты
// w, h - ширина и высота
// *address - адрес массива с изображением
//------------------------------------------------------------------------------
void LcdBmp(char x1, char y1, char w, char h, char *address)
{
  char page1, page2, x2, y2, b, shift, x, y = 0, y_, p_;
  x2 = x1 + w - 1;
  y2 = y1 + h - 1;
  page1 = y1 / 8;
  page2 = y2 / 8;
  shift = y1 % 8;
  if (x2 > 127) x2 = 127;
  if (y1 > 63) y2 = 63;
  
  y_ = y1 + ((h - 1) / 8) * 8;
  p_ = page2 * 8;
  
  for (char j = page1; j <= page2; j++)
  {
    x = 0;
    for (char i = x1; i <= x2; i++)
    {
      if (j == page1)
      {
        b = video_buff[i][j] | (*(address + x) << shift);
      }
      else
        if ((j == page2) && (y_ <= p_))
        {
          b = video_buff[i][j] | (*(address + ((y - 1) * w) + x) >> (8 - shift));
        }
        else
        {
          b = video_buff[i][j] | ((*(address + (y * w) + x) << shift) | 
            (*(address + ((y - 1) * w) + x) >> (8 - shift)));
        };
      LcdStroke(j, i, b);
      x++;
    };
    y++;
  };
}
*/
//------------------------------------------------------------------------------
// Ф-ция выводит на экран изображение
// Параметры:
// x1, y1 - координаты
// w, h - ширина и высота
// *address - адрес массива с изображением
//------------------------------------------------------------------------------
void LcdBmp(char x1, char y1, char w, char h, char *address)
{
  char page1, page2, x2, y2, b, shift, x, y = 0, y_, p_;
  x2 = x1 + w - 1;
  y2 = y1 + h - 1;
  
  LcdSetColor(~curr_color);
  LCD_rect(x1, y1, x2, y2);
  LcdSetColor(~curr_color);
  
  page1 = y1 / 8;
  page2 = y2 / 8;
  shift = y1 % 8;
  if (x2 > 127) x2 = 127;
  if (y1 > 63) y2 = 63;
  
  y_ = y1 + ((h - 1) / 8) * 8;
  p_ = page2 * 8;
  
  for (char j = page1; j <= page2; j++)
  {
    x = 0;
    for (char i = x1; i <= x2; i++)
    {
      if (j == page1)
      {
        b = (*(address + x) << shift);
      }
      else
        if ((j == page2) && (y_ <= p_))
        {
          b = (*(address + ((y - 1) * w) + x) >> (8 - shift));
        }
        else
        {
          b = ((*(address + (y * w) + x) << shift) | 
            (*(address + ((y - 1) * w) + x) >> (8 - shift)));
        };
      LcdStroke(j, i, b);
      x++;
    };
    y++;
  };
}
/*
//------------------------------------------------------------------------------
// Ф-ция выводит на экран текст черного цвета
// Параметры:
// x1, y1 - координаты
// str - текст
//------------------------------------------------------------------------------
void LCD_textout(char x1, char y1, char *str)
{
  char page, mask1, mask2, i, j, x, shift, b, ch;
  page = y1 / 8;
  if((y1 % 8) == 0)
  {
    x = x1;
    for(i = 0; i < strlen(str); i++)
    {
      ch = LCD_Font(str[i]);
      for(j = 0; j < 5; j++)
      {
        video_buff[x][page] = font[ch][j];
        x++;
      };
    };
    if (x1 < 64) pages[page] = 1;
    if (x - 1 >= 64) pages[page + 8] = 1;
  }
  else
  {
    x = x1;
    shift = y1 % 8;
    mask2 = 0xFF << shift;
    for(i = 0; i < strlen(str); i++)
    {
      ch = LCD_Font(str[i]);
      for(j = 0; j < 5; j++)
      {
        b = video_buff[x][page];
        mask1 = font[ch][j];
        mask1 = mask1 << shift;
        b = (b & ~mask2) | mask1;
        video_buff[x][page] = b;
        x++;
      }
    };

    x = x1;
    shift = 8 - (y1 % 8);
    mask2 = 0xFF >> shift;
    for(i = 0; i < strlen(str); i++)
    {
      ch = LCD_Font(str[i]);
      for(j = 0; j < 5; j++)
      {
        b = video_buff[x][page + 1];
        mask1 = font[ch][j];
        mask1 = mask1 >> shift;
        b = (b & ~mask2) | mask1;
        video_buff[x][page + 1] = b;
        x++;
      };
    };
    if (x1 < 64)
    {
      pages[page] = 1;
      pages[page + 1] = 1;
    };
    if (x - 1 >= 64)
    {
      pages[page + 8] = 1;
      pages[page + 9] = 1;
    };
  }
}
*/
inline void NormalizeCoord(char *x1, char *y1, char *x2, char *y2)
{
  if (*x1 > *x2) *x2 ^= *x1 ^= *x2 ^= *x1;
  if (*y1 > *y2) *y2 ^= *y1 ^= *y2 ^= *y1;
  if (*x1 > 127) *x1 = 127;
  if (*y1 > 63) *y1 = 63;
  if (*x2 > 127) *x2 = 127;
  if (*y2 > 63) *y2 = 63;
}

void LcdLine(char x1, char y1, char x2, char y2)
{
  NormalizeCoord(&x1, &y1, &x2, &y2);
  if (x1 == x2)
  {
    LCD_vertline(x1, y1, y2);
  }
  else
  {
    if (y1 == y2)
    {
      LCD_horline(x1, y1, x2);
    }
    else
    {
      // diagonal lines not supported yet
    };
  };
} 

void LcdRect(char x1, char y1, char x2, char y2)
{
  NormalizeCoord(&x1, &y1, &x2, &y2);
  LCD_rect(x1, y1, x2, y2);
}

void LcdFrame(char x1, char y1, char x2, char y2)
{
  NormalizeCoord(&x1, &y1, &x2, &y2);
  LcdLine(x1, y1, x2, y1);
  LcdLine(x2, y1, x2, y2);
  LcdLine(x1, y2, x2, y2);
  LcdLine(x1, y1, x1, y2);
}

inline void SetPixel(char x, char y)
{
  LcdStroke(y / 8, x, 1 << (y % 8));
}

void LcdText(char x1, char y1, char x2, char y2, char *text)
{
  NormalizeCoord(&x1, &y1, &x2, &y2);
  LcdSetColor(~curr_color);
  LCD_rect(x1, y1, x2, y2);
  LcdSetColor(~curr_color);
  // text out
  char x = x1;
  char fh = GetFontHeight();
   
  for(char i = 0; (i < strlen(text)) && (x < x2); i++)
  {
    char ch = text[i];
    for(char j = 0; (j < GetCharWidth(ch)) && (x < x2); j++)
    {
      char y = y1;
      for(char k = 0; k < fh; k++)
      {
        if(GetFontData(ch, j) & (1 << k))
          SetPixel(x, y);  //!
        y++;
      };
      x++;
    }
  }
  
}
