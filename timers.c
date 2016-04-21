#include "timers.h"

unsigned int Timers[MAX_TIMERS];

void ProcessTimers(void)
{
  for(char i = 0; i < MAX_TIMERS; i++)
    Timers[i]++;
}

unsigned int GetTimer(char Timer)
{
  return Timers[Timer];
}

void ResetTimer(char Timer)
{
  Timers[Timer] = 0;
}

void InitTimers(void)
{
  for(char i = 0; i < MAX_TIMERS; i++)
    Timers[i] = 0;
}
