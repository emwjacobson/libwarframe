#include "network.h"
#include "warframe.h"

#include <stdint.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

static bool network_initialized = false;
static CURL *curl = NULL;
static int references = 0;

// https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
static size_t write_callback(char *data, size_t size, size_t nmemb, void *userdata)
{
  size_t realsize = size * nmemb;
  curl_data *mem = (curl_data *)userdata;

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
  if (network_initialized) {
    references++;
    return true;
  }

  curl = curl_easy_init();
  if (curl == NULL)
    return false;

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

  network_initialized = true;
  references = 1;

  return true;
}

/**
 * @brief Makes a GET request to `endpoint`
 * 
 * @param url The URL to make a request to
 * @param endpoint The endpoint to make a request to
 * @return curl_data* The raw `curl_data`. User is responsible for deallocating with `free()`
 */
curl_data *make_GET_Raw(char *url, char *endpoint) {
  if (!network_initialized || curl == NULL)
    return NULL;

  curl_data *chunk = (curl_data *)malloc(sizeof(curl_data));
  if (chunk == NULL) return NULL;
  memset(chunk, 0, sizeof(curl_data));

  char constructed_url[URL_MAX_LENGTH + ENDPOINT_MAX_LENGTH];
  strncat(constructed_url, url, URL_MAX_LENGTH);
  strncat(constructed_url, endpoint, ENDPOINT_MAX_LENGTH);

  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
  curl_easy_setopt(curl, CURLOPT_URL, constructed_url);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);

  curl_easy_perform(curl);

  return chunk;
}

/**
 * @brief Makes a GET request to `endpoint`
 *
 * @param url The base URL to make a request to
 * @param endpoint The endpoint to make the request to
 * @return cJSON* A JSON object. User is responsible for deallocating with `cJSON_Delete()`
 */
cJSON *make_GET_JSON(char *url, char *endpoint)
{
  if (!network_initialized || curl == NULL)
    return NULL;

  curl_data *chunk = make_GET_Raw(url, endpoint);
  if (chunk == NULL) {
    printf("Problem making request.\n");
    return NULL;
  }

  cJSON *json = cJSON_ParseWithLength(chunk->response, chunk->size);

  free(chunk);

  return json;
}

void network_cleanup()
{
  references--;
  if (references == 0)
    curl_easy_cleanup(curl);
}
