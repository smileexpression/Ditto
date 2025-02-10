#ifndef _DEBUG_H_
#define _DEBUG_H_

#define L_DEBUG 0
#define L_INFO 1
#define L_ERROR 2

// 定义了调试信息的输出级别（DEBUG、INFO、ERROR）
#define STR_L_DEBUG "[DEBUG]"
#define STR_L_INFO "[INFO]"
#define STR_L_ERROR "[ERROR]"

#define VERBO L_INFO

#include <stdio.h>

// 使用宏`printd`根据设置的详细级别（VERBO）输出调试信息
// 在非调试模式下（未定义`_DEBUG`时），`printd`宏不会输出任何内容
#ifdef _DEBUG
#define printd(level, fmt, ...)                                         \
  do {                                                                  \
    if (level >= VERBO)                                                 \
      printf(STR_##level " %s:%d:%s():\t" fmt "\n", __FILE__, __LINE__, \
             __func__, ##__VA_ARGS__);                                  \
  } while (0)
#else
#define printd(level, fmt, ...)
#endif

#endif