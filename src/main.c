#include "woody.h"
#include "libft.h"
#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

/* Finds and stores the executable header of the binary, which contains .text
Return : `true` if ephdr == 1, `false` otherwise
*/

extern unsigned char _binary_src_stub_nasm_start[];
extern unsigned char _binary_src_stub_nasm_end[];
extern unsigned char _binary_src_stub_nasm_size[];



// bool create_woody_file() 
// {
// 	int woody = open("woody", O_CREAT);
// 	if (!woody)
// 		return _perror(strerror(errno));

// 	return true;
// }

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
	printf("Checking stub file is correctly baked in woody-woodpacker... Stub Size: %ld\n", _binary_src_stub_nasm_end - _binary_src_stub_nasm_start);
	free(ctx.text_data);
	close(bin_fd);
	return 0;
}
