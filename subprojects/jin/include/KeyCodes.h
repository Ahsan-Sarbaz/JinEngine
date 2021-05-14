#pragma once

// THIS IS A COPY PASTE OF THE GLFW KEYS RENAMED 

/*************************************************************************
 * GLFW 3.3 - www.glfw.org
 * A library for OpenGL, window and input
 *------------------------------------------------------------------------
 * Copyright (c) 2002-2006 Marcus Geelnard
 * Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would
 *    be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not
 *    be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 *
 *************************************************************************/

#define JIN_KEY_UNKNOWN            -1

/* Printable keys */
#define JIN_KEY_SPACE              32
#define JIN_KEY_APOSTROPHE         39  /* ' */
#define JIN_KEY_COMMA              44  /* , */
#define JIN_KEY_MINUS              45  /* - */
#define JIN_KEY_PERIOD             46  /* . */
#define JIN_KEY_SLASH              47  /* / */
#define JIN_KEY_0                  48
#define JIN_KEY_1                  49
#define JIN_KEY_2                  50
#define JIN_KEY_3                  51
#define JIN_KEY_4                  52
#define JIN_KEY_5                  53
#define JIN_KEY_6                  54
#define JIN_KEY_7                  55
#define JIN_KEY_8                  56
#define JIN_KEY_9                  57
#define JIN_KEY_SEMICOLON          59  /* ; */
#define JIN_KEY_EQUAL              61  /* = */
#define JIN_KEY_A                  65
#define JIN_KEY_B                  66
#define JIN_KEY_C                  67
#define JIN_KEY_D                  68
#define JIN_KEY_E                  69
#define JIN_KEY_F                  70
#define JIN_KEY_G                  71
#define JIN_KEY_H                  72
#define JIN_KEY_I                  73
#define JIN_KEY_J                  74
#define JIN_KEY_K                  75
#define JIN_KEY_L                  76
#define JIN_KEY_M                  77
#define JIN_KEY_N                  78
#define JIN_KEY_O                  79
#define JIN_KEY_P                  80
#define JIN_KEY_Q                  81
#define JIN_KEY_R                  82
#define JIN_KEY_S                  83
#define JIN_KEY_T                  84
#define JIN_KEY_U                  85
#define JIN_KEY_V                  86
#define JIN_KEY_W                  87
#define JIN_KEY_X                  88
#define JIN_KEY_Y                  89
#define JIN_KEY_Z                  90
#define JIN_KEY_LEFT_BRACKET       91  /* [ */
#define JIN_KEY_BACKSLASH          92  /* \ */
#define JIN_KEY_RIGHT_BRACKET      93  /* ] */
#define JIN_KEY_GRAVE_ACCENT       96  /* ` */
#define JIN_KEY_WORLD_1            161 /* non-US #1 */
#define JIN_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define JIN_KEY_ESCAPE             256
#define JIN_KEY_ENTER              257
#define JIN_KEY_TAB                258
#define JIN_KEY_BACKSPACE          259
#define JIN_KEY_INSERT             260
#define JIN_KEY_DELETE             261
#define JIN_KEY_RIGHT              262
#define JIN_KEY_LEFT               263
#define JIN_KEY_DOWN               264
#define JIN_KEY_UP                 265
#define JIN_KEY_PAGE_UP            266
#define JIN_KEY_PAGE_DOWN          267
#define JIN_KEY_HOME               268
#define JIN_KEY_END                269
#define JIN_KEY_CAPS_LOCK          280
#define JIN_KEY_SCROLL_LOCK        281
#define JIN_KEY_NUM_LOCK           282
#define JIN_KEY_PRINT_SCREEN       283
#define JIN_KEY_PAUSE              284
#define JIN_KEY_F1                 290
#define JIN_KEY_F2                 291
#define JIN_KEY_F3                 292
#define JIN_KEY_F4                 293
#define JIN_KEY_F5                 294
#define JIN_KEY_F6                 295
#define JIN_KEY_F7                 296
#define JIN_KEY_F8                 297
#define JIN_KEY_F9                 298
#define JIN_KEY_F10                299
#define JIN_KEY_F11                300
#define JIN_KEY_F12                301
#define JIN_KEY_F13                302
#define JIN_KEY_F14                303
#define JIN_KEY_F15                304
#define JIN_KEY_F16                305
#define JIN_KEY_F17                306
#define JIN_KEY_F18                307
#define JIN_KEY_F19                308
#define JIN_KEY_F20                309
#define JIN_KEY_F21                310
#define JIN_KEY_F22                311
#define JIN_KEY_F23                312
#define JIN_KEY_F24                313
#define JIN_KEY_F25                314
#define JIN_KEY_KP_0               320
#define JIN_KEY_KP_1               321
#define JIN_KEY_KP_2               322
#define JIN_KEY_KP_3               323
#define JIN_KEY_KP_4               324
#define JIN_KEY_KP_5               325
#define JIN_KEY_KP_6               326
#define JIN_KEY_KP_7               327
#define JIN_KEY_KP_8               328
#define JIN_KEY_KP_9               329
#define JIN_KEY_KP_DECIMAL         330
#define JIN_KEY_KP_DIVIDE          331
#define JIN_KEY_KP_MULTIPLY        332
#define JIN_KEY_KP_SUBTRACT        333
#define JIN_KEY_KP_ADD             334
#define JIN_KEY_KP_ENTER           335
#define JIN_KEY_KP_EQUAL           336
#define JIN_KEY_LEFT_SHIFT         340
#define JIN_KEY_LEFT_CONTROL       341
#define JIN_KEY_LEFT_ALT           342
#define JIN_KEY_LEFT_SUPER         343
#define JIN_KEY_RIGHT_SHIFT        344
#define JIN_KEY_RIGHT_CONTROL      345
#define JIN_KEY_RIGHT_ALT          346
#define JIN_KEY_RIGHT_SUPER        347
#define JIN_KEY_MENU               348

#define JIN_MOD_SHIFT           0x0001
#define JIN_MOD_CONTROL         0x0002
#define JIN_MOD_ALT             0x0004
#define JIN_MOD_SUPER           0x0008
#define JIN_MOD_CAPS_LOCK       0x0010
#define JIN_MOD_NUM_LOCK        0x0020



#define JIN_MOUSE_BUTTON_1         0
#define JIN_MOUSE_BUTTON_2         1
#define JIN_MOUSE_BUTTON_3         2
#define JIN_MOUSE_BUTTON_4         3
#define JIN_MOUSE_BUTTON_5         4
#define JIN_MOUSE_BUTTON_6         5
#define JIN_MOUSE_BUTTON_7         6
#define JIN_MOUSE_BUTTON_8         7
#define JIN_MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_8
#define JIN_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1
#define JIN_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2
#define JIN_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3
