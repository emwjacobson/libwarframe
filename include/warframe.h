#ifndef WARFRAME__H
#define WARFRAME__H

#include<stdbool.h>

typedef struct wf_config {
    char wfm_url[64];
} wf_config;

#define DEFAULT_CONFIG { \
        .wfm_url="https://api.warframe.market/v1" \
    }

bool wfm_init(wf_config config);

void wfm_get_items();

void wfm_cleanup();

#endif