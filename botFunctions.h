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
    headers = curl_slist_append(headers, "User Agent: DiscordBot(curl, C)");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, "https://discord.com/api/v10/users/@me");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

}

