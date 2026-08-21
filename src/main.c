#include "woody.h"
#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>

bool create_woody_file(unsigned char *updated_pbuffer, size_t pbuffer_len)
{
	int woody = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0755);
	if (!woody)
		return _perror(strerror(errno));
	int wret = write(woody, updated_pbuffer, pbuffer_len);
	close(woody);
	if (!wret)
		return _perror(strerror(errno));
	return true;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
		return _perror("Usage: ./woody_woodpacker <binary>");
	int bin_fd = open(argv[1], O_RDONLY);
	if (!bin_fd)
		return _perror(strerror(errno));
	off_t len = lseek(bin_fd, 0, SEEK_END);
	if (len == -1)
		return _perror(strerror(errno));
	off_t sret = lseek(bin_fd, 0, SEEK_SET);
	if (sret == -1)
		return _perror(strerror(errno));

	void *file_map = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_PRIVATE, bin_fd, 0);
	if (file_map == MAP_FAILED)
		return _perror(strerror(errno));

	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)file_map;
	s_pdhr_info phdrs = {0};
	s_bin_ctx ctx = {0};

	if (!validate_format(ehdr, &ctx, &phdrs))
		return false;
	if (!find_xphdr((Elf64_Phdr *)(file_map + phdrs.phdr_offset), &phdrs, &ctx))
		return false;

	ctx.xphdr.txt_data = (unsigned char *)(file_map + ctx.xphdr.txt_offset);

	if (!create_cipher_key(ctx.key)) {
		close(bin_fd);
		return 1;
	}
	encrypt_text(ctx.key, ctx.xphdr.txt_data, *(ctx.xphdr.txt_size));
	insert_stub(file_map, &ctx);
	create_woody_file(file_map, len);

	close(bin_fd);
	return 0;
}
