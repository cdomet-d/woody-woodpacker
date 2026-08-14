#include "woody.h"
#include "libft.h"

void print_ehdr(Elf64_Ehdr *upckbin)
{
	printf("--- ELF Header ---\n");

	// e_ident: 16-byte array, not a single value. Bytes 0-3 are the magic
	// number (0x7F 'E' 'L' 'F'), byte 4 = class (32/64-bit), byte 5 = data
	// encoding (endianness), byte 6 = ELF version, byte 7 = OS/ABI, rest padding.
	printf("e_ident      : %c%c%c%c (magic)\n",
		   upckbin->e_ident[EI_MAG0], upckbin->e_ident[EI_MAG1],
		   upckbin->e_ident[EI_MAG2], upckbin->e_ident[EI_MAG3]);

	switch (upckbin->e_ident[EI_CLASS])
	{
	case 0:
		printf("e_type       : Invalid class\n");
		break;
	case 1:
		printf("e_type       : 32-bit objects\n");
		break;
	case 2:
		printf("e_type       : 64-bit objects\n");
		break;
	default:
		break;
	}

	switch (upckbin->e_type)
	{
	case 1:
		printf("e_type       : %d (object file type: relocatable)\n", upckbin->e_type);
		break;
	case 2:
		printf("e_type       : %d (object file type: non-PIE executable)\n", upckbin->e_type);
		break;
	case 3:
		printf("e_type       : %d (object file type: shared object/PIE executable)\n", upckbin->e_type);
		break;
	case 4:
		printf("e_type       : %d (object file type: core dump)\n", upckbin->e_type);
		break;
	default:
		_perror("Could not identify file type");
		break;
	}

	// e_machine: target instruction set architecture (EM_X86_64 = 62)
	printf("e_machine    : %d (target architecture, 62 = x86_64)\n", upckbin->e_machine);

	// e_version: ELF format version, currently always 1 (EV_CURRENT)
	printf("e_version    : %d (ELF format version, always 1)\n", upckbin->e_version);

	// e_entry: virtual address the kernel jumps to when the process starts
	printf("e_entry      : 0x%lx (entry point virtual address)\n", upckbin->e_entry);

	// e_phoff: file offset (in bytes) where the program header table starts
	printf("e_phoff      : %ld (program header table file offset)\n", upckbin->e_phoff);

	// e_shoff: file offset (in bytes) where the section header table starts
	printf("e_shoff      : %ld (section header table file offset)\n", upckbin->e_shoff);

	// e_ehsize: size in bytes of this ELF header itself
	printf("e_ehsize     : %d (ELF header size in bytes)\n", upckbin->e_ehsize);

	// e_phentsize: size in bytes of ONE entry in the program header table
	printf("e_phentsize  : %d (size of one program header entry)\n", upckbin->e_phentsize);

	// e_phnum: number of entries in the program header table
	printf("e_phnum      : %d (number of program header entries)\n", upckbin->e_phnum);

	// e_shentsize: size in bytes of ONE entry in the section header table
	printf("e_shentsize  : %d (size of one section header entry)\n", upckbin->e_shentsize);

	// e_shnum: number of entries in the section header table
	printf("e_shnum      : %d (number of section header entries)\n", upckbin->e_shnum);

	// e_shstrndx: index into the section header table of the section
	// that holds section NAME strings (the ".shstrtab" section)
	printf("e_shstrndx   : %d (index of section name string table)\n", upckbin->e_shstrndx);
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
	printf("%s%-25s%s	%ld\n", INFO, "ctx.original_entrypoint", RESET, ctx.original_entrypoint);
	printf("%s%-25s%s	%d\n", INFO, "ctx.program_hdr_count", RESET, ctx.program_hdr_count);
	printf("%s%-25s%s	%ld\n", INFO, "ctx.program_hdr_offset", RESET, ctx.program_hdr_offset);
	printf("%s%-25s%s	%ld\n", INFO, "ctx.text_offset", RESET, ctx.text_offset);
	printf("%s%-25s%s	%ld\n", INFO, "ctx.text_size", RESET, ctx.text_size);
	printf("%s%-25s%s	%ld\n", INFO, "ctx.text_vaddress", RESET, ctx.text_vaddress);
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
	printf("Header [%02d]	R %-5s | W %-5s | E %-5s | Type %-15s\n", i,
		   phdr.p_flags & PF_R ? "true" : "false",
		   phdr.p_flags & PF_W ? "true" : "false",
		   phdr.p_flags & PF_X ? "true" : "false",
		   phdr_type_str(phdr.p_type));
}