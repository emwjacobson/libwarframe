#ifndef DEBUG__H
#define DEBUG__H

#define ERROR 1
#define WARN  2
#define INFO  3
#define DEBUG 4
#define TRACE 5

#define BLACK    "\033[0;30m"
#define RED      "\033[0;31m"
#define GREEN    "\033[0;32m"
#define YELLOW   "\033[0;33m"
#define BLUE     "\033[0;34m"
#define PURPLE   "\033[0;35m"
#define CYAN     "\033[0;36m"
#define WHITE    "\033[0;37m"
#define B_BLACK  "\033[1;30m"
#define B_RED    "\033[1;31m"
#define B_GREEN  "\033[1;32m"
#define B_YELLOW "\033[1;33m"
#define B_BLUE   "\033[1;34m"
#define B_PURPLE "\033[1;35m"
#define B_CYAN   "\033[1;36m"
#define B_WHITE  "\033[1;37m"
#define RESET   "\033[0m"

#if PRINT_LEVEL >= ERROR
    #define PRINT_ERROR(...) fprintf(stderr, B_RED "[ERROR] " RESET __VA_ARGS__)
#endif
#if PRINT_LEVEL >= WARN
    #define PRINT_WARN(...) fprintf(stderr, B_YELLOW "[WARN] " RESET __VA_ARGS__)
#endif
#if PRINT_LEVEL >= INFO
    #define PRINT_INFO(...) fprintf(stderr, B_CYAN "[INFO] " RESET __VA_ARGS__)
#endif
#if PRINT_LEVEL >= DEBUG
    #define PRINT_DEBUG(...) fprintf(stderr, CYAN "[DEBUG] " RESET __VA_ARGS__)
#endif
#if PRINT_LEVEL >= TRACE
    #define PRINT_TRACE(...) fprintf(stderr, "[TRACE] " __VA_ARGS__)
#endif

#ifndef PRINT_ERROR
    #define PRINT_ERROR(...)
#endif
#ifndef PRINT_WARN
    #define PRINT_WARN(...)
#endif
#ifndef PRINT_INFO
    #define PRINT_INFO(...)
#endif
#ifndef PRINT_DEBUG
    #define PRINT_DEBUG(...)
#endif
#ifndef PRINT_TRACE
    #define PRINT_TRACE(...)
#endif

#endif