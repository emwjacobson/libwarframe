#include "warframe.h"
#include "network.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <LzmaDec.h>
#include "debug.h"  

static bool wf_initialized = false;
wf_config wf_cfg = {0};

// Export URLs
typedef struct wf_content {
  char Customs[ENDPOINT_MAX_LENGTH];
  char Drones[ENDPOINT_MAX_LENGTH];
  char Flavour[ENDPOINT_MAX_LENGTH];
  char FusionBundles[ENDPOINT_MAX_LENGTH];
  char Gear[ENDPOINT_MAX_LENGTH];
  char Keys[ENDPOINT_MAX_LENGTH];
  char Recipes[ENDPOINT_MAX_LENGTH];
  char Regions[ENDPOINT_MAX_LENGTH];
  char RelicArcane[ENDPOINT_MAX_LENGTH];
  char Resources[ENDPOINT_MAX_LENGTH];
  char Sentinels[ENDPOINT_MAX_LENGTH];
  char SortieRewards[ENDPOINT_MAX_LENGTH];
  char Upgrades[ENDPOINT_MAX_LENGTH];
  char Warframes[ENDPOINT_MAX_LENGTH];
  char Weapons[ENDPOINT_MAX_LENGTH];
  char Manifest[ENDPOINT_MAX_LENGTH];
} wf_content;
static wf_content content_endpoints = {0};

bool is_wf_initialized() {
  return wf_initialized;
}

void *SzAlloc(const ISzAlloc *p, size_t size) { p = p; return malloc(size); }
void SzFree(const ISzAlloc *p, void *address) { p = p; free(address); }
static ISzAlloc alloc = { SzAlloc, SzFree };

bool wf_get_pe_index() {
  if (!is_wf_initialized()) return false;

  curl_data *data = make_GET_Raw(wf_cfg.wf_pe_index_url, "");

  size_t out_len = 2 * data->size; // This should be enough?
  unsigned char *decoded = malloc(out_len);
  ELzmaStatus status;
  size_t in_size = data->size - 13;
  SRes res = LzmaDecode(decoded, &out_len, &data->response[13], &in_size, 
    data->response, LZMA_PROPS_SIZE, LZMA_FINISH_ANY, 
    &status, &alloc);

  if (res != SZ_OK) {
    PRINT_WARN("Bad res value from LzmaDecode: %i\n", res);
    free(decoded);
    return false;
  }

  FILE *f = fmemopen(decoded, out_len, "r");

  char *line = NULL;
  size_t len = 0;
  size_t read;
  while ((read = getline(&line, &len, f)) != -1) {
    char *tmp_str = malloc(read);
    strncpy(tmp_str, line, read);
    char *ctnt_type = strtok(tmp_str, "_");

    char *ref = NULL;
    if (strncmp(ctnt_type, "ExportCustoms", sizeof("ExportCustoms")) == 0) {
      ref = content_endpoints.Customs;
    } else if (strncmp(ctnt_type, "ExportDrones", sizeof("ExportDrones")) == 0) {
      ref = content_endpoints.Drones;
    } else if (strncmp(ctnt_type, "ExportFlavour", sizeof("ExportFlavour")) == 0) {
      ref = content_endpoints.Flavour;
    } else if (strncmp(ctnt_type, "ExportFusionBundles", sizeof("ExportFusionBundles")) == 0) {
      ref = content_endpoints.FusionBundles;
    } else if (strncmp(ctnt_type, "ExportGear", sizeof("ExportGear")) == 0) {
      ref = content_endpoints.Gear;
    } else if (strncmp(ctnt_type, "ExportKeys", sizeof("ExportKeys")) == 0) {
      ref = content_endpoints.Keys;
    } else if (strncmp(ctnt_type, "ExportRecipes", sizeof("ExportRecipes")) == 0) {
      ref = content_endpoints.Recipes;
    } else if (strncmp(ctnt_type, "ExportRegions", sizeof("ExportRegions")) == 0) {
      ref = content_endpoints.Regions;
    } else if (strncmp(ctnt_type, "ExportRelicArcane", sizeof("ExportRelicArcane")) == 0) {
      ref = content_endpoints.RelicArcane;
    } else if (strncmp(ctnt_type, "ExportResources", sizeof("ExportResources")) == 0) {
      ref = content_endpoints.Resources;
    } else if (strncmp(ctnt_type, "ExportSentinels", sizeof("ExportSentinels")) == 0) {
      ref = content_endpoints.Sentinels;
    } else if (strncmp(ctnt_type, "ExportSortieRewards", sizeof("ExportSortieRewards")) == 0) {
      ref = content_endpoints.SortieRewards;
    } else if (strncmp(ctnt_type, "ExportUpgrades", sizeof("ExportUpgrades")) == 0) {
      ref = content_endpoints.Upgrades;
    } else if (strncmp(ctnt_type, "ExportWarframes", sizeof("WExportarframes")) == 0) {
      ref = content_endpoints.Warframes;
    } else if (strncmp(ctnt_type, "ExportWeapons", sizeof("ExportWeapons")) == 0) {
      ref = content_endpoints.Weapons;
    } else {
      ctnt_type = strtok(ctnt_type, ".");
      if (strncmp(ctnt_type, "ExportManifest", sizeof("ExportManifest")) == 0) {
        ref = content_endpoints.Manifest;
      }
    }

    if (ref != NULL) {
                  // Remove /r/n
      strncpy(ref, strtok(strtok(line, "\r"), "\n"), read);
    } else {
      PRINT_INFO("Got unknown content type: %s\n", ctnt_type);
    }
    free(tmp_str);
  }

  fclose(f);

  free(data->response);
  free(data);
  free(line);
  free(decoded);

  return true;
}

bool wf_init(wf_config *config)
{
  if (is_wf_initialized())
    return true;

  // Copy config in
  memcpy(&wf_cfg, config, sizeof(wf_config));

  bool res = network_init();
  if (!res) {
    PRINT_ERROR("WF init - error initializing network\n");
    return false;
  }

  wf_initialized = true;
  
  res = wf_get_pe_index();
  if (!res) {
    PRINT_ERROR("Error initializing WF Endpoints\n");
    wf_initialized = false;
    return false;
  }

  return true;
}

void get_string(cJSON *data, unsigned char **ref, char* name) {
  cJSON *tmp = cJSON_GetObjectItem(data, name);
  if (tmp == NULL) {
    // NULLs can happen with optional fields
    PRINT_DEBUG("String parse error: '%s' is NULL (might be optional field)\n", name);
    *ref = NULL;
    return;
  }
  if (!cJSON_IsString(tmp)) {
    PRINT_WARN("String parse error: '%s' is not a string type: %i!\n", name, tmp->type);
    *ref = NULL;
    return;
  }
  int len = strlen(tmp->valuestring);
  *ref = malloc(len + 1);
  memcpy(*ref, tmp->valuestring, len + 1);
}

void get_number(cJSON *data, unsigned int *ref, char* name) {
  cJSON *tmp = cJSON_GetObjectItem(data, name);
  if (tmp == NULL) {
    PRINT_DEBUG("Number parse error: '%s' is NULL (might be optional field)\n", name);
    *ref = 0;
    return;
  }
  if (!cJSON_IsNumber(tmp)) {
    PRINT_WARN("Number parse error: '%s' is not a number type: %i!\n", name, tmp->type);
    *ref = 0;
    return;
  }
  *ref = tmp->valueint;
}

void get_boolean(cJSON *data, bool *ref, char* name) {
  cJSON *tmp = cJSON_GetObjectItem(data, name);
  if (tmp == NULL) {
    PRINT_DEBUG("Boolean parse error: '%s' is NULL (might be optional field)\n", name);
    *ref = 0;
    return;
  }
  if (!cJSON_IsBool(tmp)) {
    PRINT_WARN("Boolean parse error: '%s' is not a boolean type: %i!\n", name, tmp->type);
    *ref = 0;
    return;
  }
  *ref = cJSON_IsTrue(tmp);
}

void get_double(cJSON *data, double *ref, char* name) {
  cJSON *tmp = cJSON_GetObjectItem(data, name);
  if (tmp == NULL) {
    PRINT_DEBUG("Double parse error: '%s' is NULL (might be optional field)\n", name);
    *ref = 0;
    return;
  }
  if (!cJSON_IsNumber(tmp)) {
    PRINT_WARN("Double parse error: '%s' is not a double type: %i!\n", name, tmp->type);
    *ref = 0;
    return;
  }
  *ref = tmp->valuedouble;
}

worldstate *wf_get_worldstate() {
  if (!is_wf_initialized()) return NULL;

  cJSON *data = make_GET_JSON(wf_cfg.wf_ws_url, "");

  if (data == NULL) {
    PRINT_WARN("WF - Error getting world state JSON\n");
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

  // TODO: Finish parsing JSON

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

warframe_t *wf_get_warframes(int *num_wf_out) {
  if (!is_wf_initialized()) return NULL;

  cJSON *data = make_GET_JSON(wf_cfg.wf_pe_content_url, content_endpoints.Warframes);

  if (data == NULL) {
    PRINT_WARN("WF - Error getting warframe JSON\n");
    return NULL;
  }

  // ExportWarframes
  cJSON *export_warframes = cJSON_GetObjectItem(data, "ExportWarframes");
  int num_warframes = cJSON_GetArraySize(export_warframes);
  warframe_t *warframes = malloc(num_warframes * sizeof(warframe_t));

  int i = 0;
  cJSON *wf_json;
  cJSON_ArrayForEach(wf_json, export_warframes) {
    warframe_t *wf = &(warframes[i]);
    get_string(wf_json, &(wf->uniqueName), "uniqueName");
    get_string(wf_json, &(wf->name), "name");
    get_string(wf_json, &(wf->parentName), "parentName");
    get_string(wf_json, &(wf->description), "description");
    get_number(wf_json, &(wf->health), "health");
    get_number(wf_json, &(wf->shield), "shield");
    get_number(wf_json, &(wf->armor), "armor");
    get_number(wf_json, &(wf->stamina), "stamina");
    get_number(wf_json, &(wf->power), "power");
    get_boolean(wf_json, &(wf->codexSecret), "codexSecret");
    get_number(wf_json, &(wf->masteryReq), "masteryReq");
    get_double(wf_json, &(wf->sprintSpeed), "sprintSpeed");
    get_string(wf_json, &(wf->productCategory), "productCategory");

    // "Optional" item, might be NULL.
    get_string(wf_json, &(wf->passiveDescription), "passiveDescription");

    cJSON *abilities = cJSON_GetObjectItem(wf_json, "abilities");
    wf->numAbilities = cJSON_GetArraySize(abilities);
    wf->abilities = malloc(wf->numAbilities * sizeof(ability_t));

    cJSON *ability_json;
    int j = 0;
    cJSON_ArrayForEach(ability_json, abilities) {
      get_string(ability_json, &(wf->abilities[j].abilityUniqueName), "abilityUniqueName");
      get_string(ability_json, &(wf->abilities[j].abilityName), "abilityName");
      get_string(ability_json, &(wf->abilities[j].description), "description");

      j++;
    }

    i++;
  }

  // TODO: ExportAbilities

  cJSON_Delete(data);

  // Only set at end. Early returns might modify `num_wf_out` but return NULL.
  *num_wf_out = num_warframes;
  return warframes;
}

void wf_free_warframes(warframe_t *warframes, int num_warframes) {
  warframe_t *wf;
  for (int i=0; i<num_warframes; i++) {
    wf = &warframes[i];

    free(wf->uniqueName);
    free(wf->name);
    free(wf->parentName);
    free(wf->description);
    free(wf->passiveDescription);
    free(wf->productCategory);

    ability_t *ab;
    for (int j=0; j<wf->numAbilities; j++) {
      ab = &(wf->abilities[j]);

      free(ab->abilityUniqueName);
      free(ab->abilityName);
      free(ab->description);
    }

    free(wf->abilities);
  }

  free(warframes);
}

void wf_cleanup()
{
  if (!is_wf_initialized()) return;
  
  network_cleanup();
  wf_initialized = false;
}