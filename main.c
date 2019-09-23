#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "server.h"
#include "client.h"

#define SUCCESS 0
#define ERROR -1

int main(int argc, char* argv[]) {
    if (argc == 3 && strncmp(argv[1], "server", 6) == 0) {
        printf("Creando server en puerto %s\n", argv[2]);
        server_t server;
        server_open(&server, argv[2]);
        server_run(&server);
        server_close(&server);
        return SUCCESS;
    } else if (argc == 4 && strncmp(argv[1], "client", 6) == 0) {
        printf("Conectando cliente a la ip %s en puerto %s\n",
                argv[2], argv[3]);
        client_t client;
        client_open(&client, argv[2], argv[3]);
        client_handle(&client);
        client_close(&client);
        return SUCCESS;
    } else {
        printf("Modo no soportado, ");
        printf("el primer parámetro debe ser server o client\n");
        return ERROR;
    }
}

