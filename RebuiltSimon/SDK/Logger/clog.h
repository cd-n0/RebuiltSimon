/* https://gist.github.com/cd-n0/5e564a8187e3fcfa9d6bbd4d35337ecb */
#ifndef CLOG_H_
#define CLOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif

typedef enum {
    CLOG_fatal = 0,
    CLOG_error,
    CLOG_warning,
    CLOG_info,
    CLOG_debug,
    CLOG_trace
} clog_log_level_e;

#define CLOG_LEVEL_COUNT (CLOG_trace + 1)

/* Not for direct usage */
#define _CLOG_FL __FUNCTION__, __LINE__

#define CLOG_FATAL    CLOG_fatal,   _CLOG_FL
#define CLOG_ERROR    CLOG_error,   _CLOG_FL
#define CLOG_WARN     CLOG_warning, _CLOG_FL
#define CLOG_INFO     CLOG_info,    _CLOG_FL
#define CLOG_DEBUG    CLOG_debug,   _CLOG_FL
#define CLOG_TRACE    CLOG_trace,   _CLOG_FL

/* MSVC doesn't have this `__STDC_VERSION__` macro at all but supports __VA_ARGS__ */
#if (__STDC_VERSION__ >= 199901L) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#   define LOG_FATAL(message, ...) clog_log(CLOG_FATAL, message, ##__VA_ARGS__)
#   define LOG_ERROR(message, ...) clog_log(CLOG_ERROR, message, ##__VA_ARGS__)
#   define LOG_WARN(message, ...)  clog_log(CLOG_WARN, message, ##__VA_ARGS__)
#   define LOG_INFO(message, ...)  clog_log(CLOG_INFO, message, ##__VA_ARGS__)
#   define LOG_DEBUG(message, ...) clog_log(CLOG_DEBUG, message, ##__VA_ARGS__)
#   define LOG_TRACE(message, ...) clog_log(CLOG_TRACE, message, ##__VA_ARGS__)
#endif

typedef void(*clog_log_callback_function_t)(clog_log_level_e log_level, const char* message);

#ifndef CLOG_ALLOC
#define CLOG_ALLOC malloc
#endif
#ifndef CLOG_FREE
#define CLOG_FREE free
#endif
/* `snprintf` and `vsnprintf` is not in the ISO C89 standard but most compilers
 * support it anyway unless you enable all warnings and treat them as errors
 * (Implicit function declaration)
 *
 * If you must use ISO C89 no
 * matter what you can specify a replacement function by overriding these
 * definitions
 * Note: a good replacement can be found here:
 * https://github.com/weiss/c99-snprintf
 * A minimal version can also get written and used instead but making it
 * standard compliant is the hard part soo... */
#ifndef CLOG_VSNPRINTF
#define CLOG_VSNPRINTF vsnprintf
#endif
#ifndef CLOG_SNPRINTF
#define CLOG_SNPRINTF snprintf
#endif

int clog_log(clog_log_level_e log_level, const char* file_path, int line, const char* fmt, ...);
void clog_set_log_callback(clog_log_callback_function_t callback);

#endif /* CLOG_H_ */

#ifdef CLOG_IMPLEMENTATION

static clog_log_callback_function_t log_callback;

static const char* log_level_atlas[CLOG_LEVEL_COUNT] = {
    "[FATAL]: ",
    "[ERROR]: ",
    "[WARN]:  ",
    "[INFO]:  ",
    "[DEBUG]: ",
    "[TRACE]: "
};

void clog_set_log_callback(clog_log_callback_function_t callback) {
    log_callback = callback;
}

/* Note: Current win32 implementations doesn't set console attributes back to what they were */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
static const unsigned char levels[CLOG_LEVEL_COUNT] = { 64, 4, 6, 2, 1, 8 };
static void platform_console_write(clog_log_level_e log_level, const char* message) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_handle, levels[log_level]);
    OutputDebugStringA(message);
    size_t length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, 0);
}

static void platform_console_write_error(clog_log_level_e log_level, const char* message) {
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    SetConsoleTextAttribute(console_handle, levels[log_level]);
    OutputDebugStringA(message);
    size_t length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, number_written, 0);
}
#elif defined(__unix__)
static const char* levels[] = { "0;41", "1;31", "1;33", "1;32", "1;34", "1;30" };
static void platform_console_write(clog_log_level_e log_level, const char* message) {
    printf("\033[%sm%s\033[0m", levels[log_level], message);
}

static void platform_console_write_error(clog_log_level_e log_level, const char* message) {
    printf("\033[%sm%s\033[0m", levels[log_level], message);
}
#else
#   error "Unsupported platform"
#endif

/* Logging implementation */
int clog_log(clog_log_level_e log_level, const char* file_path, int line, const char* fmt, ...) {
    va_list va_listp;
    size_t length, prelude_length;
    char* message;

    time_t t = time(NULL);
    char time_str[32] = { 0 };

    if (NULL == file_path || 0 >= (int)strlen(file_path)) file_path = "(null)";

    strcpy(time_str, ctime(&t));
    {
        char* p = strrchr(time_str, '\n');
        if (p) *p = '\0';
    }

    prelude_length = CLOG_SNPRINTF(NULL, 0, "[%s] %s %s:%d: ", time_str, log_level_atlas[log_level], file_path, line);

    va_start(va_listp, fmt);
    length = CLOG_VSNPRINTF(NULL, 0, fmt, va_listp);
    va_end(va_listp);

    length += prelude_length;

    message = (char*)CLOG_ALLOC(length + 2); /* \n and \0 */
    if (!message) return -1;
    CLOG_SNPRINTF(message, prelude_length + 1, "[%s] %s %s:%d: ", time_str, log_level_atlas[log_level], file_path, line);

    va_start(va_listp, fmt);
    CLOG_VSNPRINTF(message + prelude_length, length - prelude_length + 1, fmt, va_listp);
    va_end(va_listp);
    message[length] = '\n';
    message[length + 1] = '\0';

    if (log_level < CLOG_warning) {
        platform_console_write_error(log_level, message);
    }
    else {
        platform_console_write(log_level, message);
    }

    if (log_callback) {
        log_callback(log_level, message);
    }

    CLOG_FREE(message);
    return 0;
}
#endif /* CLOG_IMPLEMENTATION */