#ifndef WARFRAME__H
#define WARFRAME__H

#include<stdbool.h>

typedef struct wfm_config {
    char wfm_url[64];
} wfm_config;

#define DEFAULT_CONFIG { \
        .wfm_url="https://api.warframe.market/v1" \
    }

bool wfm_init(wfm_config config);
void wfm_get_items();
void wfm_cleanup();

bool wf_init();
void wf_cleanup();

#endif