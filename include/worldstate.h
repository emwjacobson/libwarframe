#ifndef WORLDSTATE__H
#define WORLDSTATE__H

#include <stdint.h>

typedef struct worldstate {
    char *WorldSeed;
    int Version;
    char *MobileVersion;
    char *BuildLabel;
    int Time;
} worldstate;

#endif