#ifndef WARFRAME__H
#define WARFRAME__H

#include <stdbool.h>
#include "worldstate.h"

#define URL_MAX_LENGTH 128
#define ENDPOINT_MAX_LENGTH 64

/**
 * Warframe Market API
 */

typedef struct wfm_config
{
  char wfm_url[URL_MAX_LENGTH]; // API Base URL w/ version
} wfm_config;

#define WFM_DEFAULT_CONFIG                      \
  {                                             \
    .wfm_url = "https://api.warframe.market/v1" \
  }

bool wfm_init(wfm_config *config);
void wfm_get_items();
void wfm_cleanup();

/**
 * Warframe API
 */

typedef struct wf_config
{
  char wf_ws_url[URL_MAX_LENGTH]; // World State URL
  char wf_dt_url[URL_MAX_LENGTH]; // Drop Table URL
  char wf_pe_index_url[URL_MAX_LENGTH]; // Public Export URL
  char wf_pe_content_url[URL_MAX_LENGTH];
} wf_config;

#define WF_DEFAULT_CONFIG                                                                 \
  {                                                                                       \
    .wf_ws_url = "https://content.warframe.com/dynamic/worldState.php",                   \
    .wf_dt_url = "https://n8k6e2y6.ssl.hwcdn.net/repos/hnfvc0o3jnfvc873njb03enrf56.html", \
    .wf_pe_index_url = "https://origin.warframe.com/PublicExport/index_en.txt.lzma", \
    .wf_pe_content_url = "https://content.warframe.com/PublicExport/Manifest/"            \
  }

bool wf_init(wf_config *config);
worldstate *wf_get_worldstate();
void wf_free_worldstate();
void wf_get_warframes();
void wf_cleanup();

#endif