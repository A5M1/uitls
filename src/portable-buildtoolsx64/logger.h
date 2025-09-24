#pragma once
#include "config.h"
#include <stdio.h>

#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_RESET   "\x1b[0m"

#if ENABLE_LOGGING
    #define LOG_INFO(fmt, ...)  do { \
        printf(ANSI_GREEN "[INFO] " ANSI_RESET); \
        printf(fmt, ##__VA_ARGS__); \
        printf("\n"); \
    } while(0)

    #define LOG_WARN(fmt, ...)  do { \
        printf(ANSI_YELLOW "[WARN] " ANSI_RESET); \
        printf(fmt, ##__VA_ARGS__); \
        printf("\n"); \
    } while(0)

    #define LOG_ERROR(fmt, ...) do { \
        printf(ANSI_RED "[ERROR] " ANSI_RESET); \
        printf(fmt, ##__VA_ARGS__); \
        printf("\n"); \
    } while(0)

    #if ENABLE_DEBUG_LOG
        #define LOG_DEBUG(fmt, ...) do { \
            printf(ANSI_BLUE "[DEBUG] " ANSI_RESET); \
            printf(fmt, ##__VA_ARGS__); \
            printf("\n"); \
        } while(0)
    #else
        #define LOG_DEBUG(fmt, ...) do {} while(0)
    #endif

#else
    #define LOG_INFO(fmt, ...)  do {} while(0)
    #define LOG_WARN(fmt, ...)  do {} while(0)
    #define LOG_ERROR(fmt, ...) do {} while(0)
    #define LOG_DEBUG(fmt, ...) do {} while(0)
#endif

void logger_enable_ansi(void);
