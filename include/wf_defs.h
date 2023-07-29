#ifndef WF_DEFS__H
#define WF_DEFS__H

#include <stdbool.h>

typedef struct ability_t {
    unsigned char *abilityUniqueName;
    unsigned char *abilityName;
    unsigned char *description;
} ability_t;

typedef struct warframe_t {
    unsigned char *uniqueName;
    unsigned char *name;
    unsigned char *parentName;
    unsigned char *description;
    unsigned int health;
    unsigned int shield;
    unsigned int armor;
    unsigned int stamina;
    unsigned int power;
    bool codexSecret;
    unsigned int masteryReq;
    double sprintSpeed;
    unsigned char *passiveDescription;
    unsigned int numAbilities;
    ability_t *abilities;
    unsigned char *productCategory;
} warframe_t;

#endif