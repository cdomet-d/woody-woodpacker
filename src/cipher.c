#include "woody.h"

void format_key_to_hex(unsigned char *key, unsigned char *hexkey)
{
	const unsigned char hex[KEY_LENGHT] = "0123456789abcdef";
	uint8_t hex_i = 0;

	for (uint8_t i = 0; i < KEY_LENGHT; i++)
	{
		hexkey[hex_i] = hex[(key[i] >> 4 & 0xF)];
		hex_i++;
		hexkey[hex_i] = hex[(key[i] & 0xF)];
		hex_i++;
	}
	hexkey[32] = '\n';
}

/*
	create_cipher_key generates a random key of KEY_LENGHT bytes by reading /dev/urandom
*/
bool create_cipher_key(unsigned char *key)
{
	int urandomFd, bytesRead = 0;

	urandomFd = open("/dev/urandom", O_RDONLY);
	if (urandomFd == -1)
		return _perror("Unable to open urandom");
	bytesRead = read(urandomFd, key, KEY_LENGHT);
	if (bytesRead == -1 || bytesRead != KEY_LENGHT)
	{
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
void init_array_S(unsigned char *S)
{
	for (int i = 0; i < 256; i++)
		S[i] = i;
}

/*
	swap_S_values swaps two values of a given array
*/
void swap_S_values(unsigned char *S, Elf64_Xword i, Elf64_Xword j)
{
	char tmp = S[i];

	S[i] = S[j];
	S[j] = tmp;
}

/*
	Key-scheduling algorithm
*/
void ksa(unsigned char *S, unsigned char *key)
{
	int i = 0;
	int j = 0;

	init_array_S(S);
	while (i < 256)
	{
		j = (j + S[i] + key[i % KEY_LENGHT]) % 256;
		swap_S_values(S, i, j);
		i++;
	}
}

/*
	Pseudo-random generation algorithm
*/
void prga(unsigned char *S, unsigned char *text, Elf64_Xword text_size)
{
	int i = 0;
	int j = 0;
	int t = 0;

	for (Elf64_Xword idx = 0; idx < text_size; idx++)
	{
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		swap_S_values(S, i, j);
		t = (S[i] + S[j]) % 256;
		text[idx] ^= S[t]; // XOR
	}
}

void encrypt_text(unsigned char *key, unsigned char *text, Elf64_Xword text_size)
{
	unsigned char S[256];

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
PlaElf64_Xwordext + Keystream -> XOR -> Ciphertext | Ciphertext + Keystream -> XOR -> PlaElf64_Xwordext
*/