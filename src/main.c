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

	s_bin_exec_seg exec_seg = {0};
	s_dyn_info dyn_info = {0};
	s_pdhr_info phdrs = {0};

	if (!validate_format((Elf64_Ehdr *)file_map, &exec_seg, &phdrs))
		return exit_on_error(NULL, NULL, bin_fd);

	dyn_info.pt_load_count = get_pt_load_count((Elf64_Phdr *)(file_map + phdrs.phdr_offset), &phdrs);
	dyn_info.pt_loads = malloc(dyn_info.pt_load_count * sizeof(s_pt_load_range));
	if (!dyn_info.pt_loads)
		return exit_on_error(strerror(errno), NULL, bin_fd);

	if (!get_dynamic_and_executable_headers((Elf64_Phdr *)(file_map + phdrs.phdr_offset), &phdrs, &exec_seg, &dyn_info))
		return exit_on_error(NULL, dyn_info.pt_loads, bin_fd);

	s_rela_arr rela_arr = {0};
	if (!init_rela_info(&dyn_info, &rela_arr, (Elf64_Dyn *)(file_map + dyn_info.dynhdr.hdr_offset)))
		return exit_on_error(NULL, dyn_info.pt_loads, bin_fd);
	printf("Addr: %ld | sz : %ld | entries: %ld\n", rela_arr.vaddr, rela_arr.sz, rela_arr.entry_count);
	printf("Offset for DT_RELA is: %ld\n", rela_arr.offset);

	// if (!validate_init_array((Elf64_Addr *)(file_map + rela_arr.offset), &dyn_info, &rela_arr))
	// 	return exit_on_error("Found out of bound function address", dyn_info.pt_loads, bin_fd);
	exec_seg.xphdr.encrypted_data = (unsigned char *)file_map + exec_seg.xphdr.hdr_offset;
	if (!create_cipher_key(exec_seg.key))
		return exit_on_error(NULL, dyn_info.pt_loads, bin_fd);
	encrypt_text(exec_seg.key, exec_seg.xphdr.encrypted_data, exec_seg.xphdr.fsize_val);

	if (!insert_stub(file_map, &exec_seg))
		return exit_on_error(NULL, dyn_info.pt_loads, bin_fd);
	if (!create_woody_file(file_map, len))
		return exit_on_error(NULL, dyn_info.pt_loads, bin_fd);
	free(dyn_info.pt_loads);
	close(bin_fd);
	return 0;
}
