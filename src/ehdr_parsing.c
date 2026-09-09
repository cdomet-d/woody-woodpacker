#include "woody.h"
#include "libft.h"

static void set_encryption_positions(s_exec_seg *exec_seg,
									 Elf64_Phdr *phdr, s_hdr_info *info)
{
	if (phdr->p_offset == 0)
	{
		Elf64_Off headers_off = info->phdr_offset + (info->phdr_size * info->phdr_count);
		exec_seg->xphdr.v_addr = phdr->p_vaddr + headers_off;
		exec_seg->xphdr.fsize_val = phdr->p_filesz - headers_off;
		exec_seg->xphdr.hdr_offset = headers_off;
	}
	else
	{
		exec_seg->xphdr.v_addr = phdr->p_vaddr;
		exec_seg->xphdr.hdr_offset = phdr->p_offset;
		exec_seg->xphdr.fsize_val = phdr->p_filesz;
	}
	// printf("Base offset was %ld. Encryption will start at %ld\n", phdr.p_offset, exec_seg->xphdr.hdr_offset);
}

static void set_exec_seg_info(s_exec_seg *exec_seg, Elf64_Phdr *xphdr, s_hdr_info *hdr_info)
{
	set_encryption_positions(exec_seg, xphdr, hdr_info);
	exec_seg->xphdr.fsizse_addr = &(xphdr)->p_filesz;
	exec_seg->xphdr.mem_size_addr = &(xphdr)->p_memsz;
	exec_seg->xphdr.cave_offset = exec_seg->xphdr.hdr_offset + *(exec_seg->xphdr.fsizse_addr);
	exec_seg->xphdr.cave_lenght = compute_cave_lenght(*(exec_seg->xphdr.fsizse_addr));
}

void init_program_info(Elf64_Ehdr *ehdr, s_exec_seg *exec_seg, s_hdr_info *hdr_info)
{
	exec_seg->program_entrypoint = &(ehdr->e_entry);
	exec_seg->original_entrypoint = ehdr->e_entry;
	hdr_info->phdr_count = ehdr->e_phnum;
	hdr_info->phdr_offset = ehdr->e_phoff;
	hdr_info->phdr_size = ehdr->e_phentsize;
}

/* Finds and stores the executable PT_LOAD segment of the binary, which contains the .text section
Return : `true` if xphdr == 1, `false` otherwise
*/
bool init_exec_seg(Elf64_Phdr *filemap, s_hdr_info *hdr_info, s_exec_seg *exec_seg)
{
	size_t xphdr_count = 0, xphdr_i = 0;

	for (size_t i = 0; i < hdr_info->phdr_count; i++)
	{
		if (filemap[i].p_filesz > filemap[i].p_memsz)
			return _perror("Malformed binary doesn't allocate enough memory for its size");
		if (filemap[i].p_type == PT_INTERP)
			return _perror("Linker invocation is not supported");
		if (filemap[i].p_type == PT_LOAD && filemap[i].p_flags & PF_X && xphdr_count == 0)
		{
			xphdr_i = i;
			xphdr_count++;
		}
	}
	if (xphdr_count != 1)
		return _perror("Invalid value of executable PT_LOAD");
	set_exec_seg_info(exec_seg, &(filemap[xphdr_i]), hdr_info);
	if (!is_safe_offset(exec_seg))
		return _perror(strerror(ERANGE));
	if (!is_safe_cave(xphdr_i, filemap, hdr_info, exec_seg))
		return _perror("Code cave is unsafe because another segment overlaps it");
	filemap[xphdr_i].p_flags = 7;
	return true;
}
