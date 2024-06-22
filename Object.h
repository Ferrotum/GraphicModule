#pragma once
#ifndef OBJECT_H_
#define OBJECT_H_

#if defined(_M_IX86) || defined(_M_AMD64) || defined(__x86_64__) || defined(__i386__)
#define USE_RDTSC_INSTRUCTION 1
#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#define x86_rdtsc __rdtsc
#elif defined (__GNUC__)
#include <x86intrin.h>
#define x86_rdtsc __rdtsc
#endif //
#elif defined (_M_ARM64) || defined (__aarch64__)
static_assert(false, "isnt made yet");
#endif // 

#include <atomic>
#include <cstring>
#include <cstdint>
#include <limits.h>
#include <string>
#include <typeinfo>
#include <mutex>

#include "Storage.h"

class Object
{
private:
    void _moveProc(Object& a) noexcept;
protected:
    std::atomic<bool> _modified;
    volatile uint_fast64_t _modifiedTime;
    volatile mutable uint_fast64_t _lastUsage;

    Object() :
        _modified(true),
        _modifiedTime(0),
        _lastUsage(0)
    {
        _hasModified();
        _setLastUsage();
    }
    void _hasModified() noexcept
    {
#ifdef USE_RDTSC_INSTRUCTION
        _modifiedTime = x86_rdtsc();
#endif // USE_RDTSC_INSTRUCTION
        _modified.store(true, std::memory_order_release);
    }
    void _setLastUsage() const noexcept
    {
#ifdef USE_RDTSC_INSTRUCTION
        _lastUsage = x86_rdtsc();
#endif // USE_RDTSC_INSTRUCTION
    }
public:
    Object(const Object&) = delete;
    Object(Object&& a) noexcept
    {
        _moveProc(a);
    }
    Object& operator=(const Object&) = delete;
    Object& operator=(Object&& a) noexcept
    {
        if (this != &a) {
            _moveProc(a);
        }
        return *this;
    }
    std::atomic<bool>* modifiedFlagPtr() noexcept
    {
        return &_modified;
    }
    unsigned long long lastModification() const noexcept
    {
        return _modifiedTime;
    }
    // Гуси, гуси га-га-га. Есть хотите? Да, да, да!
    virtual ~Object() noexcept
    {

    }
};

//#ifdef USE_RDTSC_INSTRUCTION
//#undef USE_RDTSC_INSTRUCTION
//#endif // USE_RDTSC_INSTRUCTION

#endif // OBJECT_H_