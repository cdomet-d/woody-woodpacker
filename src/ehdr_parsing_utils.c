#include "woody.h"
#include "libft.h"

size_t compute_cave_lenght(Elf64_Xword txt_size)
{
	size_t aligned = (txt_size + x86_64_PAGE_SZ - 1) & ~(x86_64_PAGE_SZ - 1);
	return aligned - txt_size;
}
bool is_safe_offset(const s_exec_seg *exec_seg)
{
	if (exec_seg->xphdr.hdr_offset > UINT64_MAX - *(exec_seg->xphdr.fsizse_addr))
		return false;
	if (exec_seg->xphdr.v_addr > UINT64_MAX - *(exec_seg->xphdr.fsizse_addr))
		return false;
	if (exec_seg->xphdr.cave_offset > UINT64_MAX - exec_seg->xphdr.cave_lenght)
		return false;
	return true;
}

bool is_safe_cave(size_t self,
				  Elf64_Phdr *filemap, const s_hdr_info *phdr_info, s_exec_seg *exec_seg)
{
	Elf64_Off cave_seg_start = exec_seg->xphdr.hdr_offset;
	Elf64_Off cave_seg_end = exec_seg->xphdr.cave_offset + exec_seg->xphdr.cave_lenght;

	for (size_t i = 0; i < phdr_info->phdr_count; i++)
	{
		if (i == self)
			continue;
		Elf64_Off tested_seg_start = filemap[i].p_offset;
		Elf64_Off tested_seg_end = filemap[i].p_offset + filemap[i].p_filesz;
		if (tested_seg_end > cave_seg_start && tested_seg_start < cave_seg_end)
			return false;
	}
	return true;
}
