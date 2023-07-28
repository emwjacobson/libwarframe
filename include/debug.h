#ifndef DEBUG__H
#define DEBUG__H

#ifdef WF_DEBUG
    #define PRINT_DEBUG(...) printf(__VA_ARGS__)
#else
    #define PRINT_DEBUG(...) 
#endif

#endif