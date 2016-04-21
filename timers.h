#ifndef TIMERS_h
#define TIMERS_h

#define sec 100
#define min 60 * sec
#define hour 60 * min
#define day 24 * hour

#define MAX_TIMERS 16

#define KEYB_TIMER       0
#define LAST_KEY_TIMER   1
#define CURSOR_TIMER     2
#define ADC_CONV_TIMER   3
#define ADC_TIME_OUT     4

#define DEB_TIMER1       14
#define DEB_TIMER2       15

void InitTimers(void);

void ProcessTimers(void);

unsigned int GetTimer(char Timer);

void ResetTimer(char Timer);
//------------------------------------------
/*
char LeftPulsesArePresent(void);

char RightPulsesArePresent(void);

void SetTime1(unsigned int value);

void SetTime2(unsigned int value);

void InitIRQ(void);
*/
#endif
