#include "network.h"
#include "warframe.h"

#include <stdint.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

static bool network_initialized = false;
static CURL *curl = NULL;

typedef struct curl_data
{
  char *response;
  size_t size;
} curl_data;

// https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
static size_t write_callback(char *data, size_t size, size_t nmemb, void *userdata)
{
  size_t realsize = size * nmemb;
  struct curl_data *mem = (struct curl_data *)userdata;

  char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if (ptr == NULL)
    return 0; // Out of memory

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;

  return realsize;
}

bool network_init()
{
  if (network_initialized)
    return true;

  curl = curl_easy_init();
  if (curl == NULL)
    return false;

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

  network_initialized = true;

  return true;
}

/**
 * @brief Makes a GET request to `endpoint`
 *
 * @param endpoint The endpoint to make the request to
 * @return cJSON* A JSON object. User is responsible for deallocating with cJSON_Delete()
 */
cJSON *make_get_request(char *url, char *endpoint)
{
  if (!network_initialized || curl == NULL)
    return NULL;

  curl_data chunk = {0};

  char constructed_url[URL_MAX_LENGTH + ENDPOINT_MAX_LENGTH];
  strncat(constructed_url, url, URL_MAX_LENGTH);
  strncat(constructed_url, endpoint, ENDPOINT_MAX_LENGTH);

  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
  curl_easy_setopt(curl, CURLOPT_URL, constructed_url);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  curl_easy_perform(curl);

  long res_code;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res_code);

  cJSON *json = cJSON_ParseWithLength(chunk.response, chunk.size);

  return json;
}

void network_cleanup()
{
  curl_easy_cleanup(curl);
}
