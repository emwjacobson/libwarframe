#include "network.h"
#include "warframe.h"
#include "debug.h"
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

  unsigned char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if (ptr == NULL) {
    PRINT_WARN("Out of memory during request\n");
    return 0; // Out of memory
  }

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
  if (curl == NULL) {
    PRINT_ERROR("Could not initialize CURL\n");
    return false;
  }

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
  
  // Should POST requests be converted to GET on redirect?
  // curl_easy_setopt(curl, CURLOPT_POSTREDIR, ???)

  network_initialized = true;
  references = 1;

  return true;
}

/**
 * @brief Makes a GET request to `endpoint`
 * 
 * @param url The URL to make a request to
 * @param endpoint The endpoint to make a request to
 * @return curl_data* The raw `curl_data`. User is responsible for deallocating chunk.response and chunk with `free()`
 */
curl_data *make_GET_Raw(char *url, char *endpoint) {
  if (!network_initialized || curl == NULL)
    return NULL;

  curl_data *chunk = (curl_data *)malloc(sizeof(curl_data));
  if (chunk == NULL) return NULL;
  memset(chunk, 0, sizeof(curl_data));

  char constructed_url[URL_MAX_LENGTH + ENDPOINT_MAX_LENGTH];
  memset(constructed_url, 0, URL_MAX_LENGTH + ENDPOINT_MAX_LENGTH);
  strncat(constructed_url, url, URL_MAX_LENGTH);
  char *enc_endpoint = curl_easy_escape(curl, endpoint, strlen(endpoint));
  strncat(constructed_url, enc_endpoint, ENDPOINT_MAX_LENGTH);
  curl_free(enc_endpoint);

  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
  curl_easy_setopt(curl, CURLOPT_URL, constructed_url);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);

  PRINT_DEBUG("Making request to: %s\n", constructed_url);
  CURLcode res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
    PRINT_WARN("Error making request: %i\n", res);
    free(chunk);
    return NULL;
  }

  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(chunk->response_code));

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
    PRINT_WARN("Problem making RAW request\n");
    return NULL;
  }
  if (chunk->response_code != 200) {
    PRINT_DEBUG("Got bad JSON response code %li\n", chunk->response_code);
    free(chunk->response);
    free(chunk);
    return NULL;
  }

  cJSON *json = cJSON_ParseWithLength((char *)chunk->response, chunk->size);

  if (json == NULL) {
    const char *err = cJSON_GetErrorPtr();
    if (err != NULL)
      PRINT_DEBUG("Error parsing JSON before: %s\n", err);
  }

  free(chunk->response);
  free(chunk);

  return json;
}

void network_cleanup()
{
  references--;
  if (references == 0)
    curl_easy_cleanup(curl);
}
