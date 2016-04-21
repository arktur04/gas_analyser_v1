#include "lcd.h"
#include "font.h"
#include "bmps.h"
#include "main_screen.h"

void DrawMainScreen(void)
{ 
  SetFont(SMALL_FONT);
  LcdText(44, 0, 84, 8, "Левый");
  LcdText(88, 0, 127, 8, "Правый");
  
  LcdLine(0, 8, 127, 8);
  LcdLine(0, 9, 127, 9);
  LcdBmp(0, 12, BMP_HN_WIDTH, BMP_HN_HEIGHT, bmp_hn);
  
  LcdLine(0, 25, 127, 25);
  LcdBmp(0, 27, BMP_O2_WIDTH, BMP_O2_HEIGHT, bmp_o2); 
  
  LcdLine(0, 41, 127, 41);
  SetFont(SMALL_FONT);
  
  LcdText(0, 43, 38, 51, "T, oC");
  LcdLine(0, 52, 127, 52);
  LcdLine(0, 53, 127, 53);
  
  LcdText(0, 55, 38, 63, "Tх25,7C");
  //----------------------------------------------------------------------------
//  LcdSetColor(WHITE);
  LcdText(1, 0, 38, 7, "УралЭКО");
//  LcdSetColor(BLACK);
  
  LcdText(41, 55, 127, 63, "Уралэкоавтоматика");
  /*
  LcdSetColor(WHITE);
  LcdBmp(37, 55, 9, 8, bmp_small_tp);
  LcdBmp(46, 55, 9, 8, bmp_small_che); 
  LcdBmp(55, 55, 9, 8, bmp_small_ne); 
  LcdBmp(64, 55, 9, 8, bmp_small_o2); 
  LcdBmp(73, 55, 9, 8, bmp_small_hn);
  
  LcdBmp(83, 55, 9, 8, bmp_small_tp);
  LcdBmp(92, 55, 9, 8, bmp_small_che); 
  LcdBmp(101, 55, 9, 8, bmp_small_ne); 
  LcdBmp(110, 55, 9, 8, bmp_small_o2); 
  LcdBmp(119, 55, 9, 8, bmp_small_hn);     
  LcdSetColor(BLACK);
  */
  //----------------------------------------------------------------------------
  LcdLine(39, 0, 39, 51);
  LcdLine(84, 0, 84, 51);
  
  SetFont(BIG_FONT);
  LcdText(43, 12, 83, 24, "1234");
  LcdText(88, 12, 127, 24, "5678");
  
  LcdText(43, 28, 83, 40, "19,78");
  LcdText(88, 28, 127, 40, "21,54");
  
  SetFont(SMALL_FONT);
  LcdText(43, 43, 83, 51, "650,4");
  LcdText(88, 43, 127, 51, "654,2");
}