#include "woody.h"
#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

bool validate_binary(Elf64_Ehdr *bin_ptr)
{
	if (!is_valid_magic(bin_ptr->e_ident))
		return _perror("File format not supported");
	_plog("File format is ELF");

	if (!is_valid_format(bin_ptr->e_ident[EI_CLASS]))
		return _perror("File architecture not supported");
	_plog("File architecture is 64-bit");
	if (!is_valid_machine(bin_ptr->e_machine))
		return _perror("Machine architecture not supported");
	_plog("Machine is AMD x86-64");
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
	if (!validate_binary(ehdr))
		return false;
	pehdr(ehdr);
	return 0;
}