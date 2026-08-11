#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <elf.h>

void print_error(char *error);
void print_elf_struc(Elf64_Ehdr* upckbin);
