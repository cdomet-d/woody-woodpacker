#define STUB_VADDR_OFF 0x1a1
#define TEXT_OFF 0x16b
#define OENTRY_OFF 0x199
#define KEY_OFF 0x17b
#define TEXTSZ_OFF 0x173

#include "woody.h"
#include "libft.h"

static size_t compute_cave_lenght(Elf64_Xword txt_size)
{
	size_t aligned = (txt_size + x86_64_PAGE_SZ - 1) & ~(x86_64_PAGE_SZ - 1);
	return aligned - txt_size;
}

static bool is_safe_cave(size_t self, Elf64_Phdr *filemap, const s_pdhr_info *phdr_info, s_bin_ctx *ctx)
{
	Elf64_Off cave_seg_start = ctx->xphdr.txt_offset;
	Elf64_Off cave_seg_end = ctx->xphdr.cave_offset + ctx->xphdr.cave_lenght;

	printf("Segment to be encrypted is %ld bytes long, starting at offset %ld, \
ending at offset %ld\n",
		   cave_seg_end - cave_seg_start, cave_seg_start, cave_seg_end);
	for (size_t i = 0; i < phdr_info->phdr_count; i++)
	{
		if (i == self)
			continue;
		Elf64_Off tested_seg_start = filemap[i].p_offset;
		Elf64_Off tested_seg_end = filemap[i].p_offset + filemap[i].p_filesz;
		if (tested_seg_end > cave_seg_start && tested_seg_start < cave_seg_end)
		{
			printf("Found another header in the encryption segment, at offset %ld\n", filemap[i].p_offset);
			return false;
		}
	}
	return true;
}

static void set_encryption_data(s_bin_ctx *ctx,
								Elf64_Phdr phdr, const s_pdhr_info *info)
{
	if (phdr.p_offset == 0)
	{
		Elf64_Off headers_off = info->phdr_offset + (info->phdr_size * info->phdr_count);
		ctx->xphdr.txt_vaddress = phdr.p_vaddr + headers_off;
		ctx->xphdr.txt_size_val = phdr.p_filesz - headers_off;
		ctx->xphdr.txt_offset = headers_off;
	}
	else
	{
		ctx->xphdr.txt_vaddress = phdr.p_vaddr;
		ctx->xphdr.txt_offset = phdr.p_offset;
		ctx->xphdr.txt_size_val = phdr.p_filesz;
	}
	printf("Base offset was %ld. Encryption will start at %ld\n", phdr.p_offset, ctx->xphdr.txt_offset);
}

/* Finds and stores the executable PT_LOAD segment of the binary, which contains the .text section
Return : `true` if ephdr == 1, `false` otherwise
*/
bool find_xphdr(Elf64_Phdr *filemap, const s_pdhr_info *phdr_info, s_bin_ctx *ctx)
{
	int ephdr_count = 0;
	size_t xphdr_i = 0;

	for (int i = 0; i < phdr_info->phdr_count; i++)
	{
		if (filemap[i].p_flags & PF_X && filemap[i].p_type == PT_LOAD)
		{
			if (ephdr_count == 0)
				xphdr_i = i;
			ephdr_count++;
		}
	}

	if (ephdr_count == 0 || ephdr_count > 1)
		return _perror("Invalid value of executable PT_LOAD");

	set_encryption_data(ctx, filemap[xphdr_i], phdr_info);
	ctx->xphdr.txt_size_addr = &(filemap[xphdr_i]).p_filesz;
	ctx->xphdr.mem_size_addr = &(filemap[xphdr_i]).p_memsz;
	ctx->xphdr.cave_offset = ctx->xphdr.txt_offset + *(ctx->xphdr.txt_size_addr);
	ctx->xphdr.cave_lenght = compute_cave_lenght(*(ctx->xphdr.txt_size_addr));

	if (!is_safe_offset(ctx))
		return _perror(strerror(ERANGE));
	if (!is_safe_cave(xphdr_i, filemap, phdr_info, ctx))
		return _perror("Found another segment in the code cave.");
	filemap[xphdr_i].p_flags = 7;
	return true;
}

bool insert_stub(void *file_map, s_bin_ctx *ctx)
{
	extern unsigned char _binary_stub_bin_start[];
	extern unsigned char _binary_stub_bin_end[];

	Elf64_Xword stub_len = _binary_stub_bin_end - _binary_stub_bin_start;
	Elf64_Addr *text = (Elf64_Addr *)(file_map + ctx->xphdr.cave_offset + TEXT_OFF);
	Elf64_Xword *text_size = (Elf64_Xword *)(file_map + ctx->xphdr.cave_offset + TEXTSZ_OFF);
	unsigned char *key = (unsigned char *)(file_map + ctx->xphdr.cave_offset + KEY_OFF);

	if (stub_len > ctx->xphdr.cave_lenght)
		return _perror("Code cave is too short for stub");

	ft_memcpy(file_map + ctx->xphdr.cave_offset, _binary_stub_bin_start, stub_len);

	Elf64_Addr *o_entry = (Elf64_Addr *)(file_map + ctx->xphdr.cave_offset + OENTRY_OFF);
	Elf64_Addr *stub_vaddr = (Elf64_Addr *)(file_map + ctx->xphdr.cave_offset + STUB_VADDR_OFF);

	*o_entry = ctx->original_entrypoint;

	*(ctx->program_entrypoint) = ctx->xphdr.txt_vaddress + *(ctx->xphdr.txt_size_addr);
	*stub_vaddr = *(ctx->program_entrypoint);
	printf("New entrypoint: %#lx\n", *(ctx->program_entrypoint));
	ft_memcpy(key, ctx->key, 16);
	*text = ctx->xphdr.txt_vaddress;
	*text_size = ctx->xphdr.txt_size_val;

	*(ctx->xphdr.txt_size_addr) += stub_len;
	*(ctx->xphdr.mem_size_addr) += stub_len;

	return true;
}
