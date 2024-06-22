#pragma once
#ifndef _MATRIXLIBRARY_H_
#define _MATRIXLIBRARY_H_
#if defined (_USE_EIGEN_MATRIX_LIBRARY_)
#ifdef rint
#define RINT_UNDEFINED_ 1
#pragma push_macro("rint")
#undef rint
#endif // rint
#include <Eigen/core>
#include <Eigen/Geometry>
#if defined (RINT_UNDEFINED_)
#pragma pop_macro("rint")
#undef RINT_UNDEFINED_
#endif // RINT_UNDEFINED_
#elif defined (_USE_QT_MATRIX_LIBRARY_)
#include <QGenericMatrix>
#endif // _GF_MATRIX_LIB_



#endif // !_MATRIXLIBRARY_H_
