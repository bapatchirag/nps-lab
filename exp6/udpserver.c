#include "../socketheaders.h"
#include <stdio.h>
#include <stdlib.h>

void echo_to_client(int, struct sockaddr*, int);

int main() {
    struct sockaddr_in server, client;
    int sockfd;

    // Socket created
    if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != -1) {
        printf("Server socket created\n");
    }
    else {
        printf("Server socket creation failed\n");
        exit(0);
    }

    // Bind to port 15000
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(15000);
    if(!bind(sockfd, (struct sockaddr*)&server, sizeof(server))) {
        printf("Bind to port 15000 successful\n");
    }
    else {
        printf("Bind to port 15000 failed\n");
        exit(0);
    }

    // Start accepting client requests
    echo_to_client(sockfd, (struct sockaddr*)&client, sizeof(client));

    return 0;
}

void echo_to_client(int sockfd, struct sockaddr *client, int cli_len) {
    char *buffer = malloc(1024);
    int bytes_recv;

    while(1) {
        int addrlen = cli_len;
        if((bytes_recv = recvfrom(sockfd, buffer, 1024, 0, client, &addrlen)) > 0) {
            printf("Data received. Echoing back...\n");
            if(sendto(sockfd, buffer, bytes_recv, 0, client, addrlen) != -1) {
                printf("Data echoed\n");
            }
            else {
                printf("Data failed to be echoed\n");
                free(buffer);
                return;
            }
        }
        else {
            printf("Data failed to be received\n");
            free(buffer);
            return;
        }        
    }
}