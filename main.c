#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "server.h"
#include "client.h"

#define SUCCESS 0
#define ERROR -1

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        if (strncmp(argv[1], "server", 6) == 0) {
            if (argc == 3) {
            server_t server;
            server_open(&server, argv[2]);
            server_run(&server);
            server_close(&server);
            return SUCCESS;
            } else {
                char msgErr[] = "Uso: ./tp server <puerto>\n";
                fprintf(stderr, "%s", msgErr);
                return ERROR;
            }
        } else if (strncmp(argv[1], "client", 6) == 0) {
            if (argc == 4) {
            client_t client;
            client_open(&client, argv[2], argv[3]);
            client_handle(&client);
            client_close(&client);
            return SUCCESS;
            } else {
                char msgErr[] = "Uso: ./tp client <host> <puerto>\n";
                fprintf(stderr, "%s", msgErr);
                return ERROR;
            }
        } else {
            char msgErr[] =
            "Modo no soportado, el primer parámetro debe ser server o client\n";
            fprintf(stderr, "%s", msgErr);
            return ERROR;
        }
    } else {
        char msgErr[] =
            "Modo no soportado, el primer parámetro debe ser server o client\n";
        fprintf(stderr, "%s", msgErr);
        return ERROR;
    }
}

