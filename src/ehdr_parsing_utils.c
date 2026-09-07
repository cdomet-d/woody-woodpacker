#include "woody.h"
#include "libft.h"

static s_pt_load_range *get_exec_range(size_t count, s_pt_load_range *ranges)
{
	for (size_t i = 0; i < count; i++)
	{
		if (ranges[i].is_executable)
			return ranges + i;
	}
	return NULL;
}

static bool is_safe_address(const Elf64_Addr to_check, const s_pt_load_range *danger_range)
{
	// print_pt_load_range(danger_range);
	return to_check <= danger_range->vaddr_start || to_check > danger_range->vaddr_end;
}

size_t compute_cave_lenght(Elf64_Xword txt_size)
{
	size_t aligned = (txt_size + x86_64_PAGE_SZ - 1) & ~(x86_64_PAGE_SZ - 1);
	return aligned - txt_size;
}

bool is_safe_cave(size_t self,
				  Elf64_Phdr *filemap, const s_pdhr_info *phdr_info, s_bin_ctx *ctx)
{
	Elf64_Off cave_seg_start = ctx->xphdr.hdr_offset;
	Elf64_Off cave_seg_end = ctx->xphdr.cave_offset + ctx->xphdr.cave_lenght;

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

void set_encryption_data(s_bin_ctx *ctx,
						 Elf64_Phdr phdr, const s_pdhr_info *info)
{
	if (phdr.p_offset == 0)
	{
		Elf64_Off headers_off = info->phdr_offset + (info->phdr_size * info->phdr_count);
		ctx->xphdr.v_addr = phdr.p_vaddr + headers_off;
		ctx->xphdr.fsize_val = phdr.p_filesz - headers_off;
		ctx->xphdr.hdr_offset = headers_off;
	}
	else
	{
		ctx->xphdr.v_addr = phdr.p_vaddr;
		ctx->xphdr.hdr_offset = phdr.p_offset;
		ctx->xphdr.fsize_val = phdr.p_filesz;
	}
	// printf("Base offset was %ld. Encryption will start at %ld\n", phdr.p_offset, ctx->xphdr.hdr_offset);
}

size_t get_pt_load_count(Elf64_Phdr *filemap, s_pdhr_info *info)
{
	size_t count = 0;

	for (size_t i = 0; i < info->phdr_count; i++)
		if (filemap[i].p_type == PT_LOAD)
			count++;
	return count;
}

bool validate_dt_init(const s_bin_ctx *ctx, Elf64_Dyn *dyn)
{
	Elf64_Xword dt_init_entry_count = 0;
	Elf64_Addr dt_init_arr = 0;
	for (size_t i = 0; dyn[i].d_tag != DT_NULL; i++)
	{
		printf("%.20s	", dtag_value(dyn[i].d_tag));
		printf("%016ld | %016ld\n", dyn[i].d_un.d_ptr, dyn[i].d_un.d_val / sizeof(Elf64_Addr));
		if (dyn[i].d_tag == DT_INIT)
		{
			if (!is_safe_address(dyn[i].d_un.d_ptr, get_exec_range(ctx->pt_load_count, ctx->pt_loads)))
				return _perror("Unsafe address for DT_INIT");
		}
		else if (dyn[i].d_tag == DT_INIT_ARRAY)
			dt_init_arr = dyn[i].d_un.d_ptr;
		else if (dyn[i].d_tag == DT_INIT_ARRAYSZ)
			dt_init_entry_count = (dyn[i].d_un.d_val / sizeof(Elf64_Addr));
	}
	printf("Addr: %ld | sz : %ld\n", dt_init_arr, dt_init_entry_count);

	return true;
}