#include "woody.h"
#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>

#define REQ_ARG 2
int exit_on_error(const char *err, s_pt_load_range *pt_loads, int fd)
{
	if (pt_loads)
		free(pt_loads);
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
		return exit_on_error("Usage: ./woody_woodpacker <binary>", NULL, 0);
	int bin_fd = open(argv[1], O_RDONLY);
	if (!bin_fd)
		return exit_on_error(strerror(errno), NULL, 0);
	off_t len = lseek(bin_fd, 0, SEEK_END);
	if (len == -1)
		return exit_on_error(strerror(errno), NULL, bin_fd);
	off_t sret = lseek(bin_fd, 0, SEEK_SET);
	if (sret == -1)
		return exit_on_error(strerror(errno), NULL, bin_fd);

	void *file_map = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_PRIVATE, bin_fd, 0);
	if (file_map == MAP_FAILED)
		return exit_on_error(strerror(errno), NULL, bin_fd);

	s_pdhr_info phdrs = {0};
	s_bin_ctx ctx = {0};

	if (!validate_format((Elf64_Ehdr *)file_map, &ctx, &phdrs))
		return exit_on_error(NULL, NULL, bin_fd);

	ctx.pt_load_count = get_pt_load_count((Elf64_Phdr *)(file_map + phdrs.phdr_offset), &phdrs);
	ctx.pt_loads = malloc(ctx.pt_load_count * sizeof(s_pt_load_range));
	if (!ctx.pt_loads)
		return exit_on_error(strerror(errno), NULL, bin_fd);

	if (!get_dynamic_and_executable_headers((Elf64_Phdr *)(file_map + phdrs.phdr_offset), &phdrs, &ctx))
		return exit_on_error(NULL, ctx.pt_loads, bin_fd);
	validate_dt_init(&ctx, (Elf64_Dyn *)(file_map + ctx.dynhdr.hdr_offset));
	
	ctx.xphdr.encrypted_data = (unsigned char *)file_map + ctx.xphdr.hdr_offset;
	if (!create_cipher_key(ctx.key))
		return exit_on_error(NULL, ctx.pt_loads, bin_fd);
	encrypt_text(ctx.key, ctx.xphdr.encrypted_data, ctx.xphdr.fsize_val);

	if (!insert_stub(file_map, &ctx))
		return exit_on_error(NULL, ctx.pt_loads, bin_fd);
	if (!create_woody_file(file_map, len))
		return exit_on_error(NULL, ctx.pt_loads, bin_fd);
	free(ctx.pt_loads);
	close(bin_fd);
	return 0;
}
