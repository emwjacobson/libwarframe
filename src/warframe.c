#include "warframe.h"
#include "network.h"
#include<stdint.h>
#include<stdio.h>
#include<cJSON.h>


static bool wfm_initialized = false;


bool wfm_init(wf_config cfg_in) {
  if (wfm_initialized) return true;

  bool res;

  res = network_init(cfg_in.wfm_url);
  if (!res) return false;

  wfm_initialized = true;

  return true;
}

void wfm_get_items() {
  make_request("/items");
}

void wfm_cleanup() {
  network_cleanup();
  wfm_initialized = false;
}