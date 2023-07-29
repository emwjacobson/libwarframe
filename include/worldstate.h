#ifndef WORLDSTATE__H
#define WORLDSTATE__H

#include <stdint.h>

typedef struct worldstate {
    unsigned char *WorldSeed;
    unsigned int Version;
    unsigned char *MobileVersion;
    unsigned char *BuildLabel;
    unsigned int Time;
} worldstate;

#endif