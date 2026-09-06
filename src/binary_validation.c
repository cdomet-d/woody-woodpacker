#include "woody.h"
#include <stdbool.h>
#include "libft.h"

static const char *get_file_type(Elf64_Half type)
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

static const char *get_file_class(const unsigned char ident[EI_NIDENT])
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

static bool is_valid_magic(const unsigned char *ident)
{
	const unsigned char ELF_MAGIC[4] = {0x7F, 'E', 'L', 'F'};
	return ft_memcmp(ident, ELF_MAGIC, 4) == 0;
}

static bool is_valid_format(const int ei_class)
{
	return ei_class == ELFCLASS64 ? true : false;
}

static bool is_valid_machine(const int e_machine)
{
	return e_machine == EM_X86_64 ? true : false;
}

bool validate_preinit_arr(const s_bin_ctx *ctx, Elf64_Dyn *dyn)
{
	Elf64_Addr *init_arr = 0;
	Elf64_Xword init_arr_sz = 0;
	printf("%ld\n", ctx->xphdr.hdr_offset);

	for (size_t i = 0; dyn[i].d_tag; i++) {
		if (dyn[i].d_tag == DT_INIT_ARRAY)
			init_arr = &(dyn[i].d_un.d_ptr);
		else if (dyn[i].d_tag == DT_INIT_ARRAYSZ)
			init_arr_sz = (dyn[i].d_un.d_val) / sizeof(Elf64_Addr);
	}

	if (!init_arr || !init_arr_sz)
		return _perror("Init array address or size not found");
	printf("address: %ld | size: %ld\n", *init_arr, init_arr_sz);

	for (Elf64_Xword i = 0; i < init_arr_sz; i++)
	{
		printf("[%ld]	%ld\n", i, init_arr[i]);
	}
	return true;
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
	ctx->original_entrypoint = ehdr->e_entry;
	phdr_info->phdr_count = ehdr->e_phnum;
	phdr_info->phdr_offset = ehdr->e_phoff;
	phdr_info->phdr_size = ehdr->e_phentsize;
	print_ehdr(get_file_type(ehdr->e_type), get_file_class(ehdr->e_ident), ehdr->e_entry, phdr_info);
	return true;
}
bool is_safe_offset(const s_bin_ctx *ctx)
{
	if (ctx->xphdr.hdr_offset > UINT64_MAX - *(ctx->xphdr.fsizse_addr))
		return false;
	if (ctx->xphdr.v_addr > UINT64_MAX - *(ctx->xphdr.fsizse_addr))
		return false;
	if (ctx->xphdr.cave_offset > UINT64_MAX - ctx->xphdr.cave_lenght)
		return false;
	return true;
}