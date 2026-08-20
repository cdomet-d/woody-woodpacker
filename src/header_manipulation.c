#define OENTRY_OFF 0x46
#define STUB_VADDR_OFF 0x4e
#include "woody.h"
#include "libft.h"

static size_t compute_cave_lenght(Elf64_Xword txt_size)
{
	size_t aligned = (txt_size + x86_64_PAGE_SZ - 1) & ~(x86_64_PAGE_SZ - 1);
	return aligned - txt_size;
}

/* Finds and stores the executable PT_LOAD segment of the binary, which contains the .text section
Return : `true` if ephdr == 1, `false` otherwise
*/
bool find_xphdr(Elf64_Phdr *phdr, const s_pdhr_info *phdr_info, s_bin_ctx *ctx)
{
	int ephdr_count = 0;
	int xphdr_index = 0;

	for (int i = 0; i < phdr_info->phdr_count; i++)
	{
		if (phdr[i].p_flags & PF_X && phdr[i].p_type == PT_LOAD)
		{
			if (ephdr_count == 0)
				xphdr_index = i;
			ephdr_count++;
		}
	}

	if (ephdr_count == 0)
		return _perror("Couldn't find any executable headers");
	if (ephdr_count > 1)
		return _perror("Found more than one executable header. Aborting...");

	ctx->xphdr.txt_offset = phdr[xphdr_index].p_offset;
	ctx->xphdr.txt_size = &(phdr[xphdr_index]).p_filesz;
	ctx->xphdr.mem_size = &(phdr[xphdr_index]).p_memsz;
	ctx->xphdr.txt_vaddress = phdr[xphdr_index].p_vaddr;
	ctx->xphdr.cave_offset = ctx->xphdr.txt_offset + *(ctx->xphdr.txt_size);
	ctx->xphdr.cave_lenght = compute_cave_lenght(*(ctx->xphdr.txt_size));
	return true;
}

bool insert_stub(void *file_map, s_bin_ctx *ctx)
{
	extern unsigned char _binary_stub_bin_start[];
	extern unsigned char _binary_stub_bin_end[];
	Elf64_Xword stub_len = _binary_stub_bin_end - _binary_stub_bin_start;
	Elf64_Addr *o_entry = (Elf64_Addr *)(file_map + ctx->xphdr.cave_offset + OENTRY_OFF);
	Elf64_Addr *stub_vaddr = (Elf64_Addr *)(file_map + ctx->xphdr.cave_offset + STUB_VADDR_OFF);

	if (stub_len > ctx->xphdr.cave_lenght)
		return _perror("Code cave is too short for stub");

	ft_memcpy(file_map + ctx->xphdr.cave_offset, _binary_stub_bin_start, stub_len);
	*o_entry = ctx->original_entrypoint;
	*(ctx->program_entrypoint) = ctx->xphdr.txt_vaddress + *(ctx->xphdr.txt_size);
	*stub_vaddr = *(ctx->program_entrypoint);
	*(ctx->xphdr.txt_size) += stub_len;
	*(ctx->xphdr.mem_size) += stub_len;
	return true;
}
