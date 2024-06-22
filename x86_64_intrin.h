#pragma once
#ifndef X86_64_INTRIN_H_
#define X86_64_INTRIN_H_

#define _USE_SIMD_EXTENTIONS_FOR_CALCULATING_ 1

#if defined(_M_IX86) || defined(_M_AMD64) || defined(__x86_64__) || defined(__i386__)
#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(_mm_and_pd)
#pragma intrinsic(_mm_and_ps)
#pragma intrinsic(_mm_broadcastss_ps)
#pragma intrinsic(_mm_cmp_pd)
#pragma intrinsic(_mm_cmp_ps)
#pragma intrinsic(_mm_maskstore_ps)
#pragma intrinsic(_mm_max_pd)
#pragma intrinsic(_mm_max_ps)
#pragma intrinsic(_mm_min_pd)
#pragma intrinsic(_mm_min_ps)
#pragma intrinsic(_mm_movemask_pd)
#pragma intrinsic(_mm_movemask_ps)
#pragma intrinsic(_mm_or_pd)
#pragma intrinsic(_mm_or_ps)
#pragma intrinsic(_mm_sub_pd)
#pragma intrinsic(_mm_sub_ps)
#pragma intrinsic(_mm256_add_ps)
#pragma intrinsic(_mm256_and_pd)
#pragma intrinsic(_mm256_and_ps)
#pragma intrinsic(_mm256_broadcast_pd)
#pragma intrinsic(_mm256_broadcast_ps)
#pragma intrinsic(_mm256_broadcast_sd)
#pragma intrinsic(_mm256_cmp_pd)
#pragma intrinsic(_mm256_cmp_ps)
#pragma intrinsic(_mm256_maskstore_pd)
#pragma intrinsic(_mm256_maskstore_ps)
#pragma intrinsic(_mm256_max_pd)
#pragma intrinsic(_mm256_max_ps)
#pragma intrinsic(_mm256_min_pd)
#pragma intrinsic(_mm256_min_ps)
#pragma intrinsic(_mm256_movemask_pd)
#pragma intrinsic(_mm256_movemask_ps)
#pragma intrinsic(_mm256_or_pd)
#pragma intrinsic(_mm256_or_ps)
#pragma intrinsic(_mm256_sub_pd)
#pragma intrinsic(_mm256_sub_ps)
#pragma intrinsic(_mm256_xor_ps)

#define _AVX_addps256_ _mm256_add_ps
#define _AVX_andpd256_ _mm256_and_pd
#define _AVX_andps256_ _mm256_and_ps
#define _AVX_broadcastpd256_ _mm256_broadcast_pd
#define _AVX_broadcastps256_ _mm256_broadcast_ps
#define _AVX_broadcastsd256_ _mm256_broadcast_sd
#define _AVX_cmppd_ _mm_cmp_pd
#define _AVX_cmppd256_ _mm256_cmp_pd
#define _AVX_cmpps_ _mm_cmp_ps
#define _AVX_cmpps256_ _mm256_cmp_ps
#define _AVX_maskstorepd256_ _mm256_maskstore_pd
#define _AVX_maskstoreps_ _mm_maskstore_ps
#define _AVX_maskstoreps256_ _mm256_maskstore_ps
#define _AVX_maxpd256_ _mm256_max_pd
#define _AVX_maxps256_ _mm256_max_ps
#define _AVX_minpd256_ _mm256_min_pd
#define _AVX_minps256_ _mm256_min_ps
#define _AVX_movemaskpd256_ _mm256_movemask_pd
#define _AVX_movemaskps256_ _mm256_movemask_ps
#define _AVX_orpd256_ _mm256_or_pd
#define _AVX_orps256_ _mm256_or_ps
#define _AVX_subpd256_ _mm256_sub_pd
#define _AVX_subps256_ _mm256_sub_ps
#define _AVX_xorps256_ _mm256_xor_ps
#define _AVX2_broadcastssps_ _mm_broadcastss_ps
#define _SSE_andps_ _mm_and_ps
#define _SSE_maxps_ _mm_max_ps
#define _SSE_minps_ _mm_min_ps
#define _SSE_movemaskps_ _mm_movemask_ps
#define _SSE_orps_ _mm_or_ps
#define _SSE_subps_ _mm_sub_ps
#define _SSE2_andpd_ _mm_and_pd
#define _SSE2_maxpd_ _mm_max_pd
#define _SSE2_minpd_ _mm_min_pd
#define _SSE2_movemaskpd_ _mm_movemask_pd
#define _SSE2_orpd_ _mm_or_pd
#define _SSE2_subpd_ _mm_sub_pd

using _int_4_ = __m128i;
using _int64_2_ = __m128i;
using _int_8_ = __m256i;
using _int64_4_ = __m256i;
using _single_4_ = __m128;
using _single_8_ = __m256;
using _double_2_ = __m128d;
using _double_4_ = __m256d;

#elif defined (__GNUC__)
#include <x86intrin.h>

#define _AVX_addps256_ _mm256_add_ps
#define _AVX_andpd256_ _mm256_and_pd
#define _AVX_andps256_ _mm256_and_ps
#define _AVX_broadcastpd256_ _mm256_broadcast_pd
#define _AVX_broadcastps256_ _mm256_broadcast_ps
#define _AVX_broadcastsd256_ _mm256_broadcast_sd
#define _AVX_cmppd_ _mm_cmp_pd
#define _AVX_cmppd256_ _mm256_cmp_pd
#define _AVX_cmpps_ _mm_cmp_ps
#define _AVX_cmpps256_ _mm256_cmp_ps
#define _AVX_maskstorepd256_ _mm256_maskstore_pd
#define _AVX_maskstoreps_ _mm_maskstore_ps
#define _AVX_maskstoreps256_ _mm256_maskstore_ps
#define _AVX_maxpd256_ _mm256_max_pd
#define _AVX_maxps256_ _mm256_max_ps
#define _AVX_minpd256_ _mm256_min_pd
#define _AVX_minps256_ _mm256_min_ps
#define _AVX_movemaskpd256_ _mm256_movemask_pd
#define _AVX_movemaskps256_ _mm256_movemask_ps
#define _AVX_orpd256_ _mm256_or_pd
#define _AVX_orps256_ _mm256_or_ps
#define _AVX_subpd256_ _mm256_sub_pd
#define _AVX_subps256_ _mm256_sub_ps
#define _AVX_xorps256_ _mm256_xor_ps
#define _AVX2_broadcastssps_ _mm_broadcastss_ps
#define _SSE_andps_ _mm_and_ps
#define _SSE_maxps_ _mm_max_ps
#define _SSE_minps_ _mm_min_ps
#define _SSE_movemaskps_ _mm_movemask_ps
#define _SSE_orps_ _mm_or_ps
#define _SSE_subps_ _mm_sub_ps
#define _SSE2_andpd_ _mm_and_pd
#define _SSE2_maxpd_ _mm_max_pd
#define _SSE2_minpd_ _mm_min_pd
#define _SSE2_movemaskpd_ _mm_movemask_pd
#define _SSE2_orpd_ _mm_or_pd
#define _SSE2_subpd_ _mm_sub_pd

using _int_4_ = __m128i;
using _int64_2_ = __m128i;
using _int_8_ = __m256i;
using _int64_4_ = __m256i;
using _single_4_ = __m128;
using _single_8_ = __m256;
using _double_2_ = __m128d;
using _double_4_ = __m256d;

#endif // _MSC_VER

#endif // 
#endif // X86_64_INTRIN_H_