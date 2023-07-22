#include "warframe.h"
#include "network.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static bool wf_initialized = false;
wf_config wf_cfg = {0};

bool is_wf_initialized() {
  return wf_initialized;
}

bool wf_init(wf_config *config)
{
  if (is_wf_initialized())
    return true;

  // Copy config in
  memcpy(&wf_cfg, config, sizeof(wf_config));

  bool res = network_init();
  if (!res)
    return false;

  wf_initialized = true;

  return true;
}

void get_string(cJSON *data, char **ref, char* name) {
  cJSON *tmp = cJSON_GetObjectItem(data, name);
  if (!cJSON_IsString(tmp)) {
    printf("'%s' is not a string!\n", name);
    return;
  }
  int len = strlen(tmp->valuestring);
  *ref = malloc(len + 1);
  memcpy(*ref, tmp->valuestring, len + 1);
}

void get_number(cJSON *data, int *ref, char* name) {
  cJSON *tmp = cJSON_GetObjectItem(data, name);
  *ref = tmp->valueint;
}

worldstate *wf_get_worldstate() {
  if (!is_wf_initialized) return NULL;

  cJSON *data = make_get_request(wf_cfg.wf_ws_url, "");

  if (data == NULL) {
    printf("Error getting world state\n");
    return NULL;
  }

  worldstate *ws = malloc(sizeof(worldstate));

  cJSON *tmp;
  int len;
  get_string(data, &(ws->WorldSeed), "WorldSeed");
  get_number(data, &(ws->Version), "Version");
  get_string(data, &(ws->MobileVersion), "MobileVersion");
  get_string(data, &(ws->BuildLabel), "BuildLabel");
  get_number(data, &(ws->Time), "Time");

  cJSON_Delete(data);
  return ws;
}

void wf_free_worldstate(worldstate *ws) {
  free(ws->WorldSeed);
  free(ws->MobileVersion);
  free(ws->BuildLabel);
  free(ws);
}

void wf_cleanup()
{
  if (!is_wf_initialized()) return;
  
  network_cleanup();
  wf_initialized = false;
}