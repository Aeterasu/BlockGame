#pragma once

// 0 - no logs
// 1 - only errors
// 2 - warnings and info
#ifndef LOG_LEVEL
#define LOG_LEVEL 2
#endif

void log_info(const char* format, ...);
void log_warning(const char* file, const int line, const char* format, ...);
void log_error(const char* file, const int line, const char* format, ...);

#ifndef LOG_IMPLEMENTATION
#if LOG_LEVEL >= 2
#define log_info(...) log_info(__VA_ARGS__)
#define log_warning(...) log_warning(__FILE__, __LINE__, __VA_ARGS__)
#else
#define log_info(...) ((void)0)
#define log_warning(...) ((void)0)
#endif

#if LOG_LEVEL >= 1
#define log_error(...) log_error(__FILE__, __LINE__, __VA_ARGS__)
#else
#define log_error(...) ((void)0)
#endif
#endif
