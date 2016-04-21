#include "board.h"
#include <nxp/iolpc2378.h>
#include "timers.h"

/*************************************************************************
 * Function Name: fiq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: FIQ handler
 *
 *************************************************************************/

__fiq __arm void FIQ_Handler (void)
{
  while(1);
}

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ handler
 *
 *************************************************************************/

__irq __arm void IRQ_Handler (void)
{
void (*interrupt_function)();
unsigned int vector;

  vector = VICADDRESS;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  if(interrupt_function != NULL)
  {
    interrupt_function();  // Call vectored interrupt function.
  }
  else
  {
    VICADDRESS = 0;      // Clear interrupt in VIC.
  }
}

/*************************************************************************
 * Function Name: Timer0IntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void Timer0IntrHandler (void)
{
  ProcessTimers();
  
  // clear interrupt
  T0IR_bit.MR0INT = 1;
  VICADDRESS = 0;
}

/*************************************************************************
 * Function Name: VIC_SetVectoredIRQ
 * Parameters:  void(*pIRQSub)()
 *              unsigned int VicIrqSlot
 *              unsigned int VicIntSouce
 *
 * Return: void
 *
 * Description:  Init vectored interrupts
 *
 *************************************************************************/
void VIC_SetVectoredIRQ(void(*pIRQSub)(), unsigned int Priority,
                        unsigned int VicIntSource)
{
unsigned long volatile *pReg;
  // load base address of vectored address registers
  pReg = &VICVECTADDR0;
  // Set Address of callback function to corresponding Slot
  *(pReg+VicIntSource) = (unsigned long)pIRQSub;
  // load base address of ctrl registers
  pReg = &VICVECTPRIORITY0;
  // Set source channel and enable the slot
  *(pReg+VicIntSource) = Priority;
  // Clear FIQ select bit
  VICINTSELECT &= ~(1<<VicIntSource);
}

/*************************************************************************
 * Function Name: VIC_Init
 * Parameters: void
 * Return: void
 *
 * Description: Initialize VIC
 *
 *************************************************************************/
void VIC_Init(void)
{
volatile unsigned long * pVecAdd, *pVecCntl;
int i;
  // Assign all interrupt channels to IRQ
  VICINTSELECT  =  0;
  // Disable all interrupts
  VICINTENCLEAR = 0xFFFFFFFF;
  // Clear all software interrupts
  VICSOFTINTCLEAR = 0xFFFFFFFF;
  // VIC registers can be accessed in User or privileged mode
  VICPROTECTION = 0;
  // Clear interrupt
  VICADDRESS = 0;

  // Clear address of the Interrupt Service routine (ISR) for vectored IRQs
  // and disable all vectored IRQ slots
  for(i = 0,  pVecCntl = &VICVECTPRIORITY0, pVecAdd = &VICVECTADDR0; i < 32; ++i)
  {
    *pVecCntl++ = *pVecAdd++ = 0;
  }
}

void MamInit(void)
{
  // MAM init
  MAMCR_bit.MODECTRL = 0;
  MAMTIM_bit.CYCLES = 3;    // FCLK > 40 MHz
  MAMCR_bit.MODECTRL = 2;   // MAM functions fully enabled
}
/*************************************************************************
 * Function Name: InitClock
 * Parameters: void
 * Return: void
 *
 * Description: Initialize PLL and clocks' dividers. Hclk - 288MHz,
 * Usbclk - 48MHz
 *
 *************************************************************************/
void InitClock(void)
{
  // 1. Init OSC
  SCS_bit.OSCRANGE = 0;
  SCS_bit.OSCEN = 1;
  // 2.  Wait for OSC ready
  while(!SCS_bit.OSCSTAT);
  // 3. Disconnect PLL
  PLLCON_bit.PLLC = 0;
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
  // 4. Disable PLL
  PLLCON_bit.PLLE = 0;
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
  // 5. Select source clock for PLL
  CLKSRCSEL_bit.CLKSRC = 1; // Selects the main oscillator as a PLL clock source.
  // 6. Set PLL settings 288 MHz
  PLLCFG_bit.MSEL = 48-1;   //!!!!!!!!!!!!!!!!!!!!!!!!!!!  // for oscillator = 6 MHz; for 12 MHz M = 24-1 
  PLLCFG_bit.NSEL = 2-1;
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
  // 7. Enable PLL
  PLLCON_bit.PLLE = 1;
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
  // 8. Wait for the PLL to achieve lock
  while(!PLLSTAT_bit.PLOCK);
  // 9. Set clk divider settings
  CCLKCFG   = 4-1;            // 1/4 Fpll = 72MHz
  USBCLKCFG = 6-1;            // 1/6 Fpll - 48 MHz
  PCLKSEL0 = PCLKSEL1 = 0;    // other peripherals
  // 10. Connect the PLL
  PLLCON_bit.PLLC = 1;
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
}

/*************************************************************************
 * Function Name: GpioInit
 * Parameters: void
 * Return: void
 *
 * Description: Reset all GPIO pins to default: primary function
 *
 *************************************************************************/
void GpioInit(void)
{
  SCS_bit.GPIOM = 1; //
  PCLKSEL1_bit.PCLK_GPIO = 1; //gpio clock = pclk
  // Set to inputs
  IO0DIR  = \
  IO1DIR  = \
  FIO0DIR = \
  FIO1DIR = \
  FIO2DIR = \
  FIO3DIR = \
  FIO4DIR = 0;

  // Enable Fast GPIO0,1
  SCS_bit.GPIOM = 1;

  // clear mask registers
  FIO0MASK =\
  FIO1MASK =\
  FIO2MASK =\
  FIO3MASK =\
  FIO4MASK = 0;

  // Reset all GPIO pins to default primary function
  PINSEL0 =\
  PINSEL1 =\
  PINSEL2 =\
  PINSEL3 =\
  PINSEL4 =\
  PINSEL5 =\
  PINSEL6 =\
  PINSEL7 =\
  PINSEL8 =\
  PINSEL9 =\
  PINSEL10= 0;
}

//------------------------------------------------------------------------------

#define INIT_STATE 0 

unsigned long board_state;

void BoardInit(void)
{
  board_state = INIT_STATE;
  SpiWriteUInt32(board_state); 
}

void OutputSet(unsigned long x)
{
  board_state |= x;
  SpiWriteUInt32(board_state);
}

void OutputClr(unsigned long x)
{
  board_state &= ~x;
  SpiWriteUInt32(board_state);
}

void SpiWriteUInt32(unsigned long UInt32)
{
  SpiCsClr();
  for (char i = 0; i < 24; i++)
  {
    SpiSclkClr();
   // Delay();
    if(UInt32 & (1 << (23 - i)))
    {
      SpiMosiSet();
    }
    else
    {
      SpiMosiClr();
    };
    SpiSclkSet();
  };
 // Delay();
  SpiCsSet();
 // Delay();
}

void LcdInitPio(void)
{
  FIO0DIR = D6 | D7 | SPI_MOSI | SPI_SCLK;
  FIO1DIR = A0 | E | D0 | D1 | D2 | D3 | D4 | D5 | SPI_CS;
}

void LcdData(char data)
{
  if (data & 0x01)
  {
    FIO1SET = D0;
  }
  else
  {
    FIO1CLR = D0;
  };

  if (data & 0x02)
  {
    FIO1SET = D1;
  }
  else
  {
    FIO1CLR = D1;
  };

  if (data & 0x04)
  {
    FIO1SET = D2;
  }
  else
  {
    FIO1CLR = D2;
  };
  
  if (data & 0x08)
  {
    FIO1SET = D3;
  }
  else
  {
    FIO1CLR = D3;
  };  
 
  if (data & 0x10)
  {
    FIO1SET = D4;
  }
  else
  {
    FIO1CLR = D4;
  };
  
  if (data & 0x20)
  {
    FIO1SET = D5;
  }
  else
  {
    FIO1CLR = D5;
  };  
  
  if (data & 0x40)
  {
    FIO0SET = D6;
  }
  else
  {
    FIO0CLR = D6;
  };    
  
  if (data & 0x80)
  {
    FIO0SET = D7;
  }
  else
  {
    FIO0CLR = D7;
  };    
}

void Delay(int delay)
{
  for(; delay > 0; delay--);
}

char GetKbdBus(void)
{
  char result;
  unsigned long bus;
  FIO1DIR &= ~(D0 | D1 | D2 | D3 | D4 | D5);
  Delay(1);
  bus = FIO1PIN;
  result  = ((bus & D0) ? 0x01 : 0);
  result |= ((bus & D1) ? 0x02 : 0);
  result |= ((bus & D2) ? 0x04 : 0);
  result |= ((bus & D3) ? 0x08 : 0);
  result |= ((bus & D4) ? 0x10 : 0);
  result |= ((bus & D5) ? 0x20 : 0);
  
  FIO1DIR |= (D0 | D1 | D2 | D3 | D4 | D5);
  return result;
}
//------------------------------------------------------------------------------
// Fram and temp sensor interface
//------------------------------------------------------------------------------
void SysSpiWriteByte(char byte);

void SysSpiInitPio(void)
{
  FIO0DIR |= SYS_MOSI | SYS_SCLK | TEMP_CS;
  FIO4DIR |= FRAM_CS;
  SysFramCsSet();
  SysTempCsSet();
  //-----------------------------------------------
  SysFramCsClr();
  SysSpiWriteByte(0x06); //WREN opcode
  SysFramCsSet();
}

void SysSpiWriteByte(char byte)
{
  for (char i = 0; i < 8; i++)
  {
   // Delay(5);
    SysSclkClr();
   // Delay(5);
    if(byte & (1 << (7 - i)))
      SysMosiSet();
    else
      SysMosiClr();
    SysSclkSet();
  };
 // Delay();
//  SpiCsSet();
 // Delay();
}

char SysSpiReadByte(void)
{
  char byte = 0;
  
  SysSclkSet();
  for (char i = 0; i < 8; i++)
  {
    SysSclkClr();
  //  Delay(10);
    SysSclkSet();
    byte |= GetSysMiso() << (7 - i);
  };
  return byte;
 // Delay();
//  SpiCsSet();
 // Delay();
}

void SysSpiWriteBuffer(unsigned int fram_addr, char *buffer, char size)
{
  char addr_lo, addr_hi;
  addr_lo = fram_addr % 256;
  addr_hi = fram_addr / 256;
 /* 
  SysFramCsClr();
  SysSpiWriteByte(0x06); //WREN opcode
  SysFramCsSet();
  Delay(10);
  */
  SysFramCsClr();
  SysSpiWriteByte(0x02); //WRITE opcode
  SysSpiWriteByte(addr_hi);
  SysSpiWriteByte(addr_lo);
  for(char i = 0; i < size; i++)
    SysSpiWriteByte(*(buffer + i));
  SysFramCsSet();
  /*
  Delay(10);  
  SysFramCsClr();
  SysSpiWriteByte(0x04); //WRDI opcode
  SysFramCsSet();*/
}

void SysSpiReadBuffer(unsigned int fram_addr, char *buffer, char size)
{
  char addr_lo, addr_hi;
  addr_lo = fram_addr % 256;
  addr_hi = fram_addr / 256;
    
  SysFramCsClr();
  SysSpiWriteByte(0x03); //READ opcode
  SysSpiWriteByte(addr_hi);
  SysSpiWriteByte(addr_lo);
  for(char i = 0; i < size; i++)
    *(buffer + i) = SysSpiReadByte();
  SysFramCsSet();
}

void WriteIntToFram(unsigned int fram_add, signed int data)
{
  SysSpiWriteBuffer(fram_add, (char*)&data, sizeof data);
}

void WriteFloatToFram(unsigned int fram_add, float data)
{
  SysSpiWriteBuffer(fram_add, (char*)&data, sizeof data);
}

void ReadIntFromFram(unsigned int fram_add, signed int *data)
{
  SysSpiReadBuffer(fram_add, (char*)data, sizeof data);
}

void ReadFloatFromFram(unsigned int fram_add, float *data)
{
  SysSpiReadBuffer(fram_add, (char*)data, sizeof data);
}

int ReadTemp(void)
{
  char hi_byte, lo_byte;
  SysTempCsClr();
  hi_byte = SysSpiReadByte();
  lo_byte = SysSpiReadByte();
  SysTempCsSet();
  return hi_byte * 256 + lo_byte;
}
//------------------------------------------------------------------------------
// DAC interface
//------------------------------------------------------------------------------
  //DAC AD5620 - 12-bit (code 0-4095)
// Data format:
  // MSB | ...                              ...| LSB
  //----------------------------------------------------------------------------
  // 0   | 0  | D11 | D10 | ...   |D1 | D0 | X | X |
  //----------------------------------------------------------------------------
  
  //------------------------------
  //  Signal  | Pin | Port
  //------------------------------
  // DAC_SYNC | 76  | P0.9
  // DAC_SCLK | 77  | P0.8
  // DINT0    | 67  | P2.6
  // DINT1    | 68  | P2.5
  // DINT2    | 69  | P2.4
  // DINT3    | 70  | P2.3

void DacInitPio(void)
{
  FIO0DIR |= DAC_SYNC | DAC_SCLK;
  FIO2DIR |= DIN0 | DIN1 | DIN2 | DIN3;
  DacSyncSet();
  DacSclkSet();
}

void WriteToDac(unsigned int value[4])
{
  unsigned int code[4];
  
  for(char i = 0; i < 4; i++)
  {
    if (value[i] > 4095)
      value[i] = 4095;
    code[i] = value[i] << 2;
  };
  
  DacSclkSet();
  DacSyncClr();
  for(char i = 0; i < 16; i++)
    {
    DacSclkSet();
    for(char j = 0; j < 4; j++)
      if(code[j] & (1 << (15 - i)))
        DinSet(j);
      else
        DinClr(j);
      DacSclkClr();    
    };
  DacSyncSet();
}
//------------------------------------------------------------------------------
// ADC interface
//------------------------------------------------------------------------------
void AdcInitPio(void)
{
  FIO0DIR |= ADC0_MOSI | ADC1_MOSI | ADC2_MOSI | ADC3_MOSI;
  FIO2DIR |= ADC_CLK | ADC_KEY;
  AdcClkSet();
  AdcKeyClr();  
}

void WriteByteToAdc(char bytes[4])
{
  for(char j = 0; j < 8; j++)
  {
    AdcClkClr();
    for(char i = 0; i < 4; i++)
      if(bytes[i] & (1 << (7 - j))) 
        AdcMosiSet(i);
      else
        AdcMosiClr(i);
    AdcClkSet();
  };
}

void WriteByteToAllAdc(char byte)
{
  for(char j = 0; j < 8; j++)
  {
    AdcClkClr();
    if(byte & (1 << (7 - j)))
      for(char i = 0; i < 4; i++)
        AdcMosiSet(i);
    else
      for(char i = 0; i < 4; i++)
        AdcMosiClr(i);
    AdcClkSet();
  };
}

void ReadByteFromAdc(char bytes[4])
{
  for(char i = 0; i < 4; i++)
      bytes[i] = 0;
  
  for(char j = 0; j < 8; j++)
  {
    AdcClkClr();
  //  Delay(10);
    for(char i = 0; i < 4; i++)
      bytes[i] |= (GetAdcMiso(i) << (7 - j));
  //  Delay(10);
    AdcClkSet();
  //  Delay(10);    
  };
}

char RdyIsLow(void)
{
  for(char i = 0; i < 4; i++)
//  char i = 0;
    if (GetAdcMiso(i))
      return 0;
  return 1;
}

void GetAdcValue(unsigned int values[4])
{

  for(char i = 0; i < 4; i++)
      values[i] = 0;
  
  for(char j = 0; j < BITS; j++)
  {
    AdcClkClr();
  //  Delay(10);
    for(char i = 0; i < 4; i++)
      values[i] |= (GetAdcMiso(i) << (BITS - 1 - j));
  //  Delay(10);
    AdcClkSet();
  //  Delay(10);
  };  
}
//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
