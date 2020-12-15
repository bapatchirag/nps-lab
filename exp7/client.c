#include "../socketheaders.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void to_exec(int);

int main(int argc, char* argv[]) {
    struct sockaddr_in client;
    int socket_created;

    // Check if address provided or not
    if(argc == 1) {
        printf("Address missing\n");
        exit(0);
    }

    // Create client socket
    if((socket_created = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) != -1) {
        printf("Client socket created successfully\n");
    }
    else {
        printf("Client socket failed to be created\n");
        exit(0);
    }

    // Set port 15000 to connect to
    client.sin_family = AF_INET;
    client.sin_port = htons(15000);
    inet_pton(AF_INET, argv[1], &client.sin_addr);

    // Connect to server
    if(!connect(socket_created, (struct sockaddr*)&client, sizeof(client))) {
        printf("Client connected to server\n");
    }
    else {
        printf("Couldn't connect to server\n");
        exit(0);
    }

    // Send data to be echoed
    printf("-------------------------------------------------\n");
    to_exec(socket_created);
    
    if(!close(socket_created)) {
        printf("Connection closed successfully\n");
    }
    else {
        printf("Connection couldn't be closed\n");
        exit(0);
    }

    return 0;
}

void to_exec(int socket) {
    int bytes_recv, to_cont;
    char *buffer = malloc(1024);

    // Prompt
    while(1) {
        printf("Enter command to be executed: ");
        fgets(buffer, 1024, stdin);

        // Check if its time to stop execution
        if(!strcmp(buffer, "end")) {
            free(buffer);
            return;
        }

        // Send command
        send(socket, buffer, sizeof(buffer), 0);
        
        // Wait for server to execute
        sleep(1);
    }
}