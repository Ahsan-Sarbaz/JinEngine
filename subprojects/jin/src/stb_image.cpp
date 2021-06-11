#define STB_IMAGE_IMPLEMENTATION

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "../include/stb_image.h"
#else
#include "stb_image.h"
#endif