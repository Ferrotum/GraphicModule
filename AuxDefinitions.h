#pragma once
#ifndef _AUXDEFINITIONS_H_
#define _AUXDEFINITIONS_H_

#include <new>

#ifdef __cpp_lib_hardware_interference_size
constexpr int CASH_LINE_SIZE = std::hardware_constructive_interference_size;
#else
constexpr int CASH_LINE_SIZE = 64;
#endif // __cpp_lib_hardware_interference_size
#endif // !_AUXDEFINITIONS_H_
