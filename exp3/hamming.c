#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkbinstring(char*, int);
char* gethammingcode(char*, char*);
int checkhammingcode(char*);

int main() {
    // Get message to be transmitted

    char* nibble = malloc(5);
    printf("Enter 4 bit bitstring to be sent: ");
    scanf("%s", nibble);
    if(strlen(nibble) != 4 || checkbinstring(nibble, 4)) {
        printf("Bad input\n");
        free(nibble);
        exit(0);
    }

    // Encode using the Hamming code

    char* encoded = malloc(8);
    *(encoded + 7) = 0;
    gethammingcode(encoded, nibble);
    printf("Encoded string is: %s\n", encoded);

    // Get received message

    char* recv = malloc(8);
    printf("Enter 7 bit received bitstring: ");
    scanf("%s", recv);
    if(strlen(recv) != 7 || checkbinstring(recv, 7)) {
        printf("Bad input\n");
        free(nibble);
        free(encoded);
        free(recv);
        exit(0);
    }

    // Check if error exists or not

    int hamming_error = checkhammingcode(recv);
    if(hamming_error == 0) {
        printf("No data corruption\n");
    }
    else {
        printf("Error at position: %d\n", hamming_error);
    }

    // Free allocated memory

    free(nibble);
    free(encoded);
    free(recv);

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
        if(c != 48 && c != 49) {
            return -1;
        }
    }

    return 0;
}

/**
 * Gets the Hamming encoded bitstring
 * @param dest Final Hamming encoded bitstring
 * @param src Bitstring to be encoded
 * @returns Pointer to dest
 */
char* gethammingcode(char* dest, char* src) {
    // Initialization of encoded string
    *(dest + 0) = *(src + 0);
    *(dest + 1) = *(src + 1);
    *(dest + 2) = *(src + 2);
    *(dest + 4) = *(src + 3);

    // Setting of parity bits
    *(dest + 6) = ((*(dest + 4) - 48) ^ (*(dest + 2) - 48) ^ (*(dest + 0) - 48)) + 48;
    *(dest + 5) = ((*(dest + 4) - 48) ^ (*(dest + 1) - 48) ^ (*(dest + 0) - 48)) + 48;
    *(dest + 3) = ((*(dest + 2) - 48) ^ (*(dest + 1) - 48) ^ (*(dest + 0) - 48)) + 48;

    return dest;
}

/**
 * Checks for error in Hamming code
 * @param code Hamming encoded bitstring to be checked
 * @returns (1-indexed) Position of error in code, 0 if no error
 */
int checkhammingcode(char* code) {
    // Check even parity 
    int p2 = ((*(code + 3) - 48) ^ (*(code + 2) - 48) ^ (*(code + 1) - 48) ^ (*(code + 0) - 48));
    int p1 = ((*(code + 5) - 48) ^ (*(code + 4) - 48) ^ (*(code + 1) - 48) ^ (*(code + 0) - 48));
    int p0 = ((*(code + 6) - 48) ^ (*(code + 4) - 48) ^ (*(code + 2) - 48) ^ (*(code + 0) - 48));

    return ((p2 * 4) + (p1 * 2) + p0);
} 