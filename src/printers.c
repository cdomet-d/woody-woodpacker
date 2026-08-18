#include "woody.h"
#include "libft.h"

void print_ehdr(const char *ftype, const char *fclass, const Elf64_Addr entrypoint, const s_pdhr_info *iphdr)
{
	printf("\n%s--- ELF Header ---%s\n", INFO, RESET);

	printf("Elf file type is %-45s\n", ftype);
	printf("Elf file class is %s\n", fclass);
	printf("Entry point 0x%lx\n", entrypoint);
	printf("There are %u program headers, starting at offset %lu\n", iphdr->phdr_count, iphdr->phdr_offset);
}

#include <stdio.h>
#include <elf.h>

// Turn p_type's numeric value into a readable name
static const char *phdr_type_str(Elf64_Word type)
{
	switch (type)
	{
	case PT_NULL:
		return "PT_NULL";
	case PT_LOAD:
		return "PT_LOAD";
	case PT_DYNAMIC:
		return "PT_DYNAMIC";
	case PT_INTERP:
		return "PT_INTERP";
	case PT_NOTE:
		return "PT_NOTE";
	case PT_SHLIB:
		return "PT_SHLIB";
	case PT_PHDR:
		return "PT_PHDR";
	case PT_TLS:
		return "PT_TLS";
	case PT_GNU_EH_FRAME:
		return "PT_GNU_EH_FRAME";
	case PT_GNU_STACK:
		return "PT_GNU_STACK";
	case PT_GNU_RELRO:
		return "PT_GNU_RELRO";
	default:
		return "UNKNOWN";
	}
}

void print_phdr(const Elf64_Phdr *phdr, const int i)
{
	char flags[4] = {0};

	flags[0] = phdr->p_flags & PF_R ? 'R' : ' ';
	flags[1] = phdr->p_flags & PF_W ? 'W' : ' ';
	flags[2] = phdr->p_flags & PF_X ? 'X' : ' ';

	printf("Header [%02d]	%-3s	%-15s\n", i, flags, phdr_type_str(phdr->p_type));
}

// to find the cave
// figure out how long the text size is
// find the nearest multiple of 4096
void print_xphdr(const s_xphdr *xphdr)
{
	char ascii[17] = {0};
	size_t total_header_lenght = xphdr->txt_size + xphdr->cave_lenght;

	printf("\n%s--- Printing executable text segment of size %lu at offset %ld ---%s\n",
		   INFO, xphdr->txt_size, xphdr->txt_offset, RESET);
	printf("\n%s--- Code cave of %ld bytes was found at offset %ld ---%s\n",
		   INFO, xphdr->cave_lenght, xphdr->cave_offset, RESET);

	printf("Total header lenght is %ld, which is %s\n", 
		total_header_lenght, total_header_lenght % x86_64_PAGE_SZ == 0 ? "well formed" : "malformed");
	printf("\n");

	for (Elf64_Xword i = 0; i < total_header_lenght; i++)
	{
		if ((i) % 16 == 0)
			printf("%08lX: ", xphdr->txt_offset + i);
		printf("%02x", xphdr->txt_data[i]);
		if (i % 2)
			printf(" ");
		ascii[i % 16] = xphdr->txt_data[i] >= ' ' && xphdr->txt_data[i] < 127 ? xphdr->txt_data[i] : '.';
		if ((i + 1) % 16 == 0)
		{
			printf("	%s\n", ascii);
			ft_memset(ascii, 0, 17);
		}
	}
	printf("\n");
}
