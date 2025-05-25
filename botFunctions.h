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

#define TOKEN "Token here"

void call_discord_api() {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize CURL\n");
        return;
    }

    struct curl_slist *headers = NULL;
    char authHeader[256];
    snprintf(authHeader, sizeof(authHeader), "Authorization: Bot %s", TOKEN);

    //build out header request
    headers = curl_slist_append(headers, authHeader);
    headers = curl_slist_append(headers, "User-Agent: DiscordBot(curl, C)");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, "https://discord.com/api/v10/users/@me");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

}

static int callback_gateway(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    switch (reason) {
	case LWS_CALLBACK_CLIENT_ESTABLISHED:
	    printf("Websocket connect\n");
	    
	    json_t *identify = json_pack("{s:i, s:o}",
		    "op", 2,
		    "d", json_pack("{s:s, s:i, s:o}",
			"token BOT", TOKEN,
			"intents", 513,
			"properties", json_pack("{s:s, s:s, s:s}",
			    "$os", "linux",
			    "$browser", "c-client",
			    "$device", "c-client"

			    )
			)


		    );

	    char *payload = json_dumps(identify, 0);
	    size_t len = strlen(payload);
	    unsigned char *buf = malloc(LWS_PRE + len);
	    memcpy(&buf[LWS_PRE], payload, len);
	    lws_write(wsi, &buf[LWS_PRE], len, LWS_WRITE_TEXT);
	    free(buf);
	    free(payload);
	    json_decref(identify);

	    break;
	case LWS_CALLBACK_CLIENT_RECEIVE:
	    printf("Received: %.*s\n", (int)len, (char *)in);
	    break;
	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
	    printf("Connection error\n");
	    break;
	default:
	    break;
	    
    }
    return 0;
}

int connect_to_discord_gateway() {
    struct lws_context_creation_info info = {0};
    struct lws_client_connect_info ccinfo = {0};

    struct lws_protocols protocols[] = {
	{ "discord-gateway", callback_gateway, 0, 65536 },
	{ NULL, NULL, 0, 0}
    };

    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;

    struct lws_context *context = lws_create_context(&info);

    ccinfo.context = context;
    ccinfo.address = "gateway.discord.gg";
    ccinfo.port = 443;
    ccinfo.path = "encoding=json";
    ccinfo.host = ccinfo.address;
    ccinfo.origin = ccinfo.address;
    ccinfo.ssl_connection = LCCSCF_USE_SSL;
    ccinfo.protocol = "discord-gateway";

    if (!lws_client_connect_via_info(&ccinfo)) {
	fprintf(stderr, "Connection to discord gateway failed \n");
	return 1;
    }

    while (lws_service(context, 1000) >= 0);

    lws_context_destroy(context);
    return 0;
}
