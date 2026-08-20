#define OENTRY_OFF 0x46
#define STUB_VADDR_OFF 0x4e
#include "woody.h"
#include "libft.h"

static size_t compute_cave_lenght(Elf64_Xword *txt_size)
{
	Elf64_Word cpy = *(txt_size);
	size_t aligned = (cpy + x86_64_PAGE_SZ - 1) & ~(x86_64_PAGE_SZ - 1);
	return aligned - cpy;
}

bool get_xphdr(Elf64_Phdr *phdr, const s_pdhr_info *phdr_info, s_bin_ctx *ctx)
{
	int ephdr_count = 0;
	int xphdr_index = 0;

	// printf("\n%s--- Program Headers (%d entries) ---%s\n", INFO, phdr_info->phdr_count, RESET);
	for (int i = 0; i < phdr_info->phdr_count; i++)
	{
		// print_phdr(&(phdr[i]), i);
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
	{
		_plog("Found more than one executable header; Attempting to recover section labels to identify where .text is stored...");
		// call fallback function exploring header sections to store .text
		return false;
	}

	ctx->xphdr.txt_offset = phdr[xphdr_index].p_offset;
	ctx->xphdr.txt_size = &(phdr[xphdr_index]).p_filesz;
	ctx->xphdr.mem_size = &(phdr[xphdr_index]).p_memsz;
	ctx->xphdr.txt_vaddress = phdr[xphdr_index].p_vaddr;
	ctx->xphdr.cave_offset = ctx->xphdr.txt_offset + *(ctx->xphdr.txt_size);
	ctx->xphdr.cave_lenght = compute_cave_lenght(ctx->xphdr.txt_size);

	// size_t total_header_lenght = *(ctx->xphdr.txt_size) + ctx->xphdr.cave_lenght;

	// printf("\n%s--- Code cave of %ld bytes was found at offset %ld ---%s\n",
	//    INFO, ctx->xphdr.cave_lenght, ctx->xphdr.cave_offset, RESET);
	// printf("Total header lenght is %ld, which is %s\n",
	//    total_header_lenght, total_header_lenght % x86_64_PAGE_SZ == 0 ? "well formed" : "malformed");
	return true;
}

// We need this to allocate if the cave code is too small, but it will also need to be rounded to the next multiple of 4096
// unsigned char *updated_ehdr = malloc(file_len);
// if (!updated_ehdr)
// 	return _perror(strerror(errno));
// ft_memcpy(ehdr, ehdr, file_len);
// ctx->updated_file_map = ehdr;

bool insert_stub(void *file_map, s_bin_ctx *ctx)
{
	extern unsigned char _binary_stub_bin_start[];
	extern unsigned char _binary_stub_bin_end[];
	Elf64_Xword stub_len = _binary_stub_bin_end - _binary_stub_bin_start;

	// printf("\n%s--- Code cave has %ld bytes availables for a stub_len of %ld - Copy is %s ---%s\n",
	//    INFO, ctx->xphdr.cave_lenght, stub_len, stub_len < ctx->xphdr.cave_lenght ? "possible" : "impossible", RESET);

	if (stub_len > ctx->xphdr.cave_lenght)
		return _perror("Code cave is too short for stub");

	ft_memcpy(file_map + ctx->xphdr.cave_offset, _binary_stub_bin_start, stub_len);
	printf("Entry point 0x%lx\n", *(ctx->program_entrypoint));

	Elf64_Addr *o_entry = (Elf64_Addr *)(file_map + ctx->xphdr.cave_offset + OENTRY_OFF);
	*o_entry = ctx->original_entrypoint;
	*(ctx->program_entrypoint) = ctx->xphdr.txt_vaddress + *(ctx->xphdr.txt_size);
	printf("Entry point 0x%lx\n", *(ctx->program_entrypoint));

	Elf64_Addr *stub_vaddr = (Elf64_Addr *)(file_map + ctx->xphdr.cave_offset + STUB_VADDR_OFF);
	*stub_vaddr = *(ctx->program_entrypoint);
	*(ctx->xphdr.txt_size) += stub_len;
	*(ctx->xphdr.mem_size) += stub_len;

	free(ctx->xphdr.txt_data);
	ctx->xphdr.txt_data = NULL;
	ctx->xphdr.txt_data = file_map + ctx->xphdr.txt_offset;
	return true;
}
