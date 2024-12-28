#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned char       ubyte;
typedef unsigned short      ubyte2;
typedef unsigned int        ubyte4;
typedef unsigned long long  ubyte8;

typedef signed char       sbyte;
typedef signed short      sbyte2;
typedef signed int        sbyte4;
typedef signed long long  sbyte8;

typedef float   f32;
typedef double  f64;
typedef long double f80, f96, f128;

#define LOTUS_TRUE     1
#define LOTUS_FALSE    0

#define LOTUS_LITERAL(type, value) &(type){value}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define LOTUS_PLATFORM_WINDOWS 1
    #ifndef _WIN64
        #error "64-bits required on Windows"
    #endif
#elif defined(__linux__) || defined(__gnu_linux__)
    #define LOTUS_PLATFORM_LINUX 1
    #if defined(__ANDROID__)
        #define LOTUS_PLATFORM_ANDROID 1
    #endif
#elif defined(__APPLE__) && defined(__MACH__)
    #define LOTUS_PLATFORM_MACOS 1
#endif

#ifdef LOTUS_EXPORT
    #ifdef _MSC_VER
        #define LOTUS_API_ENTRY __declspec(dllexport)
    #elif defined(__GNUC__) || defined(__clang__)
        #define LOTUS_API_ENTRY __attribute__((visibility("default")))
    #else
        #define LOTUS_API_ENTRY
    #endif
#else
    #ifdef _MSC_VER
        #define LOTUS_API_ENTRY __declspec(dllimport)
    #elif defined(__GNUC__) || defined(__clang__)
        #define LOTUS_API_ENTRY __attribute__((visibility("default")))
    #else
        #define LOTUS_API_ENTRY
    #endif
#endif

#ifdef _MSC_VER
    #define _LOTUS_HIDDEN
#elif defined(__GNUC__) || defined(__clang__)
    #define _LOTUS_HIDDEN __attribute__((visibility("hidden")))
#endif

#ifndef _LOTUS_PRIVATE
    #if defined(__GNUC__) || defined(__clang__)
        #define _LOTUS_PRIVATE __attribute__((unused)) static
    #else
        #define _LOTUS_PRIVATE static
    #endif
#endif
