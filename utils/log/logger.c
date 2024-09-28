#include "logger.h"
#include "logger_defs.h"

#include <clock/system_clock.h>

#include <stdarg.h>
#include <stdio.h>

// Local functions
static const char* current_timestamp();
static const char* log_level_to_str(const LogLevel level);
static void log_message(const LogLevel level, const char* msg, ...);

// ERROR level log print
void log_error(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    log_message(LOG_ERROR, msg, args);
    va_end(args);
}

// WARNING level log print
void log_warning(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    log_message(LOG_WARNING, msg, args);
    va_end(args);
}

// INFO level log print
void log_info(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    log_message(LOG_INFO, msg, args);
    va_end(args);
}

// DEBUG level log print
void log_debug(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    log_message(LOG_DEBUG, msg, args);
    va_end(args);
}

static const char* current_timestamp()
{
    return "00:00:00";
}

static const char* log_level_to_str(const LogLevel level)
{
    const char* level_str;
    switch (level)
    {
        case LOG_ERROR:
            level_str = "ERROR";
            break;
        case LOG_WARNING:
            level_str = "WARNING";
            break;
        case LOG_INFO:
            level_str = "INFO";
            break;
        case LOG_DEBUG:
            level_str = "DEBUG";
            break;
        default:
            level_str = "UNKNOWN";
            break;
    }

    return level_str;
}

static void log_message(const LogLevel level, const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    printf("[%s] %s: ", current_timestamp(), log_level_to_str(level));
    vprintf(msg, args);
    printf("\n");
    va_end(args);
}