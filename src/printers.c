#include "woody.h"
#include "libft.h"

void print_ehdr(const char *ftype, const char *fclass, const Elf64_Addr entrypoint, const s_hdr_info *iphdr)
{
	printf("\n%s--- ELF Header ---%s\n", INFO, RESET);

	printf("Elf file type is %-45s\n", ftype);
	printf("Elf file class is %s\n", fclass);
	printf("Entry point 0x%lx\n", entrypoint);
	printf("There are %u program headers, starting at offset %lu\n", iphdr->phdr_count, iphdr->phdr_offset);
}

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

static bool line_start(Elf64_Xword index) { return index % 16 == 0; }
static bool line_end(Elf64_Xword index) { return (index + 1) % 16 == 0; }
static bool size_reached_before_line_end(Elf64_Xword index, Elf64_Xword size) { return index == (size - 1) && !line_end(index); }

void print_xphdr(const s_xphdr *xphdr)
{
	char ascii[17] = {0};
	Elf64_Word tsz = xphdr->fsize_val;

	printf("\n%s--- Printing executable text segment of size %u at offset %ld ---%s\n",
		   INFO, tsz, xphdr->hdr_offset, RESET);
	printf("\n");

	size_t sz = (xphdr->cave_lenght + *(xphdr->fsizse_addr));
	printf("Size: %ld\n", sz);
	for (Elf64_Xword i = 0; i < sz; i++)
	{
		if (line_start(i))
			printf("%p: ", (void *)(xphdr->v_addr + i));
		printf("%02x", xphdr->encrypted_data[i]);
		if (i % 2)
			printf(" ");
		ascii[i % 16] = xphdr->encrypted_data[i] >= ' ' && xphdr->encrypted_data[i] < 127 ? xphdr->encrypted_data[i] : '.';
		if (size_reached_before_line_end(i, sz))
			while (!line_end(i))
			{
				printf("   ");
				i++;
			}
		if (line_end(i))
		{
			printf("	%s\n", ascii);
			ft_memset(ascii, 0, 17);
		}
	}
	printf("\n");
}

void print_xphdr_struct(const s_xphdr *hdr)
{
	printf("Txt Offset: %lu\n\
Txt Vadress:	0x%lx\n\
Txt Lenght:	%lu\n\
Txt Size:	%lu	TxtSize Address		%p\n\
MemSize:	%lu	MemSize Address		%p\n\
Cave Offset	%lu\n\
Cave Lenght:	%lu\n",
		   hdr->hdr_offset, hdr->v_addr,
		   hdr->fsize_val,
		   *(hdr->fsizse_addr), hdr->fsizse_addr,
		   *(hdr->mem_size_addr), hdr->mem_size_addr,
		   hdr->cave_offset, hdr->cave_lenght);
}
