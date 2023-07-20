#include "warframe.h"
#include "network.h"
#include<stdint.h>
#include<stdio.h>
#include<cJSON.h>

#define SERVER "https://api.warframe.market/v1"


static bool wfm_initialized = false;

bool wfm_init() {
  bool res;
  
  res = network_init();
  if (!res) return false;

  wfm_initialized = true;

  return true;
}

void wfm_getItems() {
  // curl_easy_setopt(curl, CURLOPT_URL, SERVER "/items");

  // CURLcode res = curl_easy_perform(curl);
  
  // if (res != CURLE_OK) {
  //   printf("Error getting items: %i\n", res);
  //   return;
  // }

  // long response_code;
  // curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

  // printf("Returned with response code %li\n", response_code);
}

void wfm_cleanup() {
  network_cleanup();
  wfm_initialized = false;
}