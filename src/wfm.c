#include "warframe.h"
#include "network.h"
#include<stdint.h>
#include<stdio.h>
#include<cJSON.h>
#include<malloc.h>
#include<string.h>


static bool wfm_initialized = false;
wfm_config wfm_cfg = {0};


bool wfm_init(wfm_config *cfg_in) {
  if (wfm_initialized) return true;

  // Copy config in
  memcpy(&wfm_cfg, cfg_in, sizeof(wfm_config));

  bool res;

  res = network_init();
  if (!res) return false;

  wfm_initialized = true;

  return true;
}

void wfm_get_items() {
  cJSON *data = make_get_request(wfm_cfg.wfm_url, "/items");

  if (data == NULL) {
    printf ("Error making request.");
    return;
  }

  char *string = cJSON_Print(data);

  printf("%s", string);

  free(string);

  cJSON_Delete(data);
}

void wfm_cleanup() {
  network_cleanup();
  wfm_initialized = false;
}