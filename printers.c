#include "woody.h"

void print_elf_struc(Elf64_Ehdr* upckbin)
{
	printf("e_iden	%d\n", upckbin->e_ident[EI_NIDENT]);
	printf("e_type	%d\n", upckbin->e_type);
	printf("e_machine	%d\n", upckbin->e_machine);
	printf("e_version	%d\n", upckbin->e_version);
	printf("e_entry	%ld\n", upckbin->e_entry);
	printf("e_phoff	%ld\n", upckbin->e_phoff);
	printf("e_shoff	%ld\n", upckbin->e_shoff);
	printf("e_flags	%d\n", upckbin->e_flags);
	printf("e_ehsize	%d\n", upckbin->e_ehsize);
	printf("e_phentsize	%d\n", upckbin->e_phentsize);
	printf("e_phnum	%d\n", upckbin->e_phnum);
	printf("e_shentsize	%d\n", upckbin->e_shentsize);
	printf("e_shnum	%d\n", upckbin->e_shnum);
	printf("e_shstrndx	%d\n", upckbin->e_shstrndx);
}