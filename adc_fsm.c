#include "board.h"
#include "adc_fsm.h"
#include "messages.h"
#include "timers.h"

char adc_state = 0, adc_iteration_count = 0;

unsigned int adc_values[4], adc_values_[4], adc_curr_values[4], adc_values2[4];
char adc_break[4];

void StartAdcConvertion(char channel, char gain, long int polarity)
{
//writing operation to  the configuration register - 0x10
  WriteByteToAllAdc(0x10);
  
//configuration register - 0x0310
  char conf_m_byte = 0x03,
       conf_l_byte = 0x10;
  conf_m_byte |= gain | polarity;
  conf_l_byte |= channel | (gain << 8) | polarity;
  
  WriteByteToAllAdc(conf_m_byte);  
  WriteByteToAllAdc(conf_l_byte);

  WriteByteToAllAdc(0x58); //Start convertion
}

void InitAdc(void)
{
  //Reset ADC
  for(char i = 0; i < 4; i++)
    WriteByteToAllAdc(0xFF);

//writing operation to  the mode register - 0x08
  WriteByteToAllAdc(0x08);
  
//mode register - 0x0003
  char mode_m_byte = 0x00,
    mode_l_byte = 0x02;
  WriteByteToAllAdc(mode_m_byte);
  WriteByteToAllAdc(mode_l_byte);  

  StartAdcConvertion(0, G_1, BIPOLAR);
}

void AdcCurrOut(char on)
{
//writing operation to  the IO register - 0x28
  WriteByteToAllAdc(0x28); 
  if(on)
    WriteByteToAllAdc(0x02);  
  else
    WriteByteToAllAdc(0x00);
}

void ProcessAdc(void)
{
  switch(adc_state)
  {
    case 0: 
      if((GetTimer(ADC_CONV_TIMER) >= 1) && RdyIsLow()) {
        ResetTimer(ADC_CONV_TIMER);
        ResetTimer(ADC_TIME_OUT);
        GetAdcValue(adc_curr_values);
        for(char i = 0; i < 4; i++)
          adc_values_[i] += adc_curr_values[i];
        adc_iteration_count++;
        if (adc_iteration_count < 8)
          StartAdcConvertion(0, G_8, BIPOLAR);
        else {
          for(char i = 0; i < 4; i++) {
            adc_values[i] = adc_values_[i];
            adc_values_[i] = 0;
          };
          adc_iteration_count = 0;
          // preparing to next convertion cycle 

          AdcKeySet();  //debug!!!
        
          AdcCurrOut(1);
          StartAdcConvertion(1, G_1, UNIPOLAR);
          adc_state = 1;
        };
      };       

      if(GetTimer(ADC_TIME_OUT) >= 10)
        adc_state = 2;

/*
      if (adc_iteration_count == 8)
      {
        ResetTimer(ADC_CONV_TIMER);
        ResetTimer(ADC_TIME_OUT);        
        for(char i = 0; i < 4; i++)
        {
          adc_values[i] = adc_values_[i];
          adc_values_[i] = 0;
        };
        adc_iteration_count = 0;
        // preparing to next convertion cycle 

        AdcKeySet();  //debug!!!
        
        AdcCurrOut(1);
        StartAdcConvertion(1, G_1, BIPOLAR);
        adc_state = 1;
      };*/
      if(GetTimer(ADC_TIME_OUT) >= 10)
        adc_state = 2;
      break;
    case 1: //check if input wires are broken
      if((GetTimer(ADC_CONV_TIMER) >= 1) && RdyIsLow())
      {
        ResetTimer(ADC_CONV_TIMER);
        ResetTimer(ADC_TIME_OUT);
        GetAdcValue(adc_values2);
        for(char i = 0; i < 4; i++)
          adc_break[i] = (adc_values2[i] > 0x00C00000); //threshold
        //     SendMessage(MSG_ADC_READY);
        // preparing to next convertion cycle        
        AdcCurrOut(0);
        
        AdcKeyClr();  //debug!!!
        
        StartAdcConvertion(0, G_8, BIPOLAR);
        adc_state = 0;
      };
      if(GetTimer(ADC_TIME_OUT) >= 10)
        adc_state = 2;
      break;
    case 2: // epic fail! reset ADCs!
      ResetTimer(ADC_CONV_TIMER);
      ResetTimer(ADC_TIME_OUT);
      InitAdc();
      adc_state = 0;
      break;      
  };
}
