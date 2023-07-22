#include "warframe.h"
#include "network.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <LzmaDec.h>

static bool wf_initialized = false;
wf_config wf_cfg = {0};

bool is_wf_initialized() {
  return wf_initialized;
}

void *SzAlloc(const ISzAlloc *p, size_t size) { p = p; return malloc(size); }
void SzFree(const ISzAlloc *p, void *address) { p = p; free(address); }
static ISzAlloc alloc = { SzAlloc, SzFree };

void wf_get_pe_index() {
  if (!is_wf_initialized) return;

  curl_data *data = make_GET_Raw(wf_cfg.wf_pe_url, "");

  printf("Size: %li\n", data->size);

  size_t decoded_len = 2 * data->size;
  char *decoded = malloc(decoded_len);
  ELzmaStatus status;
  size_t in_size = data->size - 13;
  SRes res = LzmaDecode(decoded, &decoded_len, &data->response[13], &in_size, 
    data->response, LZMA_PROPS_SIZE, LZMA_FINISH_ANY, 
    &status, &alloc);

  printf("Res: %i\n", res);
  printf("Output size: %li\n", decoded_len);
  printf("Decoded:\n%s\n", decoded);

  free(decoded);
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

  wf_get_pe_index();

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

  cJSON *data = make_GET_JSON(wf_cfg.wf_ws_url, "");

  if (data == NULL) {
    printf("Error getting world state\n");
    return NULL;
  }

  worldstate *ws = malloc(sizeof(worldstate));

  /**
   * Basic Data
   */
  get_string(data, &(ws->WorldSeed), "WorldSeed");
  get_number(data, &(ws->Version), "Version");
  get_string(data, &(ws->MobileVersion), "MobileVersion");
  get_string(data, &(ws->BuildLabel), "BuildLabel");
  get_number(data, &(ws->Time), "Time");
  // TODO: "Tmp" variable?

  /**
   * Events
   */

  /**
   * Goals
   */

  /**
   * Alerts
   */

  /**
   * Sorties
   */

  /**
   * LiteSorties
   */

  /**
   * SyndicateMissions
   */

  /**
   * ActiveMissions
   */

  /**
   * GlobalUpgrades
   */

  /**
   * FlashSales
   */
  
  /**
   * InGameMarket
   */

  /**
   * Invasions
   */

  /**
   * HubEvents
   */

  /**
   * NodeOverrides
   */

  /**
   * VoidTraders
   */

  /**
   * PrimeVaultTraders
   */

  /**
   * VoidStorms
   */

  /**
   * PrimeAccessAvailability
   */

  /**
   * PrimeVaultAvailabilities
   */

  /**
   * PrimeTokenAvailability
   */

  /**
   * DailyDeals
   */

  /**
   * LibraryInfo
   */

  /**
   * PVPChallengeInstances
   */

  /**
   * PersistentEnemies
   */

  /**
   * PVPAlternativeModes
   */

  /**
   * PVPActiveTournaments
   */

  /**
   * ProjectPct
   */

  /**
   * ConstructionProjects
   */

  /**
   * TwitchPromos
   */

  /**
   * ExperimentRecommended
   */

  /**
   * EndlessXpChoices
   */

  /**
   * ForceLogoutVersion
   */

  /**
   * FeaturedGuilds
   */

  /**
   * SeasonInfo
   */


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