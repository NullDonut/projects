#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>

int main(void) {
  char url[] =
      "https://discord.com/api/webhooks/1536183147677622323/"
      "sYQyknKdgUVmYTyPbhtVJAvg5jQmJHrPSYtUaGhk6Rj4yVOvwoxJvqzbqbuq9ekn6gJ"
      "T";

  struct curl_slist* headers = NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");

  const char* json_payload = "{\"content\":\"hello from C\"}";

  CURL* curl = curl_easy_init();
  if (curl) {
    CURLcode result;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(res));
  }
  curl_slist_free_all(headers);

  curl_easy_cleanup(curl);

  return 0;
}
