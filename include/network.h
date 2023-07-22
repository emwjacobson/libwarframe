#ifndef NETWORK__H
#define NETWORK__H

#include<stdbool.h>
#include<cJSON.h>
#include "warframe.h"

typedef struct curl_data
{
  char *response;
  size_t size;
} curl_data;


bool network_init();

curl_data *make_GET_Raw(char *url, char *endpoint);
cJSON *make_GET_JSON(char *url, char *endpoint);

void network_cleanup();

#endif