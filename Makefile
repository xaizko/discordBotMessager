all:
	gcc -Wall -Wextra -O2 receive.c -o discord_client -lwebsockets -lcurl -ljansson -lssl -lcrypto

