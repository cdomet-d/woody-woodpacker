#include "woody.h"
#include <stdbool.h>
#include "libft.h"

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

bool validate_format(Elf64_Ehdr *ehdr, s_bin_ctx *ctx)
{
	if (!is_valid_magic(ehdr->e_ident))
		return _perror("File format not supported");
	if (!is_valid_format(ehdr->e_ident[EI_CLASS]))
		return _perror("File architecture not supported");
	if (!is_valid_machine(ehdr->e_machine))
		return _perror("Machine architecture not supported");
	print_ehdr(ehdr);
	ctx->original_entrypoint = ehdr->e_entry;
	ctx->program_hdr_count = ehdr->e_phnum;
	ctx->program_hdr_offset = ehdr->e_phoff;
	return true;
}