// discordo_clone.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <curl/curl.h>
#include <libwebsockets.h> 
#include <jansson.h>

int main() {



    return 0;
}

void call_discord_api(const char *botToken) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize CURL\n");
        return;
    }

    struct curl_slist *headers = NULL;
    char authHeader[256];
    snprint(authHeader, sizeof(authHeader), "Authorization: Bot %s", botToken);

    //build out header request
    headers = curl_slist_append(headers, authHeader);
    headers = curl_slist_append(headers, "Content-Type: ");
}