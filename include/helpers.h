#ifndef HELPERS__H
#define HELPERS__H

#include "debug.h"
#include <cJSON.h>
#include <malloc.h>
#include <string.h>

void json_get_string(cJSON *data, unsigned char **ref, char* name) {
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
  *ref = (unsigned char *)malloc(len + 1);
  memcpy(*ref, tmp->valuestring, len + 1);
}

void json_get_number(cJSON *data, unsigned int *ref, char* name) {
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

void json_get_boolean(cJSON *data, bool *ref, char* name) {
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

void json_get_double(cJSON *data, double *ref, char* name) {
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


#endif