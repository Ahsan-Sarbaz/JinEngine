#pragma once
#include <stdio.h>
#include "Colors.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#define LOG_TRACE(...) printf("TRACE: "  __VA_ARGS__)
#define LOG_INFO(...) printf("INFO: " __VA_ARGS__)
#define LOG_WARN(...) printf("WARN: " __VA_ARGS__)
#define LOG_ERROR(...) printf("ERROR: " __VA_ARGS__)
#define LOG_FATAL(...) printf("FATAL: " __VA_ARGS__)

#else

#define LOG_TRACE(...) printf(BHWHT "TRACE: " RESET  __VA_ARGS__)
#define LOG_INFO(...) printf(BHGRN "INFO: " RESET  __VA_ARGS__)
#define LOG_WARN(...) printf(BHYEL "WARN: " RESET  __VA_ARGS__)
#define LOG_ERROR(...) printf(BHMAG "ERROR: " RESET  __VA_ARGS__)
#define LOG_FATAL(...) printf(BHRED "FATAL: " RESET  __VA_ARGS__)

#endif // WIN32 || _WIN32


