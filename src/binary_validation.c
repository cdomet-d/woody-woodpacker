#include "woody.h"
#include <stdbool.h>
#include "libft.h"

const char *get_file_type(Elf64_Half type)
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

const char *get_file_class(const unsigned char ident[EI_NIDENT])
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

bool is_valid_magic(const unsigned char *ident)
{
	const unsigned char ELF_MAGIC[4] = {0x7F, 'E', 'L', 'F'};
	return ft_memcmp(ident, ELF_MAGIC, 4) == 0;
}

bool is_valid_format(const int ei_class)
{
	return ei_class == ELFCLASS64 ? true : false;
}

bool is_valid_machine(const int e_machine)
{
	return e_machine == EM_X86_64 ? true : false;
}

bool validate_format(Elf64_Ehdr *ehdr, s_bin_ctx *ctx, s_pdhr_info *phdr_info)
{
	if (!is_valid_magic(ehdr->e_ident))
		return _perror("File format not supported");
	if (!is_valid_format(ehdr->e_ident[EI_CLASS]))
		return _perror("File architecture not supported");
	if (!is_valid_machine(ehdr->e_machine))
		return _perror("Machine architecture not supported");
	ctx->program_entrypoint = &(ehdr->e_entry);
	ctx->orignal_entrypoint = ehdr->e_entry;
	phdr_info->phdr_count = ehdr->e_phnum;
	phdr_info->phdr_offset = ehdr->e_phoff;
	print_ehdr(get_file_type(ehdr->e_type), get_file_class(ehdr->e_ident), \
		*(ctx->program_entrypoint), phdr_info);
	return true;
}