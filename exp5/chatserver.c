#include "../socketheaders.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void chat_with_client(int);

int main() {
    struct sockaddr_in server;
    int listenfd, connfd;

    // Create socket
    if((listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("Server socket creation failed\n");
        exit(0);
    }

    // Bind socket to PORTNUM
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORTNUM);

    if(bind(listenfd, (struct sockaddr*)&server, sizeof(server))) {
        printf("Server socket bind failed\n");
        exit(0);
    }

    // Begin listening on socket
    if(listen(listenfd, MAXQUEUE)) {
        printf("Server socket listen failed\n");
        exit(0);
    }

    // Start accepting client requests
    while(1) {
        int server_len = sizeof(server);
        if((connfd = accept(listenfd, (struct sockaddr*)&server, &server_len)) == -1) {
            printf("Client request couldn't be accepted\n");
            exit(0);
        }

        // Set up iterative server
        // Close listening socket
        if(close(listenfd)) {
            printf("Listening socket close failed\n");
            exit(0);
        }

        // Chat with client
        chat_with_client(connfd);

        // Close connected socket
        if(close(connfd)) {
            printf("Connected socket close failed\n");
            exit(0);
        }
    }

    return 0;
}

void chat_with_client(int socket) {
    int bytes_recv;
    char *buffer_recv = malloc(1024);
    char *buffer_sent = malloc(1024);

    do {
        while((bytes_recv = recv(socket, buffer_recv, 1024, 0)) > 0) {
            memset(buffer_sent, 0, 1024);
            printf("Client says: %s\n", buffer_recv);
            printf("Reply: ");
            fgets(buffer_sent, 1024, stdin);
            send(socket, buffer_sent, strlen(buffer_sent), 0);
            memset(buffer_recv, 0, 1024);
        }
    } while(bytes_recv < 0);

    // Free memory
    free(buffer_recv);
    free(buffer_sent);
}