#include "woody.h"

bool create_cipher_key(unsigned char*  key) {
    int urandomFd = 0;

    urandomFd = open("/dev/urandom", O_RDONLY);
    if (urandomFd == -1)
        return _perror("Unable to open urandom");
    if (read(urandomFd, key, 16) == -1)
        return _perror("Unable to read urandom");
    if (close(urandomFd) == -1)
        return _perror("Unable to close urandomFd");
    return true;
}

bool encrypt_text(unsigned char* text) {
    unsigned char  key[16];//TODO: ft_bzero
    // unsigned char*  S = NULL;

    (void)text;
    if (!create_cipher_key(key))
        return false;
    // _psuccess((char *)key);
    return true;
}

/*
STEPS TO ENCRYPT USING RC4:
- create a random key
- create a keystream using key
- combine keystream with .text using XOR to get ciphertext

encrypt | decrypt :
Plaintext + Keystream -> XOR -> Ciphertext | Ciphertext + Keystream -> XOR -> Plaintext
*/