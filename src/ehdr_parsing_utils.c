#include "woody.h"
#include "libft.h"

// static s_pt_load_range *get_exec_range(size_t count, s_pt_load_range *ranges)
// {
// 	for (size_t i = 0; i < count; i++)
// 	{
// 		if (ranges[i].is_executable)
// 			return ranges + i;
// 	}
// 	return NULL;
// }

// static bool is_safe_address(const Elf64_Rela to_check, const s_pt_load_range *danger_range)
// {
// 	// print_pt_load_range(danger_range);
// 	return to_check <= danger_range->vaddr_start || to_check > danger_range->vaddr_end;
// }

static bool get_offset_from_vaddr(Elf64_Addr target, Elf64_Off *init_off, s_pt_load_range *pt_loads, size_t count)
{

	for (size_t i = 0; i < count; i++)
	{
		if (target >= pt_loads[i].vaddr_start && target < pt_loads[i].vaddr_end)
		{
			Elf64_Off delta = target - pt_loads[i].vaddr_start;
			*init_off = pt_loads[i].offset + delta;
			return true;
		}
	}
	_perror("Could not find segment in which DT_INIT_ARRAY is supposed to be, which is extremely weird");
	return false;
}

size_t compute_cave_lenght(Elf64_Xword txt_size)
{
	size_t aligned = (txt_size + x86_64_PAGE_SZ - 1) & ~(x86_64_PAGE_SZ - 1);
	return aligned - txt_size;
}

bool is_safe_cave(size_t self,
				  Elf64_Phdr *filemap, const s_pdhr_info *phdr_info, s_bin_exec_seg *exec_seg)
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

void set_encryption_data(s_bin_exec_seg *exec_seg,
						 Elf64_Phdr phdr, const s_pdhr_info *info)
{
	if (phdr.p_offset == 0)
	{
		Elf64_Off headers_off = info->phdr_offset + (info->phdr_size * info->phdr_count);
		exec_seg->xphdr.v_addr = phdr.p_vaddr + headers_off;
		exec_seg->xphdr.fsize_val = phdr.p_filesz - headers_off;
		exec_seg->xphdr.hdr_offset = headers_off;
	}
	else
	{
		exec_seg->xphdr.v_addr = phdr.p_vaddr;
		exec_seg->xphdr.hdr_offset = phdr.p_offset;
		exec_seg->xphdr.fsize_val = phdr.p_filesz;
	}
	// printf("Base offset was %ld. Encryption will start at %ld\n", phdr.p_offset, exec_seg->xphdr.hdr_offset);
}

size_t get_pt_load_count(Elf64_Phdr *filemap, s_pdhr_info *info)
{
	size_t count = 0;

	for (size_t i = 0; i < info->phdr_count; i++)
		if (filemap[i].p_type == PT_LOAD)
			count++;
	return count;
}

bool init_rela_info(const s_dyn_info *dyn_info, s_rela_arr *rela_arr, Elf64_Dyn *dyn)
{

	for (size_t i = 0; dyn[i].d_tag != DT_NULL; i++)
	{
		// if (dyn[i].d_tag == DT_INIT)
		// {
		// 	if (!is_safe_address(dyn[i].d_un.d_ptr, get_exec_range(dyn_info->pt_load_count, dyn_info->pt_loads)))
		// 		return _perror("Unsafe address for DT_INIT");
		// }
		if (dyn[i].d_tag == DT_RELA)
			rela_arr->vaddr = dyn[i].d_un.d_ptr;
		else if (dyn[i].d_tag == DT_RELASZ)
			rela_arr->sz = dyn[i].d_un.d_val;
		else if (dyn[i].d_tag == DT_RELAENT)
		{
			printf("sizeof(Elf64_Rela): %ld\n", sizeof(Elf64_Rela));
			if (dyn[i].d_un.d_val != sizeof(Elf64_Rela))
				return _perror("Malformed relocation entry size");
		}
	}
	rela_arr->entry_count = (rela_arr->sz / sizeof(Elf64_Rela));
	if (!get_offset_from_vaddr(rela_arr->vaddr, &(rela_arr->offset), dyn_info->pt_loads, dyn_info->pt_load_count))
		return false;
	return true;
}

// bool validate_init_array(Elf64_Rela *rela_arr, const s_dyn_info *pt_loads, s_rela_arr *rela_arr_info)
// {
// 	const s_pt_load_range *exec = get_exec_range(pt_loads->pt_load_count, pt_loads->pt_loads);

// 	for (size_t i = 0; i < rela_arr_info->entry_count; i++)
// 	{
// 		if (!is_safe_address(rela_arr[i], exec))
// 		{
// 			print_pt_load_range(exec);
// 			printf("0x%016lx", rela_arr[i]);
// 			return false;
// 		}
// 	}
// 	return true;
// }
