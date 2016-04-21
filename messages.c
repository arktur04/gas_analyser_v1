#include "messages.h"

char Messages[MAX_MESSAGES];

void InitMessages(void)
{
  for(char i = 0; i < MAX_MESSAGES; i++)
    Messages[i] = 0;
}

void SendMessage(char Msg)
{
  Messages[Msg] = 1;
}
/*
void SendIRQMessage(char Msg)
{
  Messages[Msg] = 4;
}
*/
void ProcessMessages(void)
{
  for(char i = 0; i < MAX_MESSAGES; i++)
  {
    if(Messages[i] == 2) Messages[i] = 0;
    if(Messages[i] == 1) Messages[i] = 2;
  }
}

char GetMessage(char Msg)
{
  if((Messages[Msg] == 2) || (Messages[Msg] == 4))
  {
    Messages[Msg] = 0;
    return 1;
  };
  return 0;
}
