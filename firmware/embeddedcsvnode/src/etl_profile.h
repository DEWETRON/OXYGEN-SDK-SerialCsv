#pragma once

// Setting up ETL to be compatible with common boards

// Globally undef min and max
#undef min
#undef max

#if defined(ARDUINO_ARCH_AVR)|| defined(ARDUINO_ARCH_MEGAAVR)
// AVR-specific code
// This should work with most uCs
// General
#define ETL_VERBOSE_ERRORS
#define ETL_CHECK_PUSH_POP
#define ETL_TARGET_DEVICE_GENERIC
#define ETL_TARGET_OS_NONE
#define ETL_COMPILER_GCC
#define ETL_CPP11_SUPPORTED 1
#define ETL_NO_LARGE_CHAR_SUPPORT 1
#define ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED 0
#define ETL_NO_STL

// 8-Bit AVR specficic
#define ETL_NO_CPP_NAN_SUPPORT
#define ETL_NO_HUGE_VAL_SUPPORT

#elif defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)
// SAM-specific code

// General
#define ETL_VERBOSE_ERRORS
#define ETL_CHECK_PUSH_POP
#define ETL_TARGET_DEVICE_GENERIC
#define ETL_TARGET_OS_NONE
#define ETL_COMPILER_GCC
#define ETL_CPP11_SUPPORTED 1
#define ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED 0
#define ETL_NO_STL
#undef ETL_NO_LARGE_CHAR_SUPPORT
#define ETL_NO_LARGE_CHAR_SUPPORT 0

#elif defined(WIN32) || defined(_WIN32)
// Allow testing and development using Windows/MSVC
#include "etl/profiles/msvc_x86.h"
#include "etl/profiles/cpp11.h"

#else
#error "ETL-Profile cannot be determined."
#endif
