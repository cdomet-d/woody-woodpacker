#include "woody.h"
#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	(void)argv;
	if (argc < 2)
	{
		print_error("Usage: ./woody_woodpacker <binary>");
		return 1;
	}
	int bin_fd = open(argv[1], O_RDONLY);
	size_t len = lseek(bin_fd, 0, SEEK_END);
	lseek(bin_fd, 0, SEEK_SET);
	void *bin_ptr = mmap(NULL, len, PROT_READ, MAP_PRIVATE, bin_fd, 0);

	if (bin_ptr == MAP_FAILED)
		print_error(strerror(errno));

	Elf64_Ehdr *unpacked_bin = (Elf64_Ehdr *)bin_ptr;

	print_elf_struc(unpacked_bin);
	return 0;
}