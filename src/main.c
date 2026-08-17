#include "woody.h"
#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>

/* Finds and stores the executable header of the binary, which contains .text
Return : `true` if ephdr == 1, `false` otherwise
*/
bool get_xphdr(Elf64_Phdr *phdr, s_bin_ctx *ctx)
{
	int ephdr_count = 0;
	Elf64_Phdr xphdr = {0};
	
	printf("\n%s--- Program Headers (%d entries) ---%s\n", INFO, ctx->program_hdr_count, RESET);
	for (int i = 0; i < ctx->program_hdr_count; i++)
	{
		print_phdr(phdr[i], i);
		if (phdr[i].p_flags & PF_X && phdr[i].p_type == PT_LOAD)
		{
			if (ephdr_count == 0)
				xphdr = phdr[i];
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
	ctx->text_offset = xphdr.p_offset;
	ctx->text_size = xphdr.p_filesz;
	ctx->text_vaddress = xphdr.p_vaddr;
	return true;
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

int main(int argc, char *argv[])
{
	if (argc < 2)
		return _perror("Usage: ./woody_woodpacker <binary>");
	int bin_fd = open(argv[1], O_RDONLY);
	if (bin_fd == -1)
		return _perror(strerror(errno));
	size_t len = lseek(bin_fd, 0, SEEK_END);
	lseek(bin_fd, 0, SEEK_SET);

	// We need PROT_WRITE so we can write our changes to the binary (encryption, stub injection, etc...)
	void *file_map = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_PRIVATE, bin_fd, 0);
	if (file_map == MAP_FAILED)
		return _perror(strerror(errno));

	// ehdr is the ELF Header ; it holds the info regarding the whole Elf file
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)file_map;
	s_bin_ctx ctx = {0};
	if (!validate_format(ehdr, &ctx))
		return false;

	// We use the program offset from ehdr to find the adress of the first program header
	if (!get_xphdr((Elf64_Phdr *)(file_map + ctx.program_hdr_offset), &ctx))
		return false;
	ctx.text_data = malloc(ctx.text_size);
	if (!ctx.text_data)
		return _perror(strerror(errno));
	ctx.text_data = ft_memcpy(ctx.text_data, (file_map + ctx.text_offset), ctx.text_size);
	print_text_data(ctx.text_data, ctx.text_size, ctx.text_offset);
	encrypt_text(ctx.text_data);
	free(ctx.text_data);
	return 0;
}
