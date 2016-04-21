/*************************************************************************
*    Main.c
**************************************************************************/

#include <intrinsics.h>
#include <stdio.h>
#include <nxp/iolpc2378.h>
#include "timers.h"
#include "type.h"
#include "irq.h"
#include "target.h"
#include "board.h"
#include "lcd.h"
#include "font.h"
#include "lcd_low_level.h"
#include "messages.h"
#include "keyb.h"
#include "bmps.h"
#include "cursor.h"
#include "checkbox.h"
#include "main_screen.h"
#include "param_screen.h"
#include "adc_fsm.h"
#include "dict.h"
#include "variables.h"
#include "editor_screen.h"
#include "menu_screen.h"
#include "douttest_screen.h"
#include "analog_io_test_screen.h"
#include "editor.h"
/*
//-------------------------------------------
#define A0 (1 << 19)  //P1.19
#define E (1 << 21)  //P1.21

#define D0 (1 << 22)  //P1.22
#define D1 (1 << 23)  //P1.23
#define D2 (1 << 24)  //P1.24
#define D3 (1 << 25)  //P1.25
#define D4 (1 << 26)  //P1.26
#define D5 (1 << 20)  //P1.20
#define D6 (1 << 26)  //P0.26
#define D7 (1 << 25)  //P0.25

#define SPI_MOSI (1 << 24)  //P0.24
#define SPI_SCLK (1 << 23)  //P0.23
#define SPI_CS   (1 << 31)  //P1.31

#define RESERVED0 (1 << 27)  //P1.27
#define RESERVED1 (1 << 28)  //P1.28
#define RESERVED2 (1 << 29)  //P1.29
#define RESERVED3 (1 << 0 )  //P0.0
#define RESERVED4 (1 << 12)  //P2.12
#define RESERVED5 (1 << 1 )  //P0.1
//-------------------------------------------
*/
#define TIMER0_TICK_PER_SEC   100 //10 ms timer cycle
#define TIMER0_IN_FREQ        72000000

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
int main(void)
{
  MamInit();
  // Init clock
  InitClock();
  // Init GPIO
  GpioInit();
  // Init VIC
  init_VIC();

  // Enable TIM0 clocks  
  PCONP_bit.PCTIM0 = 1; // enable clock  
  PCLKSEL0_bit.PCLK_TIMER0 = 1; //timer clock = pclk

  // Init Time0
  T0TCR_bit.CE = 0;     // counting  disable
  T0TCR_bit.CR = 1;     // set reset
  T0TCR_bit.CR = 0;     // release reset
  T0CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  T0MCR_bit.MR0I = 1;   // Enable Interrupt on MR0
  T0MCR_bit.MR0R = 1;   // Enable reset on MR0
  T0MCR_bit.MR0S = 0;   // Disable stop on MR0
  // set timer 0 period
  T0PR = 18-1;
  T0MR0 = (TIMER0_IN_FREQ)/(18 * TIMER0_TICK_PER_SEC);
  // init timer 0 interrupt
  T0IR_bit.MR0INT = 1;  // clear pending interrupt
  VIC_SetVectoredIRQ(Timer0IntrHandler,0,VIC_TIMER0);
  VICINTENABLE |= 1UL << VIC_TIMER0;
  T0TCR_bit.CE = 1;     // counting Enable
  
  __enable_interrupt();
  
  LcdInitPio();
  BoardInit();
  SysSpiInitPio();
  DacInitPio();
  AdcInitPio();
  InitAdc();
  LcdInitModule();
  LcdClear();
  
 // LcdLine(10,32,130, 32);
//  LcdLine(60,10,60, 60);
//  LcdLine(70,10,70, 60);
//  LcdRect(10,20,40, 50);
 // LcdRect(10,1,80, 7);

 /* SetFont(BIG_FONT);

  LcdLine(10, 28, 120, 28);
  LcdLine(10, 45, 120, 45);
 // LcdRect(10, 30, 50, 43);
  
  LcdText(50, 20, 200, 36, "-123456789.0");
  */
  /*
  LcdBmp(10, 20, BMP_HN_WIDTH, BMP_HN_HEIGHT, bmp_hn); 
  LcdBmp(10, 35, BMP_O2_WIDTH, BMP_O2_HEIGHT, bmp_o2); 
  DrawCheckBox(8, 8, 0, 1);
  DrawCheckBox(20, 8, 1, 1);*/
  
 // DrawMainScreen();
  DrawParamScreen();
  LcdClear();
  //DrawMenuScreen();
  
  DrawDOutTestScreen();
  
  LcdDraw();
  
  OutputSet(LCD_LED);

  InitMessages();
  InitKeyb();
  InitCursor();
  InitAdc();
  
//  SetCursorPos(100, 32);
  SendMessage(MSG_EDITOR_SCREEN_ACTIVATE);
  
  int debug = 0, debug2 = 0;
  char s[4];
  
  /*
      LcdClear();
      for(char i = 0; i < 16; i++)
        LcdLine(i * 8, 0, i * 8, 63);
      LcdDraw();
    */  
 // debug = ReadTemp() / 32;
  debug = 3855.0 + 20.0 * ((402.0 - 3855.0) / 20.0);
  unsigned int dac_codes[4] = {0, 0, 0, 0};
  dac_codes[0] = debug;
//  unsigned int dac_codes[4] = {0, 0, 0, 4095};
   WriteToDac(dac_codes);  
 // char adc_codes[4] = {0x58, 0, 0, 0};
 // char adc_codes2[4] = {0, 0, 0, 0,};
  
  char state = 0;
  while(1)
  {
/*
  WriteIntToFram(0, 100);
 
  ReadIntFromFram(0, &debug);  
 */ 
    /*
  WriteToDac(dac_codes);
    */
  /*
  switch(state)
    {
    case 0:
      //if(GetTimer(KEYB_TIMER) > 100)
      if(GetMessage(MSG_R_ENCODER_PRESSED))
      {
           OutputSet(LED11);
           OutputClr(LED12);
           OutputClr(LED13);
           OutputSet(LED15);
           ResetTimer(KEYB_TIMER);
           SendMessage(MSG_CUR_DEACTIVATE);
           state = 1;
      };
      break;
    case 1:
    //  if(GetTimer(KEYB_TIMER) > 100)
      if(GetMessage(MSG_R_ENCODER_PRESSED))      
      {
           OutputClr(LED11);
           OutputSet(LED12);
           OutputClr(LED13);
           OutputClr(LED15);           
           ResetTimer(KEYB_TIMER);
           SendMessage(MSG_CUR_ACTIVATE);
           state = 0;
      };
      break;
    case 2:
//      if(GetTimer(KEYB_TIMER) > 100)
      if(GetMessage(MSG_KEY_PRESSED))      
      {
           OutputClr(LED11);
           OutputClr(LED12);
           OutputSet(LED13);
           OutputClr(LED15);           
           ResetTimer(KEYB_TIMER);
           state = 3;
      };
      break;
    case 3:
     // if(GetTimer(KEYB_TIMER) > 100)
      if(GetMessage(MSG_KEY_PRESSED))      
      {
           OutputClr(LED11);
           OutputClr(LED12);
           OutputClr(LED13);
           OutputSet(LED15);           
           ResetTimer(KEYB_TIMER);
           state = 0;
      };
      break;      
    };
  
    if (GetMessage(MSG_L_ENCODER_CW))
    {
      debug++;
      sprintf(s, "%d", debug);
   //   LcdClear();
      LcdText(0, 0, 63, 9, s);  
      LcdDraw();
    };
     if (GetMessage(MSG_L_ENCODER_CCW))
    {
      debug--;
      sprintf(s, "%d", debug);
    //  LcdClear();
      LcdText(0, 0, 63, 9, s);  
      LcdDraw();      
    };
    
    if (GetMessage(MSG_R_ENCODER_CW))
    {
      debug2++;
      sprintf(s, "%d", debug2);
    //  LcdClear();
      LcdText(100, 0, 127, 9, s);  
      LcdDraw();
    };
     if (GetMessage(MSG_R_ENCODER_CCW))
    {
      debug2--;
      sprintf(s, "%d", debug2);
   //   LcdClear();
      LcdText(100, 0, 127, 9, s);  
      LcdDraw();      
    };
    */
 //   ProcessEditorScreen();
 //   ProcessEditor();
    
    ProcessAdc();
    ProcessCursor();
    ProcessKeyb();
    ProcessMessages();
  }
}
