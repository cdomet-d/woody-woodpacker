#include "woody.h"

/*
	create_cipher_key generates a random key of 16 bytes by reading /dev/urandom
*/
bool create_cipher_key(unsigned char*  key) {
	int urandomFd = 0;
	int bytesRead = 0;

	urandomFd = open("/dev/urandom", O_RDONLY);
	if (urandomFd == -1)
		return _perror("Unable to open urandom");
	bytesRead = read(urandomFd, key, 16);
	if (bytesRead == -1 || bytesRead != 16)
		return _perror("Unable to read urandom");
	if (close(urandomFd) == -1)
		return _perror("Unable to close urandomFd");
	return true;
}

/*
	init_array_S initializes array S's 256 bytes from 0 to 255
*/
void init_array_S(unsigned char* S) {
	for (int i = 0; i < 256; i++)
		S[i] = i;
}

/*
	swap_S_values swaps two values of a given array
*/
void swap_S_values(unsigned char*  S, int i , int j) {
	char tmp = S[i];

	S[i] = S[j];
	S[j] = tmp;
}

/*
	Key-scheduling algorithm 
*/
void ksa(unsigned char*  S, unsigned char*  key) {
	int i = 0;
	int j = 0;

	init_array_S(S);
	while (i < 256) {
		j = (j + S[i] + key[i % 16]) % 256;
		swap_S_values(S, i, j);
		i++;
	}
}

/*
	Pseudo-random generation algorithm
*/
void prga(unsigned char*  S, unsigned char* text) {
	(void)S;
	(void)text;
	/*
	i := 0
	j := 0
	while GeneratingOutput:
		i := (i + 1) mod 256
		j := (j + S[i]) mod 256
		swap values of S[i] and S[j]
		t := (S[i] + S[j]) mod 256
		K := S[t]
		output K
	endwhile
	*/
}

bool encrypt_text(unsigned char* text) {
	unsigned char  key[16];
	unsigned char  S[256];

	ft_bzero(key, 16);
	ft_bzero(S, 256);
	if (!create_cipher_key(key))
		return false;
	// _psuccess((char *)key);
	ksa(S, key);
	print_S(S);
	prga(S, text);
	return true;
}

/*
STEPS TO ENCRYPT USING RC4:
- create a random key
- create a keystream using key (KSA + PRGA)
- combine keystream with .text using XOR to get ciphertext

encrypt | decrypt :
Plaintext + Keystream -> XOR -> Ciphertext | Ciphertext + Keystream -> XOR -> Plaintext
*/