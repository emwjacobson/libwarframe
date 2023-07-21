#ifndef NETWORK__H
#define NETWORK__H

#include<stdbool.h>
#include<cJSON.h>
#include "warframe.h"

bool network_init();

cJSON *make_get_request(char *url, char *endpoint);

void network_cleanup();

#endif