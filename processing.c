//------------------------------------------------------------------------------
// Модуль реализует ф-ции работы с переменными настройки и алгоритм
// вычисления измеряемых величин
//------------------------------------------------------------------------------
#include "variables.h"
//#include "channel_controller.h"
//#include "twi.h"
#include <math.h>

//RAM_VAR ram_vars[MAX_VARS][2];
  const float d_c = 0.02; //период измерений 20ms
  const float K_clc_t = 1000.0;
  const float Lim_u_e = 200.0;

  const float Lim_lo_t = 5000.0;
  const float Lim_hi_t = 53248.0;
//------------------------------------------
//  K_f_h = exp(-d_c/T_f_h); d_c = 0.02s, where T_f_h = 0.5s
//  K_ff_h = exp(-d_c/T_ff_h); d_c = 0.02s, where T_ff_h = 0.5s
//  K_d_h = exp(-d_c/T_d_h); d_c = 0.02s, where T_d_h = 30s
//------------------------------------------
  const float K_f_h = 0.96078943915232320943921069132325;
  const float K_ff_h = 0.96078943915232320943921069132325;
  const float K_d_h = 0.99933355550618106886157596765979;

#define _X 0
#define _Y 1

//Flash variables
struct
{
//T-vars
  float//int
    Shift_t[2];
  float
    K_cd_t[2], T_f_t[2], Adj_s_t[2], K_p_t[2], K_d_t[2], T_df_t[2];//, K_clc_t[2];
  float//int
    Sh_clc_t[2], //Lim_lo_t[2], Lim_hi_t[2],
//O2-vars
    Shift_e[2];
  float
    K_cd_e[2], T_f_e[2], /*Lim_u_e[2],*/ T_ff_t[2], T_f_o[2];
  float //int
    N_point[2];
  float
    XY[6][2][2], K_i_o[2];
  float//int
    Sh_4_o[2];
//XH-vars
  float
    /*T_f_h[2], T_ff_h[2], T_d_h[2],*/ K_r_h[2], K_l_h[2], Lim_h[2], Klim_h[2], T_fd_h[2], XYH[7][2][2], K_i_h[2];
  float//int
    Sh_4_h[2];
} ram_vars;

float
//Input variables
//T-vars
  Code_t[2], T_cl_e,
//O2-vars
  Code_e[2],

//Output variables
//T-vars
  Clc_l_t[2],
//O2-vars
  Ci_o_o[2],
//XH-vars
  Ci_o_h[2],

//Internal variables
//T-vars
  Cd_sh_t[2], T_t[2], K_f_t[2], K_df_t[2], Cel_t[2], Mism_t[2],
  Mi_p_t[2], Mi_kd_t[2], Mi_dc_t[2], Clc_t[2], Clc_sh_t[2], Clc_sh_t[2],
  Cel_f_t[2][2], Mi_d_t[2][2],

//O2-vars
  Cd_sh_e[2], E_e[2], E_lf_e[2], Kel_ff_t[2], Den_o[2], Ex_o[2], C_o[2], C_kl_o[2], C_o_o[2], //!!!
  K_f_e[2], K_ff_t[2], K_f_o[2],
  X[2][7], Y[2][7],
  A[2][6], B[2][6],
  E_f_e[2][2], Cel_ff_t[2][2], C_f_o[2][2],

//XH-vars
  D_ffd_h[2], E_li_h[2], C_h[2], C_kl_h[2], Ci_h[2], //!!!
  t[2][2],
  /*K_f_h[2], K_ff_h[2], K_d_h[2],*/ K_fd_h[2],
  XH[2][7], YH[2][7],
  AH[2][6], BH[2][6],
  E_f_h[2][2], E_ff_h[2][2], E_ffd_h[2][2], C_fd_h[2][2];
/*
unsigned int GetVarIntValue(char ch, char var_id)
{
  switch(var_id)
  {
  case CODE_T  : return Code_t[ch];
  case CD_SH_T : return Cd_sh_t[ch];
  case CLC_L_T : return Clc_l_t[ch];
  case CD_SH_E : return Cd_sh_e[ch];
  };
}

float GetVarFloatValue(char ch, char var_id)
{
  switch(var_id)
  {
  case T_CL_E   : return T_cl_e;
  case T_T      : return T_t[ch];
  case CEL_T    : return Cel_t[ch];
  case CEL_F_T  : return Cel_f_t[ch][0];
  case MISM_T   : return Mism_t[ch];
  case E_E      : return E_e[ch];
  case E_F_E    : return E_f_e[ch][0];
  case KEL_FF_T : return Kel_ff_t[ch];
  case EX_O     : return Ex_o[ch];
  case C_F_O    : return C_f_o[ch][0];
  case C_KL_O   : return C_kl_o[ch];
  case E_F_H    : return E_f_h[ch][0];
  case E_FF_H   : return E_ff_h[ch][0];
  case E_FFD_H  : return E_ffd_h[ch][0];
  case D_FFD_H  : return D_ffd_h[ch];
  case E_LI_H   : return E_li_h[ch];
  case T        : return t[ch][0];
  case C_H      : return C_h[ch];
  case C_FD_H   : return C_fd_h[ch][0];
  case C_KL_H   : return C_kl_h[ch];
  case K_FD_H   : return K_fd_h[ch];
  case CD_SH_T  : return Cd_sh_t[ch];
  };
}

void LoadDefaultValues(void)
{
  char i;
  float *p_ram, *p_flash;
  p_ram = (float*)(&ram_vars);
  p_flash = (float*)(&flash_vars[0].def_val);
  for(i = 0; i <= SH_4_H; i++)
  {
    *p_ram = *p_flash;
    p_ram++;
    *p_ram = *p_flash;
    p_ram++;
    p_flash += sizeof(FLASH_VAR) / sizeof(float);
  };

  SaveValuesToEEPROM();
}
*/
void LoadValuesFromEEPROM(void)
{
  StartLoadingFromEEPROM((char*)&ram_vars, 0, sizeof(ram_vars));
}

void SaveValuesToEEPROM(void)
{
  StartSavingToEEPROM((char*)&ram_vars, 0, sizeof(ram_vars));
}

float GetValue(char ch, char var)
{
  return *((float*)(&ram_vars) + var * 2 + ch);
}

void SetValue(char ch, char var, float value)
{
  *((float*)(&ram_vars) + var * 2 + ch) = value;
}

void SetAndSaveValue(char ch, char var, float value)
{
  *((float*)(&ram_vars) + var * 2 + ch) = value;

  StartSavingToEEPROM((char*)&ram_vars + (var * 2 + ch) * sizeof(float), (var * 2 + ch) * sizeof(float), sizeof(float));
}

void SetT(char ch)
{
  K_f_t[ch] = exp(-d_c/ram_vars.T_f_t[ch]);
  K_df_t[ch] = exp(-d_c/ram_vars.T_df_t[ch]);
}

float AD7684_Code2Value(unsigned int code)
{
  /*
  const float AD7684_ref = 5000; // Vref в милливольтах
  if(code & 0x8000)
  { //отрицательное значение
    code &= 0x7FFF;
    return AD7684_ref * (code - 0x8000) / 0x8000;
  }  //положительное значение
  else
    return AD7684_ref * code / 0x8000;
  */
  return (signed int) code;
}

float AD7415_Code2Value(unsigned int code)
{
  if(code & 0x0200)
  { //отрицательное значение
    code &= 0x01FF;
    return (code - 512) / 4;
  }  //положительное значение
  else
  {
    code &= 0x01FF;
    return code / 4;
  };
}

unsigned int AD7249_Value2Code(float value)
{
  const float AD7249_ref = 5000; // Vref в милливольтах
  if(value >= 0)
    //положительное значение
    return (value * 2048) / AD7249_ref;
  else
  {
    return ((int)((value * 2048) / AD7249_ref) + 2048) | 0x0800;
  };
}
//------------------------------------------------------------------
void SetCode_t1(unsigned int code)
{
  Code_t[0] = code;
}

void SetCode_t2(unsigned int code)
{
  Code_t[1] = code;
//  Cd_sh_t[1] = /*AD7684_Code2Value*/(Code_t[1] + ram_vars.Shift_t[1]);
}

void SetT_Cl_e(unsigned int code)
{
  T_cl_e = AD7415_Code2Value(code);
}

void SetCode_e1(unsigned int code)
{
  Code_e[0] = code;
//  Cd_sh_e[0] = /*AD7684_Code2Value*/(Code_e[0] + ram_vars.Shift_e[0]);
}

void SetCode_e2(unsigned int code)
{
  Code_e[1] = code;
//  Cd_sh_e[1] = AD7684_Code2Value(Code_e[1] + ram_vars.Shift_e[1]);
}
/*
unsigned int GetClc_l_t1(void)
{
  if(GetChannelStatus(0) == CHANNEL_ERROR)
    return GetIntegratorValue(0);
  return (unsigned int)Clc_l_t[0];
}

unsigned int GetClc_l_t2(void)
{
  if(GetChannelStatus(1) == CHANNEL_ERROR)
    return GetIntegratorValue(1);
  return (unsigned int)Clc_l_t[1];
}
*/
/*
unsigned int GetMaxAngle(char ch)
{
  return (unsigned int)ram_vars.Lim_hi_t[ch];
}
*/
float GetCi_o_o1(void)
{
  return Ci_o_o[0];
}

float GetCi_o_o2(void)
{
  return Ci_o_o[1];
}

float GetCi_o_h1(void)
{
  return Ci_o_h[0];
}

float GetCi_o_h2(void)
{
  return Ci_o_h[1];
}

float GetCel_f_t1(void)
{
  float temp;
  temp = Cel_f_t[0][0];
  if(temp > 1000) temp = 1000;
  return temp;
}

float GetCel_f_t2(void)
{
  float temp;
  temp = Cel_f_t[1][0];
  if(temp > 1000) temp = 1000;
  return temp;
}
//------------------------------------------------------------------------------
float GetD_C(void)
{
  return d_c;
}
/*
void ProcessT(char ch)
{
  float code;

  if(GetChannelStatus(ch) == CHANNEL_ERROR)
    Cel_f_t[ch][0] = ram_vars.Adj_s_t[ch];
  else
  {
  Cd_sh_t[ch] = Code_t[ch] + ram_vars.Shift_t[ch];
  T_t[ch] = Cd_sh_t[ch] * ram_vars.K_cd_t[ch];

//  Cel_t[ch] = Cd_sh_t[ch] + T_cl_e;
  Cel_t[ch] =  T_t[ch] + T_cl_e;
  Cel_f_t[ch][0] = K_f_t[ch] * (Cel_f_t[ch][1] - Cel_t[ch]) + Cel_t[ch];
  };
  Mism_t[ch] = Cel_f_t[ch][0] - ram_vars.Adj_s_t[ch];
  Mi_p_t[ch] = Mism_t[ch] * ram_vars.K_p_t[ch];
  Mi_d_t[ch][0] = K_df_t[ch] * (Mism_t[ch] - Mi_d_t[ch][1]);

  Mi_kd_t[ch] = Mi_d_t[ch][0] * ram_vars.K_d_t[ch];
  Mi_dc_t[ch] = Mi_kd_t[ch] + Mi_p_t[ch];

// Mi_dc_t[ch] = Mism_t[ch];

  Clc_t[ch] = K_clc_t * Mi_dc_t[ch];
  Clc_sh_t[ch] = ram_vars.Sh_clc_t[ch] + Clc_t[ch];

  Clc_l_t[ch] = Clc_sh_t[ch];
  if(Clc_sh_t[ch] <= MIN_ANGLE)
    Clc_l_t[ch] = MIN_ANGLE;
  if(Clc_sh_t[ch] >= MAX_ANGLE)
    Clc_l_t[ch] = MAX_ANGLE;

  if(GetChannelStatus(ch) == CHANNEL_ON)
    Integrate(ch, Clc_l_t[ch]);

  code = 20.47 * Cel_f_t[ch][0] - 12282;
  if (code < 0) code = 0;
  if (code > 2047) code = 2047;

  if(DACBlocked() == 0) SetDAC(ch * 2 + 4, code);

  Cel_f_t[ch][1] = Cel_f_t[ch][0];
  Mi_d_t[ch][1] = Mi_d_t[ch][0];
}
*/
void SetO2(char ch)
{
  char i;

  K_f_e[ch] = exp(-d_c/ram_vars.T_f_e[ch]);
  K_ff_t[ch] = exp(-d_c/ram_vars.T_ff_t[ch]);
  K_f_o[ch] = exp(-d_c/ram_vars.T_f_o[ch]);
  //
  X[ch][0] = 0;
  Y[ch][0] = 0;

  for(i = 0; i < (int)(ram_vars.N_point[ch]); i++)
  {
    X[ch][i + 1] = ram_vars.XY[i][_X][ch];
    Y[ch][i + 1] = ram_vars.XY[i][_Y][ch];
    A[ch][i] = (Y[ch][i + 1] - Y[ch][i]) / (X[ch][i + 1] - X[ch][i]);
    B[ch][i] = Y[ch][i] - X[ch][i] * A[ch][i];
  };
}
/*
void ProcessO2(char ch)
{
  char i;

  Cd_sh_e[ch] = Code_e[ch] + ram_vars.Shift_e[ch];

  E_e[ch] = Cd_sh_e[ch] * ram_vars.K_cd_e[ch];

 // E_f_e[ch][0] = K_f_e[ch] * (E_f_e[ch][1] - Cd_sh_e[ch]) + Cd_sh_e[ch];
 E_f_e[ch][0] = K_f_e[ch] * (E_f_e[ch][1] - E_e[ch]) + E_e[ch];
  if(E_f_e[ch][0] > Lim_u_e)
    E_lf_e[ch] = Lim_u_e;
  else
    E_lf_e[ch] = E_f_e[ch][0];
  Cel_ff_t[ch][0] = K_ff_t[ch] * (Cel_ff_t[ch][1] - Cel_f_t[ch][0]) + Cel_f_t[ch][0];
  Kel_ff_t[ch] = 273.16 + Cel_ff_t[ch][0];
  Den_o[ch] = -0.0216 * Kel_ff_t[ch];
  Ex_o[ch] = E_lf_e[ch]/Den_o[ch];
  C_o[ch] = exp(Ex_o[ch]) * 20.67;
  C_f_o[ch][0] = K_f_o[ch] * (C_f_o[ch][1] - C_o[ch]) + C_o[ch];

  //-----------------------------------------------------
  //КЛАПП
  if((int)(ram_vars.N_point[ch]) == 0)
    C_kl_o[ch] = C_f_o[ch][0];
  else
    for(i = 0; i < (int)(ram_vars.N_point[ch]); i ++)
      if(C_f_o[ch][0] > X[ch][i])
        C_kl_o[ch] = A[ch][i] * C_f_o[ch][0] + B[ch][i];
  //------------------------------------------------------

  C_o_o[ch] = ram_vars.K_i_o[ch] * C_kl_o[ch];
  Ci_o_o[ch] = C_o_o[ch] + ram_vars.Sh_4_o[ch];// ? DAC

  //----------------------------------
  //  Канал    | ЦАП
  //----------------------------------
  // левый О2  |  0
  // правый О2 |  2
  //----------------------------------
  if((GetChannelStatus(ch) == CHANNEL_OFF) && (DACBlocked == 0))
    SetDAC(ch, ram_vars.Sh_4_o[ch]);
  else
    SetDAC(ch, Ci_o_o[ch]);

  E_f_e[ch][1] = E_f_e[ch][0];
  Cel_ff_t[ch][1] = Cel_ff_t[ch][0];
  C_f_o[ch][1] = C_f_o[ch][0];
}
*/
void SetXH(char ch)
{
  char i;
  /*
  K_f_h[ch] = exp(-d_c/ram_vars.T_f_h[ch]);
  K_ff_h[ch] = exp(-d_c/ram_vars.T_ff_h[ch]);
  K_d_h[ch] = exp(-d_c/ram_vars.T_d_h[ch]);
  */
  K_fd_h[ch] = exp(-d_c/ram_vars.T_fd_h[ch]);

  for(i = 0; i < 7; i++)
  {
    XH[ch][i] = ram_vars.XYH[i][_X][ch];
    YH[ch][i] = ram_vars.XYH[i][_Y][ch];
  };
  for(i = 0; i < 6; i++)
  {
    AH[ch][i] = (YH[ch][i + 1] - YH[ch][i])/(XH[ch][i + 1] - XH[ch][i]);
    BH[ch][i] = YH[ch][i] - XH[ch][i] * AH[ch][i];
  };
}
/*
void ProcessXH(char ch)
{
  char i;

  E_f_h[ch][0] = K_f_h * (E_f_h[ch][1] - Cd_sh_e[ch]) + Cd_sh_e[ch];
  E_ff_h[ch][0] = K_ff_h * (E_ff_h[ch][1] - E_f_h[ch][0]) + E_f_h[ch][0];
  t[ch][0] = K_d_h * (t[ch][1] - E_ff_h[ch][0]) + E_ff_h[ch][0];
  E_ffd_h[ch][0] = E_ff_h[ch][0] - t[ch][0];

  if(E_ffd_h[ch][0] <= 0)
    D_ffd_h[ch] = ram_vars.K_l_h[ch] * E_ffd_h[ch][0];
  else
    D_ffd_h[ch] =  ram_vars.K_r_h[ch] * E_ffd_h[ch][0];
  if(Cd_sh_e[ch] <= ram_vars.Lim_h[ch])
    E_li_h[ch] = 0;
  else
    E_li_h[ch] = ram_vars.Klim_h[ch] * (Cd_sh_e[ch] - ram_vars.Lim_h[ch]);
  C_h[ch]  = E_li_h[ch] + D_ffd_h[ch];
  C_fd_h[ch][0] = K_fd_h[ch] * (C_fd_h[ch][1] - C_h[ch]) + C_h[ch];

  //-----------------------------------------------------
  //КЛАПП
  for(i = 0; i < 6; i ++)
    if(C_fd_h[ch][0] > XH[ch][i])
      C_kl_h[ch] = AH[ch][i] * C_fd_h[ch][0] + BH[ch][i];
  //------------------------------------------------------

  Ci_h[ch] = ram_vars.K_i_h[ch] * C_kl_h[ch];
  Ci_o_h[ch] = ram_vars.Sh_4_h[ch] + Ci_h[ch];

  //----------------------------------
  //  Канал    | ЦАП
  //----------------------------------
  // левый ХН  |  1
  // правый ХН |  3
  //----------------------------------
  if((GetChannelStatus(ch) == CHANNEL_OFF) && (DACBlocked() == 0))
    SetDAC(ch + 2, ram_vars.Sh_4_h[ch]);
  else
    SetDAC(ch + 2, Ci_o_h[ch]);

  E_f_h[ch][1] = E_f_h[ch][0];
  E_ff_h[ch][1] = E_ff_h[ch][0];
  E_ffd_h[ch][1] = E_ffd_h[ch][0];
  C_fd_h[ch][1] = C_fd_h[ch][0];
  t[ch][1] = t[ch][0];
}
*/