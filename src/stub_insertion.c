#define STUB_VADDR_OFF 0x1a4
#define TEXT_OFF 0x16e
#define OENTRY_OFF 0x19c
#define KEY_OFF 0x17e
#define TEXTSZ_OFF 0x176

#include "woody.h"
#include "libft.h"

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

	*(ctx->program_entrypoint) = ctx->xphdr.v_addr + *(ctx->xphdr.fsizse_addr);
	*stub_vaddr = *(ctx->program_entrypoint);
	ft_memcpy(key, ctx->key, 16);
	*text = ctx->xphdr.v_addr;
	*text_size = ctx->xphdr.fsize_val;

	*(ctx->xphdr.fsizse_addr) += stub_len;
	*(ctx->xphdr.mem_size_addr) += stub_len;

	return true;
}
