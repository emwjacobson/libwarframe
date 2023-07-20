#include "warframe.h"

#define SERVER "https://api.warframe.market/"
#define VERSION "v1"

char *api_url = SERVER VERSION;

#include<curl/curl.h>
#include<cJSON.h>

int test() {
  cJSON *monitor = cJSON_CreateObject();
  if (monitor == NULL) {
    return -1;
  }

  return 11;
}
