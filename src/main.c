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

	s_pdhr_info phdrs = {0};
	s_bin_ctx ctx = {0};
	if (!validate_format(ehdr, &ctx, &phdrs))
		return false;

	// We use the program offset from ehdr to find the adress of the first program header
	if (!get_xphdr((Elf64_Phdr *)(file_map + phdrs.phdr_offset), &phdrs, &ctx))
		return false;
	ctx.xphdr.txt_data = malloc(ctx.xphdr.txt_size);
	if (!ctx.xphdr.txt_data)
		return _perror(strerror(errno));
	ft_memcpy(ctx.xphdr.txt_data, (file_map + ctx.xphdr.txt_offset), ctx.xphdr.txt_size);

	print_xphdr(&ctx.xphdr);
	realloc_headers(ehdr, &ctx);
	free(ctx.xphdr.txt_data);
	close(bin_fd);
	return 0;
}
