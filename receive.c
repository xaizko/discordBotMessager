// discordo_clone.c
#include "botFunctions.h"

int main() {

    printf("Calling Discord API...\n");
    call_discord_api();

    printf("Connecting to Discord Gateway...\n");
    connect_to_discord_gateway();

    return 0;
}

