#include "woody.h"
#include "libft.h"

char *get_file_type(Elf64_Half type)
{
	switch (type)
	{
	case 1:
		return "REL (Relocatable file)";
	case 2:
		return "EXEC (Executable file)";
	case 3:
		return "DYN (Position-Independent Executable file)";
	case 4:
		return "CORE (Core Dump)";
	default:
		return "unindentifiable";
	}
}

char *get_file_class(unsigned char ident[EI_NIDENT])
{
	switch (ident[EI_CLASS])
	{
	case 1:
		return "32-bit";
	case 2:
		return "64-bit";
	default:
		return "invalid";
	}
}
void print_ehdr(Elf64_Ehdr *upckbin)
{
	printf("\n%s--- ELF Header ---%s\n", INFO, RESET);

	printf("Elf file type is %-45s\n", get_file_type(upckbin->e_type));
	printf("Elf file class is %s\n", get_file_class(upckbin->e_ident));
	printf("Entry point 0x%lx\n", upckbin->e_entry);
	printf("There are %u program headers, starting at offset %lu\n", upckbin->e_phnum, upckbin->e_phoff);
}

void print_ascii(char *str)
{
	for (size_t i = 0; i < ft_strlen(str); i++)
	{
		printf("%d ", str[i]);
	}
	printf("\n");

	for (size_t i = 0; i < ft_strlen(str); i++)
	{
		printf("|%c| ", str[i]);
	}

	printf("\n");
}

void print_bin_context(s_bin_ctx ctx)
{
	printf("\n%s--- Printing ELF Header Info ---\n%s", INFO, RESET);
	printf("%s%-25s%s	0x%lX\n", INFO, "ctx.original_entrypoint", RESET, ctx.original_entrypoint);
	printf("%s%-25s%s	%d\n", INFO, "ctx.program_hdr_count", RESET, ctx.program_hdr_count);
	printf("%s%-25s%s	%16ld\n", INFO, "ctx.program_hdr_offset", RESET, ctx.program_hdr_offset);
	printf("%s%-25s%s	%ld\n", INFO, "ctx.text_offset", RESET, ctx.text_offset);
	printf("%s%-25s%s	%ld\n", INFO, "ctx.text_size", RESET, ctx.text_size);
	printf("%s%-25s%s	%ld\n", INFO, "ctx.text_vaddress", RESET, ctx.text_vaddress);
	printf("\n");
}

#include <stdio.h>
#include <elf.h>

// Turn p_type's numeric value into a readable name
static const char *phdr_type_str(Elf64_Word type)
{
	switch (type)
	{
	case PT_NULL:
		return "PT_NULL";
	case PT_LOAD:
		return "PT_LOAD";
	case PT_DYNAMIC:
		return "PT_DYNAMIC";
	case PT_INTERP:
		return "PT_INTERP";
	case PT_NOTE:
		return "PT_NOTE";
	case PT_SHLIB:
		return "PT_SHLIB";
	case PT_PHDR:
		return "PT_PHDR";
	case PT_TLS:
		return "PT_TLS";
	case PT_GNU_EH_FRAME:
		return "PT_GNU_EH_FRAME";
	case PT_GNU_STACK:
		return "PT_GNU_STACK";
	case PT_GNU_RELRO:
		return "PT_GNU_RELRO";
	default:
		return "UNKNOWN";
	}
}

void print_phdr(Elf64_Phdr phdr, int i)
{
	char flags[4];

	flags[0] = phdr.p_flags & PF_R ? 'R' : ' ';
	flags[1] = phdr.p_flags & PF_W ? 'W' : ' ';
	flags[2] = phdr.p_flags & PF_X ? 'X' : ' ';
	flags[4] = 0;

	printf("Header [%02d]	%-3s	%-15s\n", i, flags, phdr_type_str(phdr.p_type));
}

void print_text_data(unsigned char *txt, Elf64_Word size, Elf64_Off offset)
{
	char ascii[17] = {0};

	printf("\n%s--- Printing executable text segment of size %u at offset %ld---%s\n ", INFO, size, offset, RESET);
	printf("\n");

	for (Elf64_Xword i = 0; i < size; i++)
	{
		if ((i) % 16 == 0)
			printf("%08lX: ", offset + i);
		printf("%02x", txt[i]);
		if (i % 2 == 0) printf(" ");
		ascii[i % 16] = txt[i] >= ' '&&  txt[i] < 127 ?  txt[i] : '.';
		if (i == (size - 1) && (i + 1) % 16 != 0)
			while ((i + 1) % 16 != 0)
			{
				printf("   ");
				i++;
			}
		if ((i + 1) % 16 == 0) {
			printf("	%s\n", ascii);
			ft_memset(ascii, 0, 17);
		}
	}
	printf("\n");
}

void print_S(unsigned char*  S) {
	int i = 0;

	while (i < 256) {
		printf("%d: %d | ", i, S[i]);
		i++;
	}
}