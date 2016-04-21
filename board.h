/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2005
 **
 **    $Revision: 20579 $
 **
 ***************************************************************************/
#ifndef __BOARD_H
#define __BOARD_H

#include <intrinsics.h>
#include <nxp/iolpc2378.h>
#include "arm_comm.h"
#include "type.h"

#define I_RC_OSC_FREQ   (4MHZ)
#define MAIN_OSC_FREQ   (12MHZ)
#define RTC_OSC_FREQ    (32768UL)

//------------------------------------------------------------------------------
void Timer0IntrHandler (void);
void VIC_SetVectoredIRQ(void(*pIRQSub)(), unsigned int Priority,
                        unsigned int VicIntSource);
void VIC_Init(void);
void MamInit(void);
void GpioInit(void);
void InitClock(void);

//------------------------------------------------------------------------------
// FRAM and Temp sensor interface definition
//------------------------------------------------------------------------------
#define SYS_MOSI (1 << 6)  //P0.6
#define SYS_MISO (1 << 4)  //P0.4
#define SYS_SCLK (1 << 5)  //P0.5
#define FRAM_CS  (1 << 28) //P4.28
#define TEMP_CS  (1 << 7)  //P0.7

inline void SysMosiClr(void)
{
  FIO0CLR = SYS_MOSI;
}

inline void SysMosiSet(void)
{
  FIO0SET = SYS_MOSI;
}

inline void SysSclkClr(void)
{
  FIO0CLR = SYS_SCLK;
}

inline void SysSclkSet(void)
{
  FIO0SET = SYS_SCLK;
}

inline void SysFramCsSet(void)
{
  FIO4SET = FRAM_CS;
}

inline void SysFramCsClr(void)
{
  FIO4CLR = FRAM_CS;
}

inline void SysTempCsSet(void)
{
  FIO0SET = TEMP_CS;
}

inline void SysTempCsClr(void)
{
  FIO0CLR = TEMP_CS;
}

inline char GetSysMiso(void)
{
  return((FIO0PIN & SYS_MISO) ? 1 : 0);
}

void SysSpiInitPio(void);
void WriteIntToFram(unsigned int fram_add, signed int data);
void WriteFloatToFram(unsigned int fram_add, float data);
void ReadIntFromFram(unsigned int fram_add, signed int *data);
void ReadFloatFromFram(unsigned int fram_add, float *data);
//char SysSpiReadStatus(void);
int ReadTemp(void);
//------------------------------------------------------------------------------
//LCD board interface definitions
//-------------------------------------------
#define A0 (1 << 22)  //P1.19
#define E  (1 << 23)  //P1.21

#define D0 (1 << 19)  //P1.22
#define D1 (1 << 21)  //P1.23
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

#define LCD_LED  (1 << 0)
#define LCD_RES  (1 << 1)
#define LCD_E1   (1 << 2)
#define LCD_E2   (1 << 3)
#define KEY_ROW0 (1 << 4)
#define KEY_ROW1 (1 << 5)
#define KEY_ROW2 (1 << 6)
#define KEY_ROW3 (1 << 7)
#define LED0     (1 << 8)
#define LED1     (1 << 9)
#define LED2     (1 << 10)
#define LED3     (1 << 11)
#define LED4     (1 << 12) 
#define LED5     (1 << 13)
#define LED6     (1 << 14)
#define LED7     (1 << 15)
#define LED8     (1 << 16)
#define LED9     (1 << 17)
#define LED10    (1 << 18)
#define LED11    (1 << 19)
#define LED12    (1 << 20)
#define LED13    (1 << 21)
#define LED14    (1 << 22)
#define LED15    (1 << 23)

void BoardInit(void);

void LcdData(char data);

void LcdInitPio(void);

void OutputSet(unsigned long x);
void OutputClr(unsigned long x);

inline void LcdResSet(void)
{
  OutputSet(LCD_RES);
};

inline void LcdResClr(void)
{
  OutputClr(LCD_RES);
};

inline void LcdE1Set(void)
{
  OutputSet(LCD_E1);
};

inline void LcdE1Clr(void)
{
  OutputClr(LCD_E1);
};

inline void LcdE2Set(void)
{
  OutputSet(LCD_E2);
};

inline void LcdE2Clr(void)
{
  OutputClr(LCD_E2);
};

inline void LcdA0Set(void)
{
  FIO1SET = A0;
};

inline void LcdA0Clr(void)
{
  FIO1CLR = A0;
};

inline void LcdESet(void)
{
  FIO1SET = E;
};

inline void LcdEClr(void)
{
  FIO1CLR = E;
};

inline void SpiCsSet(void)
{
  FIO1SET = SPI_CS;
}

inline void SpiCsClr(void)
{
  FIO1CLR = SPI_CS;
}

inline void SpiMosiClr(void)
{
  FIO0CLR = SPI_MOSI;
}

inline void SpiMosiSet(void)
{
  FIO0SET = SPI_MOSI;
}

inline void SpiSclkClr(void)
{
  FIO0CLR = SPI_SCLK;
}

inline void SpiSclkSet(void)
{
  FIO0SET = SPI_SCLK;
}

void SpiWriteUInt32(unsigned long int UInt32);

void Delay(int delay);

char GetKbdBus(void);

//------------------------------------------------------------------------------
// DAC interface definitions
//------------------------------------------------------------------------------
#define DAC_SYNC (1 << 9)  //P0.9
#define DAC_SCLK (1 << 8)  //P0.8
#define DIN0     (1 << 6)  //P2.6
#define DIN1     (1 << 5)  //P2.5
#define DIN2     (1 << 4)  //P2.4
#define DIN3     (1 << 3)  //P2.3

inline void DacSyncClr(void)
{
  FIO0CLR = DAC_SYNC;
}

inline void DacSyncSet(void)
{
  FIO0SET = DAC_SYNC;
}

inline void DacSclkClr(void)
{
  FIO0CLR = DAC_SCLK;
}

inline void DacSclkSet(void)
{
  FIO0SET = DAC_SCLK;
}

inline void DinClr(char n)
{
  switch(n)
  {
  case 0: FIO2CLR = DIN0;
  break;
  case 1: FIO2CLR = DIN1;
  break;
  case 2: FIO2CLR = DIN2;
  break;
  case 3: FIO2CLR = DIN3;
  break;  
  };
}

inline void DinSet(char n)
{
  switch(n)
  {
  case 0: FIO2SET = DIN0;
  break;
  case 1: FIO2SET = DIN1;
  break;
  case 2: FIO2SET = DIN2;
  break;
  case 3: FIO2SET = DIN3;
  break;  
  };
}

void DacInitPio(void);
void WriteToDac(unsigned int value[4]);
//------------------------------------------------------------------------------
// ADC interface definitions
//------------------------------------------------------------------------------
#define _16_BIT 16
#define _24_BIT 24
#define BITS _24_BIT

#define ADC_CLK   (1 << 8)   //P2.8
#define ADC_KEY   (1 << 11)  //P2.11
#define ADC0_MISO (1 << 16)  //P0.16
#define ADC0_MOSI (1 << 15)  //P0.15
#define ADC1_MISO (1 << 17)  //P0.17
#define ADC1_MOSI (1 << 18)  //P0.18
#define ADC2_MISO (1 << 19)  //P0.19
#define ADC2_MOSI (1 << 20)  //P0.20
#define ADC3_MISO (1 << 21)  //P0.21
#define ADC3_MOSI (1 << 22)  //P0.22

inline void AdcClkClr(void)
{
  FIO2CLR = ADC_CLK;
}

inline void AdcClkSet(void)
{
  FIO2SET = ADC_CLK;
}

inline void AdcKeyClr(void)
{
  FIO2CLR = ADC_KEY;
}

inline void AdcKeySet(void)
{
  FIO2SET = ADC_KEY; 
}
inline void AdcMosiClr(char n)
{
  switch(n)
  {
  case 0: FIO0CLR = ADC0_MOSI;
  break;
  case 1: FIO0CLR = ADC1_MOSI;
  break;
  case 2: FIO0CLR = ADC2_MOSI;
  break;
  case 3: FIO0CLR = ADC3_MOSI;
  break;  
  };
}

inline void AdcMosiSet(char n)
{
  switch(n)
  {
  case 0: FIO0SET = ADC0_MOSI;
  break;
  case 1: FIO0SET = ADC1_MOSI;
  break;
  case 2: FIO0SET = ADC2_MOSI;
  break;
  case 3: FIO0SET = ADC3_MOSI;
  break;  
  };
}

inline char GetAdcMiso(char n)
{
  switch(n)
  {
  case 0: return((FIO0PIN & ADC0_MISO) ? 1 : 0);
  case 1: return((FIO0PIN & ADC1_MISO) ? 1 : 0);
  case 2: return((FIO0PIN & ADC2_MISO) ? 1 : 0);
  case 3: return((FIO0PIN & ADC3_MISO) ? 1 : 0);
  };
  return 0;
}

void AdcInitPio(void);
void WriteByteToAdc(char bytes[4]);
void WriteByteToAllAdc(char byte);
void ReadByteFromAdc(char bytes[4]);
void GetAdcValue(unsigned int values[4]);
char RdyIsLow(void);
//------------------------------------------------------------------------------
#define IRQ_FLAG            0x80
#define FIQ_FLAG            0x40

#endif /* __BOARD_H */
