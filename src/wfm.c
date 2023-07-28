#include "warframe.h"
#include "network.h"
#include "debug.h"
#include<stdint.h>
#include<stdio.h>
#include<cJSON.h>
#include<malloc.h>
#include<string.h>


static bool wfm_initialized = false;
wfm_config wfm_cfg = {0};

bool wfm_is_initialized() {
  return wfm_initialized;
}

bool wfm_init(wfm_config *cfg_in) {
  if (wfm_is_initialized()) return true;

  // Copy config in
  memcpy(&wfm_cfg, cfg_in, sizeof(wfm_config));

  bool res;

  res = network_init();
  if (!res) return false;

  wfm_initialized = true;

  return true;
}

void wfm_get_items() {
  if (!wfm_is_initialized()) return;

  cJSON *data = make_GET_JSON(wfm_cfg.wfm_url, "/items");

  if (data == NULL) {
    PRINT_DEBUG("Error getting WFM items.\n");
    return;
  }

  char *string = cJSON_Print(data);

  // TODO: Finish this function :)

  free(string);

  cJSON_Delete(data);
}

void wfm_cleanup() {
  if (!wfm_is_initialized()) return;

  network_cleanup();
  wfm_initialized = false;
}