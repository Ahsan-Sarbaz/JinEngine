#pragma once
#include <stdio.h>
#include "Colors.h"

#define LOG_TRACE(...) printf(BHWHT "TRACE: " RESET  __VA_ARGS__)
#define LOG_INFO(...) printf(BHGRN "INFO: " RESET  __VA_ARGS__)
#define LOG_WARN(...) printf(BHYEL "WARN: " RESET  __VA_ARGS__)
#define LOG_ERROR(...) printf(BHMAG "ERROR: " RESET  __VA_ARGS__)
#define LOG_FATAL(...) printf(BHRED "FATAT: " RESET  __VA_ARGS__)
