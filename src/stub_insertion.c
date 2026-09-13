#define STUB_VADDR_OFF 0x1ca
#define TEXT_OFF 0x163
#define OENTRY_OFF 0x1c2
#define KEY_OFF 0x173
#define TEXTSZ_OFF 0x16b
#define HEXKEY_OFF 0x1a1

#include "woody.h"
#include "libft.h"

bool insert_stub(void *file_map, s_exec_seg *exec_seg)
{
	extern unsigned char _binary_stub_bin_start[];
	extern unsigned char _binary_stub_bin_end[];

	Elf64_Xword stub_len = _binary_stub_bin_end - _binary_stub_bin_start;
	Elf64_Addr *text = (Elf64_Addr *)(file_map + exec_seg->xphdr.cave_offset + TEXT_OFF);
	Elf64_Xword *text_size = (Elf64_Xword *)(file_map + exec_seg->xphdr.cave_offset + TEXTSZ_OFF);
	Elf64_Addr *o_entry = (Elf64_Addr *)(file_map + exec_seg->xphdr.cave_offset + OENTRY_OFF);
	Elf64_Addr *stub_vaddr = (Elf64_Addr *)(file_map + exec_seg->xphdr.cave_offset + STUB_VADDR_OFF);
	unsigned char *key = (unsigned char *)(file_map + exec_seg->xphdr.cave_offset + KEY_OFF);
	unsigned char *hexkey = (unsigned char *)(file_map + exec_seg->xphdr.cave_offset + HEXKEY_OFF);

	if (stub_len > exec_seg->xphdr.cave_lenght)
		return _perror("Code cave is too short for stub");

	ft_memcpy(file_map + exec_seg->xphdr.cave_offset, _binary_stub_bin_start, stub_len);
	ft_memcpy(key, exec_seg->key, KEY_LENGHT);
	ft_memcpy(hexkey, exec_seg->hexkey, HEXKEY_LENGHT);

	*text = exec_seg->xphdr.v_addr;
	*text_size = exec_seg->xphdr.fsize_val;

	*o_entry = exec_seg->original_entrypoint;
	*(exec_seg->program_entrypoint) = exec_seg->xphdr.v_addr + *(exec_seg->xphdr.fsizse_addr);
	*stub_vaddr = *(exec_seg->program_entrypoint);

	*(exec_seg->xphdr.fsizse_addr) += stub_len;
	*(exec_seg->xphdr.mem_size_addr) += stub_len;
	return true;
}
