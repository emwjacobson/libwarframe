#ifndef NETWORK__H
#define NETWORK__H

#include<stdbool.h>
#include<cJSON.h>

bool network_init(char *url);

cJSON *make_get_request(char *endpoint);

void network_cleanup();

#endif