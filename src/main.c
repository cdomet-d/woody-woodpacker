#include "woody.h"
#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>

#define REQ_ARG 2

int exit_on_error(const char *err, int fd)
{
	if (fd)
		close(fd);
	if (err)
		_perror(err);
	return 1;
}

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
	if (argc != REQ_ARG)
		return exit_on_error("Usage: ./woody_woodpacker <binary>", 0);
	int bin_fd = open(argv[1], O_RDONLY);
	if (!bin_fd)
		return exit_on_error(strerror(errno), 0);
	off_t len = lseek(bin_fd, 0, SEEK_END);
	if (len == -1)
		return exit_on_error(strerror(errno), bin_fd);
	off_t sret = lseek(bin_fd, 0, SEEK_SET);
	if (sret == -1)
		return exit_on_error(strerror(errno), bin_fd);
	void *file_map = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_PRIVATE, bin_fd, 0);
	if (file_map == MAP_FAILED)
		return exit_on_error(strerror(errno), bin_fd);
	s_exec_seg exec_seg = {0};
	s_hdr_info phdrs = {0};
	if (!validate_format((Elf64_Ehdr *)file_map))
		return exit_on_error(NULL, bin_fd);
	init_program_info((Elf64_Ehdr *)file_map, &exec_seg, &phdrs);
	if (!init_exec_seg((Elf64_Phdr *)(file_map + phdrs.phdr_offset), &phdrs, &exec_seg))
		return exit_on_error(NULL, bin_fd);
	exec_seg.xphdr.encrypted_data = (unsigned char *)file_map + exec_seg.xphdr.hdr_offset;
	if (!create_cipher_key(exec_seg.key))
		return exit_on_error(NULL, bin_fd);
	encrypt_text(exec_seg.key, exec_seg.xphdr.encrypted_data, exec_seg.xphdr.fsize_val);
	if (!insert_stub(file_map, &exec_seg))
		return exit_on_error(NULL, bin_fd);
	if (!create_woody_file(file_map, len))
		return exit_on_error(NULL, bin_fd);
	close(bin_fd);
	_psuccess("Created woody :)");
	return 0;
}
