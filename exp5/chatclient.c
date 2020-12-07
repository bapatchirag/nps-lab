#include "../socketheaders.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void chat_with_server(int);

int main(int argc, char* argv[]) {
    struct sockaddr_in client;
    int socket_created;

    // Missing address
    if(argc == 1) {
        printf("Missing address\n");
        exit(0);
    }

    // Create client socket
    if((socket_created = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("Client socket failed to be created\n");
        exit(0);
    }

    // Set port PORTNUM to connect to
    client.sin_family = AF_INET;
    client.sin_port = htons(PORTNUM);
    inet_pton(AF_INET, argv[1], &client.sin_addr);

    // Connect to server
    if(connect(socket_created, (struct sockaddr*)&client, sizeof(client))) {
        printf("Couldn't connect to server\n");
        exit(0);
    }

    // Chat with server
    printf("-------------------------------------------------\n");
    chat_with_server(socket_created);
    
    if(close(socket_created)) {
        printf("Connection couldn't be closed\n");
        exit(0);
    }
    
    return 0;
}

void chat_with_server(int socket) {
    int bytes_recv, to_cont;
    char *buffer_send = malloc(1024);
    char *buffer_recv = malloc(1024);

    // Prompt
    printf("Tell server: ");

    while(strcmp(fgets(buffer_send, 1024, stdin), "end")) {
        memset(buffer_recv, 0, 1024);
        send(socket, buffer_send, strlen(buffer_send), 0);
        memset(buffer_send, 0, 1024);
        
        // Receive response from server
        if((to_cont = recv(socket, buffer_recv, 1024, 0)) > 0) {
            printf("Server replies: %s\n", buffer_recv);
        }

        // Reprompt
        printf("Tell server: ");
    }

    // Free memory
    free(buffer_recv);
    free(buffer_send);
}