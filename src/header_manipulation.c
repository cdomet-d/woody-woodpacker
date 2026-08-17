#define OENTRY 0x401039
#include "woody.h"

bool get_xphdr(Elf64_Phdr *phdr, s_bin_ctx *ctx)
{
	int ephdr_count = 0;
	Elf64_Phdr xphdr = {0};
	
	printf("\n%s--- Program Headers (%d entries) ---%s\n", INFO, ctx->program_hdr_count, RESET);
	for (int i = 0; i < ctx->program_hdr_count; i++)
	{
		print_phdr(phdr[i], i);
		if (phdr[i].p_flags & PF_X && phdr[i].p_type == PT_LOAD)
		{
			if (ephdr_count == 0)
				xphdr = phdr[i];
			ephdr_count++;
		}
	}
	if (ephdr_count == 0)
		return _perror("Couldn't find any executable headers");
	if (ephdr_count > 1) 
	{
		_plog("Found more than one executable header; Attempting to recover section labels to identify where .text is stored...");
		// call fallback function exploring header sections to store .text
		return false;
	}
	ctx->text_offset = xphdr.p_offset;
	ctx->text_size = xphdr.p_filesz;
	ctx->text_vaddress = xphdr.p_vaddr;
	return true;
}

bool update_entrypoint_in_stub()
{

}