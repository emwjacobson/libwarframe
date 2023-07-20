#include "network.h"

#include<stdint.h>
#include<stdio.h>
#include<curl/curl.h>

static CURL *curl = NULL;

bool network_init() {
  curl = curl_easy_init();
  if (curl == NULL) return false;

//   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, TODO);

  return true;
}

void network_cleanup() {
  curl_easy_cleanup(curl);
}
