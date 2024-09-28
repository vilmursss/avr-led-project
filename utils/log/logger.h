#ifndef UTILS_LOGGER_H
#define UTILS_LOGGER_H

// ERROR level log print
void log_error(const char* msg, ...);

// WARNING level log print
void log_warning(const char* msg, ...);

// INFO level log print
void log_info(const char* msg, ...);

// DEBUG level log print
void log_debug(const char* msg, ...);

#endif // UTILS_LOGGER_H
