#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#define GREEN "\033[38;2;80;200;120m"
#define INFO "\033[48;2;228;208;10m"
#define RED "\033[38;2;205;41;73m"
#define RESET "\033[0m"

bool _perror(char *error);
bool _psuccess(char *mess); 
void _plog(char *mess);
void print_elf_struc(Elf64_Ehdr* upckbin);
bool encrypt_text(unsigned char* text);
