//------------------------------------------------------------------------------
// Модуль реализует редактор значений (контрол)
//------------------------------------------------------------------------------
#ifndef EDITOR_h
#define EDITORN_h

#include "dict.h"

void SetEditorMode(char _e_type, T_VAR e_init_val);

//void DrawEditor(char x1, char y1, char x2, char y2, char active);

void ProcessEditor(void);

#endif
