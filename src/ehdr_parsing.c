#include "woody.h"
#include "libft.h"

/* Finds and stores the executable PT_LOAD segment of the binary, which contains the .text section
Return : `true` if xphdr == 1, `false` otherwise
*/
bool get_dynamic_and_executable_headers(Elf64_Phdr *filemap, const s_pdhr_info *hdr_info,
										s_bin_exec_seg *exec_seg, s_dyn_info *dyn_info)
{
	size_t xphdr_count = 0;
	size_t pt_load_i = 0;
	size_t xphdr_i = 0;

	for (size_t i = 0; i < hdr_info->phdr_count; i++)
	{
		if (filemap[i].p_filesz > filemap[i].p_memsz)
			return _perror("Malformed binary doesn't have enough memory to allocate its length");

		if (filemap[i].p_type == PT_DYNAMIC)
		{
			dyn_info->dynhdr.v_addr = filemap[i].p_vaddr;
			dyn_info->dynhdr.hdr_offset = filemap[i].p_offset;
			dyn_info->dynhdr.fsize_val = filemap[i].p_filesz;
		}

		if (filemap[i].p_type == PT_LOAD)
		{
			dyn_info->pt_loads[pt_load_i].is_executable = false;
			dyn_info->pt_loads[pt_load_i].offset = filemap[i].p_offset;
			dyn_info->pt_loads[pt_load_i].vaddr_end = filemap[i].p_vaddr + filemap[i].p_filesz;
			dyn_info->pt_loads[pt_load_i].vaddr_start = filemap[i].p_vaddr;
			if (filemap[i].p_flags & PF_X && xphdr_count == 0)
			{
				xphdr_i = i;
				xphdr_count++;
				dyn_info->pt_loads[pt_load_i].is_executable = true;
			}
			pt_load_i++;
		}
	}

	if (xphdr_count != 1)
		return _perror("Invalid value of executable PT_LOAD");

	set_encryption_data(exec_seg, filemap[xphdr_i], hdr_info);
	exec_seg->xphdr.fsizse_addr = &(filemap[xphdr_i]).p_filesz;
	exec_seg->xphdr.mem_size_addr = &(filemap[xphdr_i]).p_memsz;
	exec_seg->xphdr.cave_offset = exec_seg->xphdr.hdr_offset + *(exec_seg->xphdr.fsizse_addr);
	exec_seg->xphdr.cave_lenght = compute_cave_lenght(*(exec_seg->xphdr.fsizse_addr));

	if (!is_safe_offset(exec_seg))
		return _perror(strerror(ERANGE));
	if (!is_safe_cave(xphdr_i, filemap, hdr_info, exec_seg))
		return _perror("Found another segment in the code cave.");
	filemap[xphdr_i].p_flags = 7;
	return true;
}