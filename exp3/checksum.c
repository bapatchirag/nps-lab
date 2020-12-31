#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_TO_BSTRING(n,out) \
({\
*(out+0) = (n & 0x80) ? '1' : '0'; \
*(out+1) = (n & 0x40) ? '1' : '0'; \
*(out+2) = (n & 0x20) ? '1' : '0'; \
*(out+3) = (n & 0x10) ? '1' : '0'; \
*(out+4) = (n & 0x08) ? '1' : '0'; \
*(out+5) = (n & 0x04) ? '1' : '0'; \
*(out+6) = (n & 0x02) ? '1' : '0'; \
*(out+7) = (n & 0x01) ? '1' : '0'; \
*(out+8) = 0; \
out; \
})

typedef enum statuscode {
    ALL_OK,
    BAD_MESSAGE,
    BAD_RECV,
    FREE_ERROR,
    INVALID_MSG=-1
} scode;

int checkbinstring(char*, int);
int getsum(char*, int);
int checkvalid(char*, int);
int freemem(scode, char**);

int main() {
    // Stores all strings created
    char** strings = malloc(4096);

    // Number of strings created
    int string_count = 0;

    // Get original message
    char* message = malloc(1024);
    *(strings + string_count++) = message;
    memset(message, 0, 1024);
    printf("Enter message to be sent: ");
    scanf("%[^\n]%*c", message);

    // Validate message
    int message_length = strlen(message);
    if(checkbinstring(message, message_length)) {
        freemem(BAD_MESSAGE, strings);
        exit(0);
    }

    // Get checksum and append to message
    char* checksum = malloc(10);
    *(strings + string_count++) = checksum;
    memset(checksum, 0, 10);
    INT_TO_BSTRING(getsum(message, message_length), checksum);
    strcat(message, " ");
    strcat(message, checksum);
    printf("Message with checksum %s: %s\n", checksum, message);

    // Get received message
    char* recv = malloc(1024);
    *(strings + string_count++) = recv;
    memset(recv, 0, 1024);
    printf("Enter received message: ");
    scanf("%[^\n]%*c", recv);
    
    // Validate received message
    int recv_length = strlen(recv);
    if(recv_length != message_length + 9 || checkbinstring(recv, recv_length)) {
        freemem(BAD_RECV, strings);
        exit(0);
    }

    // Check if checksum is valid or not
    if(checkvalid(recv, recv_length)) {
        printf("Error detected\n");
    }
    else {
        printf("No error detected\n");
    }

    freemem(ALL_OK, strings);

    return 0;
}

/**
 * Checks if inputted string is a binary string or not
 * @param string Input string to be checked
 * @param len Length of input string
 * @returns 0 if binary string, -1 if not
 */
int checkbinstring(char* string, int len) {
    for(int i = 0; i < len; i++) {
        char c = *(string + i);
        if(c != 48 && c != 49 && c != 32) {
            return -1;
        }
    }

    return 0;
}

/**
 * Gets sum of all space-separated bytes of given message
 * @param message Space separated bytes
 * @param len Length of message
 * @returns 8 bit inverted sum of all bytes in message, added using 1s complement method
 */
int getsum(char* message, int len) {
    int byte_count = (len / 9) + 1;
    char* next_byte;

    if(byte_count == 1) {
        int sum = strtol(message, NULL, 2);
        sum -= (sum > 255 ? 255 : 0);
        return (~sum & 255);
    }

    int sum = (int)strtol(message, &next_byte, 2);
    for(int i = 1; i < byte_count - 1; i++) {
        sum += (int)strtol(next_byte, &next_byte, 2);
        sum -= (sum > 255 ? 255 : 0);
    }
    sum += (int)strtol(next_byte, NULL, 2);
    sum -= (sum > 255 ? 255 : 0);

    return (~sum & 255);
}

/**
 * Checks if the received message has a valid checksum
 * @param recv Received message
 * @param recv_len Length of received message
 * @returns 0 if valid message, INVALID_MSG if invalid
 */
int checkvalid(char* recv, int recv_len) {
    // Check if sum is 0 or not
    return (getsum(recv, recv_len) ? INVALID_MSG : 0);
}

/**
 * Frees up memory according to status code given
 * @param status Status code
 * @param strings Strings to be freed
 * @returns 0 if good status code, FREE_ERROR if not
 */
int freemem(scode status, char** strings) {
    if(status == ALL_OK) {
        free(*(strings+0));
        free(*(strings+1));
        free(*(strings+2));
        free(strings);
    }
    else if(status == BAD_MESSAGE) {
        printf("Bad message string\n");
        free(*(strings + 0));
        free(strings);
    }
    else if(status == BAD_RECV) {
        printf("Bad recv string\n");
        free(*(strings+0));
        free(*(strings+1));
        free(*(strings+2));
        free(strings);
    }
    
    else {
        return FREE_ERROR;
    }

    return 0;
}