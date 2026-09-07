#include "woody.h"
#include "libft.h"

/* Finds and stores the executable PT_LOAD segment of the binary, which contains the .text section
Return : `true` if xphdr == 1, `false` otherwise
*/
bool get_dynamic_and_executable_headers(Elf64_Phdr *filemap, const s_pdhr_info *info, s_bin_ctx *ctx)
{
	size_t xphdr_count = 0;
	size_t pt_load_i = 0;
	size_t xphdr_i = 0;

	for (size_t i = 0; i < info->phdr_count; i++)
	{
		if (filemap[i].p_filesz > filemap[i].p_memsz)
			return _perror("Malformed binary doesn't have enough memory to allocate its length");

		if (filemap[i].p_type == PT_DYNAMIC)
		{
			ctx->dynhdr.v_addr = filemap[i].p_vaddr;
			ctx->dynhdr.hdr_offset = filemap[i].p_offset;
			ctx->xphdr.fsize_val = filemap[i].p_filesz;
		}

		if (filemap[i].p_type == PT_LOAD)
		{
			ctx->pt_loads[pt_load_i].vaddr_start = filemap[i].p_vaddr;
			ctx->pt_loads[pt_load_i].vaddr_end = filemap[i].p_vaddr + filemap[i].p_filesz;
			ctx->pt_loads[pt_load_i].is_executable = false;
			if (filemap[i].p_flags & PF_X && xphdr_count == 0)
			{
				xphdr_i = i;
				xphdr_count++;
				ctx->pt_loads[pt_load_i].is_executable = true;
			}
			pt_load_i++;
		}
	}

	if (xphdr_count != 1)
		return _perror("Invalid value of executable PT_LOAD");

	set_encryption_data(ctx, filemap[xphdr_i], info);
	ctx->xphdr.fsizse_addr = &(filemap[xphdr_i]).p_filesz;
	ctx->xphdr.mem_size_addr = &(filemap[xphdr_i]).p_memsz;
	ctx->xphdr.cave_offset = ctx->xphdr.hdr_offset + *(ctx->xphdr.fsizse_addr);
	ctx->xphdr.cave_lenght = compute_cave_lenght(*(ctx->xphdr.fsizse_addr));

	if (!is_safe_offset(ctx))
		return _perror(strerror(ERANGE));
	if (!is_safe_cave(xphdr_i, filemap, info, ctx))
		return _perror("Found another segment in the code cave.");
	filemap[xphdr_i].p_flags = 7;
	return true;
}