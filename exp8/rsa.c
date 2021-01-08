#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define P 51637
#define Q 36887
#define N ((P)*(Q))
#define PHI (((P)-1)*((Q)-1))

int gcd(int, int, int*, int*);
long int getkeys(int*);
long int fastmod(long, long);

int main() {
    unsigned int encrypt_key, decrypt_key;
    encrypt_key = getkeys(&decrypt_key);

    printf("Encrypt key: %d\n", encrypt_key);
    printf("Decrypt key: %d\n", decrypt_key);
    
    long int message;
    printf("\nEnter decimal message to be encrypted: ");
    scanf("%ld", &message);

    long int ciphertext = fastmod(message, encrypt_key);
    printf("\nEncrypted message: %ld\n", ciphertext);

    long int plaintext = fastmod(ciphertext, decrypt_key);
    printf("\nDecrypting ciphertext, plaintext: %ld\n", plaintext);

    return 0;
}

/**
 * Calculate GCD of 2 numbers and calculates modular inverses of each w.r.t each other
 * @param n1 First number
 * @param n2 Second number
 * @param inv_n1 Pointer to inverse of first number
 * @param inv_n2 Pointer to inverse of second number
 * @returns GCD of n1 and n2
 */
int gcd(int n1, int n2, int* inv_n1, int* inv_n2) 
{ 
    if (n1 == 0)  
    { 
        *inv_n1 = 0, *inv_n2 = 1; 
        return n2; 
    } 
  
    int temp_n1, temp_n2;
    int hcf = gcd(n2 % n1, n1, &temp_n1, &temp_n2); 

    *inv_n1 = temp_n2 - (n2 / n1) * temp_n1; 
    *inv_n2 = temp_n1; 
  
    return hcf; 
} 

/**
 * Generate encrypt and decrypt keys
 * @param d_key Pointer to decrypt key generated
 * @returns Encrypt key generated
 */
long int getkeys(int* d_key) {
    // Seed and get random number as encrypt key
    srandom(time(0));
    int e_key = random(), is_prime;
    int* useless;

    // Generate encrypt key and corresponding decrypt key
    while((is_prime = gcd(e_key, PHI, d_key, useless)) > 1) {
        e_key /= is_prime;
    }

    // Shift decrypt key to be in ring {0, ..., PHI-1}
    *d_key = ((*d_key % PHI) + PHI) % PHI;
    return e_key;
}

/**
 * Does fast exponentiation to get (message ^ key) mod N
 * @param message Plaintext/Ciphertext to be encrypted/decrypted
 * @param key Encrypt/decrypt key
 * @returns Ciphertext/Plaintext generated
 */
long int fastmod(long int message, long int key) {
    long int rem;
	long int result = 1;

	while(key > 0)
	{
		rem = key % 2;

		// fast exponention
		if (rem == 1) {
			result = (result * message) % N;
        }
		message = (message * message) % N;

		key = key / 2;
	}

	return result;
}