#ifndef NETWORK__H
#define NETWORK__H

#include<stdbool.h>

bool network_init(char *url);

void make_request(char *endpoint);

void network_cleanup();

#endif