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
void init_array_S(unsigned char*  S) {
	int i = 0;

	while (i < 256) {
		S[i] = i;
		i++;
	}
}

bool encrypt_text(unsigned char* text) {
	unsigned char  key[16];//TODO: ft_bzero
	unsigned char  S[256];//TODO: ft_bzero

	(void)text;
	if (!create_cipher_key(key))
		return false;
	// _psuccess((char *)key);
	init_array_S(S);
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