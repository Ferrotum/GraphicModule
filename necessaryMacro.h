#pragma once
// necessaryMacro
#ifndef NECESSARYMACRO_H_
#define NECESSARYMACRO_H_

#include <corecrt_math_defines.h>

#define BGRtoRBGA(color) (((color >> 16) & 0xFF) | (color & 0xFF00) | ((color & 0xFF) << 16) | 0xFF000000)

#define rad(angle) (angle * (M_PI / 180.0f))

#define RBGA2FLOAT4(colori, colorf)\
colorf[0] = (float)(colori & 0xFF) / 255.0f;\
colorf[1] = (float)((colori >> 8) & 0xFF) / 255.0f;\
colorf[2] = (float)((colori >> 16) & 0xFF) / 255.0f;\
colorf[3] = (float)((colori >> 24) & 0xFF) / 255.0f;

#endif // NECESSARYMACRO_H_
