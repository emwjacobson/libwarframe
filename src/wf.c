#include "warframe.h"
#include "network.h"
#include <stdbool.h>
#include <string.h>

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

void wf_cleanup()
{
  if (!is_wf_initialized()) return;
  
  network_cleanup();
  wf_initialized = false;
}