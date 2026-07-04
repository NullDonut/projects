#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>
#include <stdio.h>

int main(void) {
  CURL* curl;
  CURLcode res;
  char* data = "hi,this is a notification";
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://ntfy.sh/shivansh_alerts");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    for (int i = 0; i < 50; i++) {
      res = curl_easy_perform(curl);
    }
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed:  %s\n",
              curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
  }
  return 0;
}
