#include <stdio.h>
#include <string.h>
#include "dict.h"
#include "board.h"
#include "variables.h"

T_DICT_RECORD flash_vars[MAX_RECORDS] = {
//---------------------------------------------------------------------------------------------------------------
// tag var_type io_type fram_addr      var_name    var_unit     def_val  min_val       after_point 
//                               modbus_addr                                  max_val     edit_allowed
//---------------------------------------------------------------------------------------------------------------
//                                                                                              +--+- must be 0    
//Переменные настройки канала термостатирования                                              // |  |
{SHIFT_T_L,  INT,   INTERNAL, 0, 1,    "Shift_T",  "б/р",         0.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{K_CD_T_L,   FLOAT, INTERNAL, 1, 3,    "K_cd_T",   "\xBECxe-3", 12.8,    0.0, 10000.0, 3, TRUE, 0, 0},
{T_F_T_L,    FLOAT, INTERNAL, 2, 5,    "T_f_T",    "-",         1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{ADJ_S_T_L,  FLOAT, INTERNAL, 3, 7,    "Adj_s_T",  "\xBEC",     600.0,   0.0, 10000.0, 3, TRUE, 0, 0},
{K_P_T_L,    FLOAT, INTERNAL, 4, 9,    "K_p_T",    "-",         1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{K_D_T_L,    FLOAT, INTERNAL, 5, 11,   "K_d_T",    "-",         1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{T_DF_T_L,   FLOAT, INTERNAL, 6, 13,   "T_df_T",   "с",         1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{SH_CLC_T_L, INT,   INTERNAL, 7, 15,   "Sh_clc_T", "-",         26624.0, 0.0, 10000.0, 3, TRUE, 0, 0},

{SHIFT_T_R,  INT,   INTERNAL, 20, 41,  "Shift_T",  "-",         0.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{K_CD_T_R,   FLOAT, INTERNAL, 21, 43,  "K_cd_T",   "\xBECxe-3", 12.8,    0.0, 10000.0, 3, TRUE, 0, 0},
{T_F_T_R,    FLOAT, INTERNAL, 22, 45,  "T_f_T",    "-",         1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{ADJ_S_T_R,  FLOAT, INTERNAL, 23, 47,  "Adj_s_T",  "\xBEC",     600.0,   0.0, 10000.0, 3, TRUE, 0, 0},
{K_P_T_R,    FLOAT, INTERNAL, 24, 49,  "K_p_T",    "-",         1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{K_D_T_R,    FLOAT, INTERNAL, 25, 51,  "K_d_T",    "-",         1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{T_DF_T_R,   FLOAT, INTERNAL, 26, 53,  "T_df_T",   "с",         1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{SH_CLC_T_R, INT,   INTERNAL, 27, 55,  "Sh_clc_T", "-",         26624.0, 0.0, 10000.0, 3, TRUE, 0, 0},
//Переменные настройки канала кислорода
{SHIFT_E_L,  INT,   INTERNAL, 40, 81,  "Shift_K",  "-",         0.0,     0.0, 10000.0, 0, TRUE, 0, 0},
{K_CD_E_L,   FLOAT, INTERNAL, 41, 83,  "K_cd_E",   "мкВ",       3.46,    0.0, 10000.0, 3, TRUE, 0, 0},
{T_F_E_L,    FLOAT, INTERNAL, 42, 85,  "T_f_E",    "c",         1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{T_FF_T_L,   FLOAT, INTERNAL, 43, 87,  "T_ff_T",   "c",         1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{T_F_O_L,    FLOAT, INTERNAL, 44, 89,  "T_f_0",    "c",         1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{N_L,        INT,   INTERNAL, 45, 91,  "N",        "-",         0.0,     0.0, 5.0,     0, TRUE, 0, 0},
{X1_L,       FLOAT, INTERNAL, 46, 93,  "X1",       "об.%",      6.0,     0.0, 100.0,   3, TRUE, 0, 0},
{Y1_L,       FLOAT, INTERNAL, 47, 95,  "Y1",       "об.%",      6.0,     0.0, 100.0,   3, TRUE, 0, 0},
{X2_L,       FLOAT, INTERNAL, 48, 97,  "X2",       "об.%",      10.0,    0.0, 100.0,   3, TRUE, 0, 0},
{Y2_L,       FLOAT, INTERNAL, 49, 99,  "Y2",       "об.%",      8.0,     0.0, 100.0,   3, TRUE, 0, 0},
{X3_L,       FLOAT, INTERNAL, 50, 101, "X3",       "об.%",      14.0,    0.0, 100.0,   3, TRUE, 0, 0},
{Y3_L,       FLOAT, INTERNAL, 51, 103, "Y3",       "об.%",      6.0,     0.0, 100.0,   3, TRUE, 0, 0},
{X4_L,       FLOAT, INTERNAL, 52, 105, "X4",       "об.%",      17.0,    0.0, 100.0,   3, TRUE, 0, 0},
{Y4_L,       FLOAT, INTERNAL, 53, 107, "Y4",       "об.%",      6.0,     0.0, 100.0,   3, TRUE, 0, 0},
{X5_L,       FLOAT, INTERNAL, 54, 109, "X5",       "об.%",      20.0,    0.0, 100.0,   3, TRUE, 0, 0},
{Y5_L,       FLOAT, INTERNAL, 55, 111, "Y5",       "об.%",      12.0,    0.0, 100.0,   3, TRUE, 0, 0},
{K_I_O_L,    FLOAT, INTERNAL, 56, 113, "K_i_0",    "1/об.%",    200.0,   0.0, 10000.0, 3, TRUE, 0, 0},
{SH_4_O_L,   INT,   INTERNAL, 57, 115, "Sh_4_0",   "-",         0.0,     0.0, 10000.0, 0, TRUE, 0, 0},

{SHIFT_E_R,  INT,   INTERNAL, 70, 141,  "Shift_K",  "-",        0.0,     0.0, 10000.0, 0, TRUE, 0, 0},
{K_CD_E_R,   FLOAT, INTERNAL, 71, 143,  "K_cd_E",   "мкВ",      3.46,    0.0, 10000.0, 3, TRUE, 0, 0},
{T_F_E_R,    FLOAT, INTERNAL, 72, 145,  "T_f_E",    "c",        1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{T_FF_T_R,   FLOAT, INTERNAL, 73, 147,  "T_ff_T",   "c",        1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{T_F_O_R,    FLOAT, INTERNAL, 74, 149,  "T_f_0",    "c",        1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{N_R,        INT,   INTERNAL, 75, 151,  "N",        "-",        0.0,     0.0, 5.0,     0, TRUE, 0, 0},
{X1_R,       FLOAT, INTERNAL, 76, 153,  "X1",       "об.%",     6.0,     0.0, 100.0,   3, TRUE, 0, 0},
{Y1_R,       FLOAT, INTERNAL, 77, 155,  "Y1",       "об.%",     6.0,     0.0, 100.0,   3, TRUE, 0, 0},
{X2_R,       FLOAT, INTERNAL, 78, 157,  "X2",       "об.%",     10.0,    0.0, 100.0,   3, TRUE, 0, 0},
{Y2_R,       FLOAT, INTERNAL, 79, 159,  "Y2",       "об.%",     8.0,     0.0, 100.0,   3, TRUE, 0, 0},
{X3_R,       FLOAT, INTERNAL, 80, 161,  "X3",       "об.%",     14.0,    0.0, 100.0,   3, TRUE, 0, 0},
{Y3_R,       FLOAT, INTERNAL, 81, 163,  "Y3",       "об.%",     6.0,     0.0, 100.0,   3, TRUE, 0, 0},
{X4_R,       FLOAT, INTERNAL, 82, 165,  "X4",       "об.%",     17.0,    0.0, 100.0,   3, TRUE, 0, 0},
{Y4_R,       FLOAT, INTERNAL, 83, 167,  "Y4",       "об.%",     6.0,     0.0, 100.0,   3, TRUE, 0, 0},
{X5_R,       FLOAT, INTERNAL, 84, 169,  "X5",       "об.%",     20.0,    0.0, 100.0,   3, TRUE, 0, 0},
{Y5_R,       FLOAT, INTERNAL, 85, 171,  "Y5",       "об.%",     12.0,    0.0, 100.0,   3, TRUE, 0, 0},
{K_I_O_R,    FLOAT, INTERNAL, 86, 173,  "K_i_0",    "1/об.%",   200.0,   0.0, 10000.0, 3, TRUE, 0, 0},
{SH_4_O_R,   INT,   INTERNAL, 87, 175,  "Sh_4_0",   "-",        0.0,     0.0, 10000.0, 0, TRUE, 0, 0},
//Переменные настройки канала химнедожога
{T_F_H_L,    FLOAT, INTERNAL, 110, 201, "T_f_h",    "с",        1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{T_FF_H_L,   FLOAT, INTERNAL, 111, 203, "T_ff_h",   "с",        1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{T_D_H_L,    FLOAT, INTERNAL, 112, 205, "T_d_h",    "c",        1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{K_R_H_L,    FLOAT, INTERNAL, 113, 207, "K_r_h",    "-",        1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{K_L_H_L,    FLOAT, INTERNAL, 114, 209, "K_l_h",    "-",        -1.0,    -10.0, 10.0,  3, TRUE, 0, 0},
{LIM_H_L,    FLOAT, INTERNAL, 115, 211, "Lim_h",    "мВ",       50.0,    0.0, 5.0,     3, TRUE, 0, 0},
{KLIM_H_L,   FLOAT, INTERNAL, 116, 213, "KLim_h",   "-%",       1.0,     0.0, 100.0,   3, TRUE, 0, 0},
{T_FD_H_L,   FLOAT, INTERNAL, 117, 215, "T_fd_h",   "с",        30.0,    0.0, 100.0,   3, TRUE, 0, 0},
{XH0_L,      FLOAT, INTERNAL, 118, 217, "XH0",      "-",        0.0,     0.0, 100.0,   3, TRUE, 0, 0},
{YH0_L,      FLOAT, INTERNAL, 119, 219, "YH0",      "-",        0.0,     0.0, 100.0,   3, TRUE, 0, 0},
{XH1_L,      FLOAT, INTERNAL, 120, 221, "XH1",      "-",        6.0,     0.0, 100.0,   3, TRUE, 0, 0},
{YH1_L,      FLOAT, INTERNAL, 121, 223, "YH1",      "-",        6.0,     0.0, 100.0,   3, TRUE, 0, 0},
{XH2_L,      FLOAT, INTERNAL, 122, 225, "XH2",      "-",        10.0,    0.0, 100.0,   3, TRUE, 0, 0},
{YH2_L,      FLOAT, INTERNAL, 123, 227, "YH2",      "-",        8.0,     0.0, 100.0,   3, TRUE, 0, 0},
{XH3_L,      FLOAT, INTERNAL, 124, 229, "XH3",      "-",        14.0,    0.0, 100.0,   3, TRUE, 0, 0},
{YH3_L,      FLOAT, INTERNAL, 125, 231, "YH3",      "-",        6.0,     0.0, 100.0,   3, TRUE, 0, 0},
{XH4_L,      FLOAT, INTERNAL, 126, 233, "XH4",      "-",        17.0,    0.0, 10000.0, 3, TRUE, 0, 0},
{YH4_L,      FLOAT, INTERNAL, 127, 235, "YH4",      "-",        6.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{YH5_L,      FLOAT, INTERNAL, 128, 237, "XH5",      "-",        20.0,    0.0, 100.0,   3, TRUE, 0, 0},
{YH5_L,      FLOAT, INTERNAL, 129, 239, "YH5",      "-",        12.0,    0.0, 100.0,   3, TRUE, 0, 0},
{K_I_H_L,    FLOAT, INTERNAL, 130, 241, "K_i_h",    "-",        1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{SH_4_H_L,   INT,   INTERNAL, 131, 243, "Sh_4_h",   "-",        0.0,     0.0, 10000.0, 0, TRUE, 0, 0},

{T_F_H_R,    FLOAT, INTERNAL, 150, 271, "T_f_h",    "с",        1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{T_FF_H_R,   FLOAT, INTERNAL, 151, 273, "T_ff_h",   "с",        1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{T_D_H_R,    FLOAT, INTERNAL, 152, 275, "T_d_h",    "c",        1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{K_R_H_R,    FLOAT, INTERNAL, 153, 277, "K_r_h",    "-",        1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{K_L_H_R,    FLOAT, INTERNAL, 154, 279, "K_l_h",    "-",        -1.0,    -10.0, 10.0,  3, TRUE, 0, 0},
{LIM_H_R,    FLOAT, INTERNAL, 155, 281, "Lim_h",    "мВ",       50.0,    0.0, 5.0,     3, TRUE, 0, 0},
{KLIM_H_R,   FLOAT, INTERNAL, 156, 283, "KLim_h",   "-%",       1.0,     0.0, 100.0,   3, TRUE, 0, 0},
{T_FD_H_R,   FLOAT, INTERNAL, 157, 285, "T_fd_h",   "с",        30.0,    0.0, 100.0,   3, TRUE, 0, 0},
{XH0_R,      FLOAT, INTERNAL, 158, 287, "XH0",      "-",        0.0,     0.0, 100.0,   3, TRUE, 0, 0},
{YH0_R,      FLOAT, INTERNAL, 159, 289, "YH0",      "-",        0.0,     0.0, 100.0,   3, TRUE, 0, 0},
{XH1_R,      FLOAT, INTERNAL, 160, 291, "XH1",      "-",        6.0,     0.0, 100.0,   3, TRUE, 0, 0},
{YH1_R,      FLOAT, INTERNAL, 161, 293, "YH1",      "-",        6.0,     0.0, 100.0,   3, TRUE, 0, 0},
{XH2_R,      FLOAT, INTERNAL, 162, 295, "XH2",      "-",        10.0,    0.0, 100.0,   3, TRUE, 0, 0},
{YH2_R,      FLOAT, INTERNAL, 163, 297, "YH2",      "-",        8.0,     0.0, 100.0,   3, TRUE, 0, 0},
{XH3_R,      FLOAT, INTERNAL, 164, 299, "XH3",      "-",        14.0,    0.0, 100.0,   3, TRUE, 0, 0},
{YH3_R,      FLOAT, INTERNAL, 165, 301, "YH3",      "-",        6.0,     0.0, 100.0,   3, TRUE, 0, 0},
{XH4_R,      FLOAT, INTERNAL, 166, 303, "XH4",      "-",        17.0,    0.0, 10000.0, 3, TRUE, 0, 0},
{YH4_R,      FLOAT, INTERNAL, 167, 305, "YH4",      "-",        6.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{YH5_R,      FLOAT, INTERNAL, 168, 307, "XH5",      "-",        20.0,    0.0, 100.0,   3, TRUE, 0, 0},
{YH5_R,      FLOAT, INTERNAL, 169, 309, "YH5",      "-",        12.0,    0.0, 100.0,   3, TRUE, 0, 0},
{K_I_H_R,    FLOAT, INTERNAL, 170, 311, "K_i_h",    "-",        1.0,     0.0, 10000.0, 3, TRUE, 0, 0},
{SH_4_H_R,   INT,   INTERNAL, 171, 313, "Sh_4_h",   "-",        0.0,     0.0, 10000.0, 0, TRUE, 0, 0}
};

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

/*
void WriteIntToFram(unsigned int fram_add, unsigned int data);
void WriteFloatToFram(unsigned int fram_add, float data);
void ReadIntFromFram(unsigned int fram_add, unsigned int *data);
void ReadFloatFromFram(unsigned int fram_add, float *data);
*/
  void SetDefaultValues(void)
  {
    for(int i = 0; i < MAX_RECORDS; i++)
      if(flash_vars[i].fram_addr != -1)
        flash_vars[i].curr_val = flash_vars[i].def_val;
    SaveValuesToFram();
  };

  void LoadValuesFromFram(void)
  {
  };
  
  void SaveValuesToFram(void)
  {
    for(int i = 0; i < MAX_RECORDS; i++)
      if(flash_vars[i].fram_addr != -1)
        WriteIntToFram(flash_vars[i].fram_addr, flash_vars[i].curr_val.ival);
  };
  
  int GetNumberByTag(int tag)
  {
    for(int i = 0; i < MAX_RECORDS; i++)
      if(flash_vars[i].tag == tag)
        return i;    
    return -1;
  };
  
  int GetIntValueByTag(int tag)
  {
    int i = GetNumberByTag(tag);
    if (i != -1)
      return flash_vars[i].curr_val.ival;
    return 0;
  };
  
  float GetFloatValueByTag(int tag)
  {
    int i = GetNumberByTag(tag);
    if (i != -1)
      return flash_vars[i].curr_val.fval;
    return 0.0;
  };
 
  char* GetNameByTag(int tag)
  {
    int i = GetNumberByTag(tag);
    if (i != -1)
      return flash_vars[i].var_name;
    return "Error";
  };
  
  char* GetUnitByTag(int tag)
  {
    int i = GetNumberByTag(tag);
    if (i != -1)
      return flash_vars[i].var_unit;
    return "Error";
  };
  
  T_VAR GetValueByTag(int tag)
  {
    return flash_vars[GetNumberByTag(tag)].curr_val;
  };
  
  char GetTypeByTag(int tag)
  {
    return flash_vars[GetNumberByTag(tag)].var_type;
  };
  
  char GetStringByTag(int tag, char id, char* buff, char buf_size)
  {
    char buffer[50];
    char j; //количество символов в строке
    int i = GetNumberByTag(tag);
    if (i != -1)
    {
      switch(id)
      {
      case ID_CURR_VAL:
        j = (flash_vars[i].var_type == INT?
             sprintf(buffer, "%d", flash_vars[i].curr_val.ival):
             sprintf(buffer, "%f", flash_vars[i].curr_val.fval));
        break;
      case ID_MIN:
        j = (flash_vars[i].var_type == INT?
             sprintf(buffer, "%d", flash_vars[i].min_val.ival):
             sprintf(buffer, "%f", flash_vars[i].min_val.fval));
        break;
      case ID_MAX:
        j = (flash_vars[i].var_type == INT?
             sprintf(buffer, "%d", flash_vars[i].max_val.ival):
             sprintf(buffer, "%f", flash_vars[i].max_val.fval));
        break;    
      };
      if(j > buf_size)  //fail!
      {
        buff[0] = 0;
        return (char)-1;
      };
      strcpy(buff, buffer); 
      return 0; //normal
    };
    return (char)-1; //epic fail!
  };
  
  void SetIntValueByTag(int tag, int value)
  {
    int i = GetNumberByTag(tag);
    if (i != -1)
      flash_vars[i].curr_val.ival = value;
  };
  
  void SetFloatValueByTag(int tag, float value)
  {
    int i = GetNumberByTag(tag);
    if (i != -1)
      flash_vars[i].curr_val.fval = value;
  };
  
  //  #define MSG_VAR_CHANGED          10
//------------------------------------------------------------------------------
