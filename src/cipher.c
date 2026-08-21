#include "woody.h"

/*
	create_cipher_key generates a random key of 16 bytes by reading /dev/urandom
*/
bool create_cipher_key(unsigned char*  key) {
	int urandomFd, bytesRead = 0;

	urandomFd = open("/dev/urandom", O_RDONLY);
	if (urandomFd == -1)
		return _perror("Unable to open urandom");
	bytesRead = read(urandomFd, key, 16);
	if (bytesRead == -1 || bytesRead != 16) {
		if (close(urandomFd) == -1)
			return _perror("Unable to close urandomFd");
		return _perror("Unable to read urandom");
	}
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
void swap_S_values(unsigned char*  S, int i, int j) {
	char tmp = S[i];

	S[i] = S[j];
	S[j] = tmp;
}

/*
	Key-scheduling algorithm 
*/
void ksa(unsigned char* S, unsigned char* key) {
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
void prga(unsigned char*  S, unsigned char* text, int text_size) {
	int i = 0;
	int j = 0;
	int t = 0;

	for (int idx = 0; idx < text_size; idx++) {
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		swap_S_values(S, i, j);
		t = (S[i] + S[j]) % 256;
		text[idx] ^= S[t]; //XOR
	}
}

void encrypt_text(unsigned char* key, unsigned char* text, int text_size) { //TODO: change int to elfuint64
	unsigned char  S[256];

	ft_bzero(S, 256);
	ksa(S, key);
	prga(S, text, text_size);
}


/*
STEPS TO ENCRYPT USING RC4:
- create a random key
- create a keystream using key (KSA + PRGA)
- combine keystream with .text using XOR to get ciphertext

encrypt | decrypt :
Plaintext + Keystream -> XOR -> Ciphertext | Ciphertext + Keystream -> XOR -> Plaintext
*/