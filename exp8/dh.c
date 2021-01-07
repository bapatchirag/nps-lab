#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define P 23
#define G 5

long int getkey(long, long);

int main() {
    long int a_private, a_public, b_private, b_public;

    // Generate random private keys for A and B
    srandom(time(0));
    a_private = random();
    b_private = random();
    printf("Private keys: \n");
    printf("A: %ld\n", a_private);
    printf("B: %ld\n\n", b_private);

    // Generate public keys for A and B
    a_public = getkey(G, a_private);
    b_public = getkey(G, b_private);
    printf("Public keys: \n");
    printf("A: %ld\n", a_public);
    printf("B: %ld\n\n", b_public);

    // Generate the shared secret key for both A and B
    long int a_key = getkey(b_public, a_private);
    long int b_key = getkey(a_public, b_private);
    printf("Shared key: \n");
    printf("A: %ld\n", a_key);
    printf("B: %ld\n\n", b_key);

    return 0;
}

/**
 * Generates key = (base^exp) (mod P)
 * @param base Base with which key is to be generated
 * @param exp Power to which base is to be raised
 * @returns Key generated as described
 */
long int getkey(long int base, long int exp) {
    long int rem;
	long int key = 1;

	while(exp > 0)
	{
		rem = exp % 2;

		// fast exponention
		if (rem == 1) {
			key = (key * base) % P;
        }
		base = (base * base) % P;

		exp = exp / 2;
	}

	return key;
}