#include "../socketheaders.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

void to_echo(int, struct sockaddr*, int);

int main(int argc, char* argv[]) {
    struct sockaddr_in server;
    int sockfd;

    // Create socket
    if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) > 0) {
        printf("Client socket successfully created\n");
    }
    else {
        printf("Client socket creation failed\n");
        exit(0);
    }
    struct timeval test;
    int ans;
    int ans_len = sizeof(ans);
    if(!getsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &ans, &ans_len)) {
        printf("%d\n", ans);
    }
    if(ans == 0) {
        ans = 5;
        test.tv_sec = ans;
        if(!setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &test, sizeof(test))) {
            if(!getsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &ans, &ans_len)) {
                printf("%d\n", ans);
            }
        }
        else {
            printf("Request timed out\n");
        }
    }

    // Set server details: Port 15000, family AF_INET
    server.sin_family = AF_INET;
    server.sin_port = htons(15000);
    inet_pton(AF_INET, argv[1], &server.sin_addr);

    // Make requests to server
    printf("--------------------------------------\n");
    to_echo(sockfd, (struct sockaddr*)&server, sizeof(server));
    
    // Close socket file descriptor
    close(sockfd);

    return 0;
}

void to_echo(int sockfd, struct sockaddr *server, int server_len) {
    char *buffer = malloc(1024);
    int bytes_to_echo;

    // Prompt
    printf("Enter data to be echoed: ");

    while(fgets(buffer, 1024, stdin) != NULL) {
        int addrlen = server_len;
        if(sendto(sockfd, buffer, sizeof(buffer), 0, server, server_len) != -1) {
            if((bytes_to_echo = recvfrom(sockfd, buffer, sizeof(buffer), 0, server, &addrlen)) != -1) {
                fputs(buffer, stdout);
            }
            else {
                printf("Couldn't receive data from server\n");
                free(buffer);
                return;
            }
        }
        else {
            printf("Couldn't send data to server\n");
            free(buffer);
            return;
        }
    }
}