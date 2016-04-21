//------------------------------------------------------------------------------
// Модуль содержит объявления переменных
//------------------------------------------------------------------------------
#ifndef VARIABLES_h
#define VARIABLES_h

#include "dict.h"
/*
//------------------------------------------------------------------------------
// var_type
//------------------------------------------------------------------------------
#define INT   0
#define FLOAT 1

//------------------------------------------------------------------------------
// io_type
//------------------------------------------------------------------------------
#define IN  0
#define OUT 1
#define INTERNAL 2

typedef union 
{
  signed long ival;
  float fval;
} T_VAR;

typedef struct 
{
  int tag; //числовой идентификатор переменной
  char var_type;
  char io_type;
  int fram_addr;
  int modbus_addr;
  char[8] var_name; 
  char[8] var_unit; //название единицы измерения
  T_VAR def_val;
  T_VAR min_val;
  T_VAR max_val;
  char after_point; //знаков после запятой
  char edit_allowed; //разрешено редактирование пользователем
} T_DICT_RECORD;
*/
//-----------------------------------------------
//  IDs for GetStringByTag
#define ID_VAR_NAME 0
#define ID_VAR_UNIT 1
#define ID_CURR_VAL 2
#define ID_MIN      3 
#define ID_MAX      4

char GetStringByTag(int tag, char id, char* buff, char buf_size);
//-----------------------------------------------
//Переменные настройки канала термостатирования
#define SHIFT_T_L  0
#define K_CD_T_L   1
#define T_F_T_L    2
#define ADJ_S_T_L  3
#define K_P_T_L    4
#define K_D_T_L    5
#define T_DF_T_L   6
#define SH_CLC_T_L 7

#define SHIFT_T_R  20
#define K_CD_T_R   21
#define T_F_T_R    22
#define ADJ_S_T_R  23
#define K_P_T_R    24
#define K_D_T_R    25
#define T_DF_T_R   26
#define SH_CLC_T_R 27

//Переменные настройки канала кислорода
#define SHIFT_E_L  40
#define K_CD_E_L   41
#define T_F_E_L    42
#define T_FF_T_L   43
#define T_F_O_L    44
#define N_L        45
#define X1_L       46
#define Y1_L       47
#define X2_L       48
#define Y2_L       49
#define X3_L       50
#define Y3_L       51
#define X4_L       52
#define Y4_L       53
#define X5_L       54
#define Y5_L       55
#define K_I_O_L    56
#define SH_4_O_L   57

#define SHIFT_E_R  70
#define K_CD_E_R   71
#define T_F_E_R    72
#define T_FF_T_R   73
#define T_F_O_R    74
#define N_R        75
#define X1_R       76
#define Y1_R       77
#define X2_R       78
#define Y2_R       79
#define X3_R       80
#define Y3_R       81
#define X4_R       82
#define Y4_R       83
#define X5_R       84
#define Y5_R       85
#define K_I_O_R    86
#define SH_4_O_R   87

//Переменные настройки канала химнедожога
#define T_F_H_L    110
#define T_FF_H_L   111
#define T_D_H_L    112
#define K_R_H_L    113
#define K_L_H_L    114
#define LIM_H_L    115
#define KLIM_H_L   116
#define T_FD_H_L   117
#define XH0_L      118
#define YH0_L      119
#define XH1_L      120
#define YH1_L      121
#define XH2_L      122
#define YH2_L      123
#define XH3_L      124
#define YH3_L      125
#define XH4_L      126
#define YH4_L      127
#define XH5_L      128
#define YH5_L      129
#define K_I_H_L    130
#define SH_4_H_L   131

#define T_F_H_R    150
#define T_FF_H_R   151
#define T_D_H_R    152
#define K_R_H_R    153
#define K_L_H_R    154
#define LIM_H_R    155
#define KLIM_H_R   156
#define T_FD_H_R   157
#define XH0_R      158
#define YH0_R      159
#define XH1_R      160
#define YH1_R      161
#define XH2_R      162
#define YH2_R      163
#define XH3_R      164
#define YH3_R      165
#define XH4_R      166
#define YH4_R      167
#define XH5_R      168
#define YH5_R      169
#define K_I_H_R    170
#define SH_4_H_R   171

#define MAX_RECORDS 172

//------------------------------------------------------------------------------
//  Modbus functions
//------------------------------------------------------------------------------
/*
  T_DICT_RECORD GetByModbusAddr(int mbaddr);
  int GetTypeByModbusAddr(int mbaddr);
  int GetIntValueByModbusAddr(int mbaddr);
  float GetFloatValueByModbusAddr(int mbaddr);
*/
//------------------------------------------------------------------------------
//  Fram functions
//------------------------------------------------------------------------------
  void SetDefaultValues(void);
  void LoadValuesFromFram(void);
  void SaveValuesToFram(void);
  
  int GetNumberByTag(int tag);
  int GetIntValueByTag(int tag);
  float GetFloatValueByTag(int tag);
  T_VAR GetValueByTag(int tag);
  char GetTypeByTag(int tag);
//------------------------------------------------------------------------------
//  Common functions
//------------------------------------------------------------------------------  
  char* GetNameByTag(int tag);
  char* GetUnitByTag(int tag);
  
  void SetIntValueByTag(int tag, int value);
  void SetFloatValueByTag(int tag, float value);
  
  //  #define MSG_VAR_CHANGED          10
//------------------------------------------------------------------------------
#endif
