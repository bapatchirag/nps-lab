#include "../socketheaders.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void echo_on_client(int);

int main() {
    struct sockaddr_in server;
    int listenfd, connfd, pid, waitstat;

    // Create socket
    if((listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) != -1) {
        printf("Server socket creation successful\n");
    }
    else {
        printf("Server socket creation failed\n");
        exit(0);
    }

    // Bind socket to port 15000
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(15000);

    if(!bind(listenfd, (struct sockaddr*)&server, sizeof(server))) {
        printf("Server socket bound to port 15000\n");
    }
    else {
        printf("Server socket bind failed\n");
        exit(0);
    }

    // Begin listening on socket
    if(!listen(listenfd, MAXQUEUE)) {
        printf("Server socket listening\n");
    }
    else {
        printf("Server socket listen failed\n");
    }

    // Start accepting client requests
    while(1) {
        int server_len = sizeof(server);
        if((connfd = accept(listenfd, (struct sockaddr*)&server, &server_len)) != -1) {
            printf("Client %s connected to server\n", inet_ntoa(server.sin_addr));
        }
        else {
            printf("Client request couldn't be accepted\n");
            exit(0);
        }

        // Set up concurrent server
        if((pid = fork()) == 0) {
            // Parent server process closes listenfd
            if(!close(listenfd)) {
                printf("Listening socket closed\n");
            }
            else {
                printf("Listening socket close failed\n");
                exit(0);
            }
            echo_on_client(connfd);
        }
        else {
            if(!close(connfd)) {
                printf("Connected socket closed\n");
            }
            else {
                printf("Connected socket close failed\n");
                exit(0);
            }
        }
    }

    return 0;
}

void echo_on_client(int socket) {
    int bytes_recv;
    char *buffer = malloc(1024);

    do {
        while((bytes_recv = recv(socket, buffer, 1024, 0)) > 0) {
            send(socket, buffer, strlen(buffer), 0);
        }
    } while(bytes_recv < 0);
}