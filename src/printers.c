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

const char *dtag_value(Elf64_Sxword tag)
{
	switch (tag)
	{
	case DT_NULL:
		return "DT_NULL: Marks end of dynamic section";

	case DT_NEEDED:
		return "DT_NEEDED: String table offset to name of a needed library";

	case DT_PLTRELSZ:
		return "DT_PLTRELSZ: Size in bytes of PLT relocation entries";

	case DT_PLTGOT:
		return "DT_PLTGOT: Address of PLT and/or GOT";

	case DT_HASH:
		return "DT_HASH: Address of symbol hash table";

	case DT_STRTAB:
		return "DT_STRTAB: Address of string table";

	case DT_SYMTAB:
		return "DT_SYMTAB: Address of symbol table";

	case DT_RELA:
		return "DT_RELA: Address of Rela relocation table";

	case DT_RELASZ:
		return "DT_RELASZ: Size in bytes of the Rela relocation table";

	case DT_RELAENT:
		return "DT_RELAENT: Size in bytes of a Rela relocation table entry";

	case DT_STRSZ:
		return "DT_STRSZ: Size in bytes of string table";

	case DT_SYMENT:
		return "DT_SYMENT: Size in bytes of a symbol table entry";

	case DT_INIT:
		return "DT_INIT: Address of the initialization function";

	case DT_FINI:
		return "DT_FINI: Address of the termination function";

	case DT_SONAME:
		return "DT_SONAME: String table offset to name of shared object";

	case DT_RPATH:
		return "DT_RPATH: String table offset to library search path (deprecated)";

	case DT_SYMBOLIC:
		return "DT_SYMBOLIC: Start symbol search in this shared object before the executable";

	case DT_REL:
		return "DT_REL: Address of Rel relocation table";

	case DT_RELSZ:
		return "DT_RELSZ: Size in bytes of Rel relocation table";

	case DT_RELENT:
		return "DT_RELENT: Size in bytes of a Rel table entry";

	case DT_PLTREL:
		return "DT_PLTREL: Type of relocation entry used by the PLT (Rel or Rela)";

	case DT_DEBUG:
		return "DT_DEBUG: Reserved for debugging; semantics unspecified";

	case DT_TEXTREL:
		return "DT_TEXTREL: Indicates that relocations may apply to a non-writable segment (typically .text)";

	case DT_JMPREL:
		return "DT_JMPREL: Address of relocation entries associated solely with the PLT";

	case DT_BIND_NOW:
		return "DT_BIND_NOW: Instructs the dynamic linker to process all relocations before transferring control to the executable";

	case DT_INIT_ARRAY:
		return "DT_INIT_ARRAY: Array with addresses of initialization functions";

	case DT_FINI_ARRAY:
		return "DT_FINI_ARRAY: Array with addresses of termination functions";

	case DT_INIT_ARRAYSZ:
		return "DT_INIT_ARRAYSZ: Size in bytes of DT_INIT_ARRAY array";

	case DT_FINI_ARRAYSZ:
		return "DT_FINI_ARRAYSZ: Size in bytes of DT_FINI_ARRAY array";

	case DT_RUNPATH:
		return "DT_RUNPATH: String table offset to library search path";

	case DT_FLAGS:
		return "DT_FLAGS: Flags for the object being loaded";

	case DT_ENCODING:
		return "DT_ENCODING: Start of encoded range of dynamic tags";

	case DT_SYMTAB_SHNDX:
		return "DT_SYMTAB_SHNDX: Address of SYMTAB_SHNDX section (for extended section indices)";

	case DT_NUM:
		return "DT_NUM: Number of standard dynamic tags";

	/* Processor-specific range */
	case DT_LOPROC:
		return "DT_LOPROC: Start of processor-specific dynamic tags";

	case DT_HIPROC:
		return "DT_HIPROC: End of processor-specific dynamic tags";
	/* OS-specific range markers */
	case DT_LOOS:
		return "DT_LOOS: Start of OS-specific dynamic tags";

	case DT_HIOS:
		return "DT_HIOS: End of OS-specific dynamic tags";

	/* DT_VALRNG* – tags using d_un.d_val */
	case DT_VALRNGLO:
		return "DT_VALRNGLO: Start of value-range dynamic tags (use d_un.d_val)";

	case DT_GNU_PRELINKED:
		return "DT_GNU_PRELINKED: Prelinking timestamp";

	case DT_GNU_CONFLICTSZ:
		return "DT_GNU_CONFLICTSZ: Size of conflict section";

	case DT_GNU_LIBLISTSZ:
		return "DT_GNU_LIBLISTSZ: Size of library list";

	case DT_CHECKSUM:
		return "DT_CHECKSUM: Checksum of dynamic section";

	case DT_PLTPADSZ:
		return "DT_PLTPADSZ: Size of PLT padding";

	case DT_MOVEENT:
		return "DT_MOVEENT: Size of one move table entry";

	case DT_MOVESZ:
		return "DT_MOVESZ: Size of move table";

	case DT_FEATURE_1:
		return "DT_FEATURE_1: Feature selection flags (DTF_*)";

	case DT_POSFLAG_1:
		return "DT_POSFLAG_1: Flags affecting the following DT_* entry";

	case DT_SYMINSZ:
		return "DT_SYMINSZ: Size of syminfo table (in bytes)";

	case DT_SYMINENT:
		return "DT_SYMINENT: Entry size of syminfo table";

	/* DT_ADDRRNG* – tags using d_un.d_ptr */
	case DT_ADDRRNGLO:
		return "DT_ADDRRNGLO: Start of address-range dynamic tags (use d_un.d_ptr)";

	case DT_GNU_HASH:
		return "DT_GNU_HASH: Address of GNU-style symbol hash table";

	case DT_TLSDESC_PLT:
		return "DT_TLSDESC_PLT: PLT entry for TLS descriptor resolution";

	case DT_TLSDESC_GOT:
		return "DT_TLSDESC_GOT: GOT entry for TLS descriptor resolution";

	case DT_GNU_CONFLICT:
		return "DT_GNU_CONFLICT: Start of conflict section";

	case DT_GNU_LIBLIST:
		return "DT_GNU_LIBLIST: Library list";

	case DT_CONFIG:
		return "DT_CONFIG: Configuration information";

	case DT_DEPAUDIT:
		return "DT_DEPAUDIT: Dependency auditing";

	case DT_AUDIT:
		return "DT_AUDIT: Object auditing";

	case DT_PLTPAD:
		return "DT_PLTPAD: PLT padding";

	case DT_MOVETAB:
		return "DT_MOVETAB: Move table";

	case DT_SYMINFO:
		return "DT_SYMINFO: Syminfo table";

	/* Versioning-related tags */
	case DT_VERSYM:
		return "DT_VERSYM: Version symbol table";

	case DT_RELACOUNT:
		return "DT_RELACOUNT: Number of Rela relocations";

	case DT_RELCOUNT:
		return "DT_RELCOUNT: Number of Rel relocations";

	case DT_FLAGS_1:
		return "DT_FLAGS_1: State flags (DF_1_*)";

	case DT_VERDEF:
		return "DT_VERDEF: Address of version definition table";

	case DT_VERDEFNUM:
		return "DT_VERDEFNUM: Number of version definitions";

	case DT_VERNEED:
		return "DT_VERNEED: Address of table with needed versions";

	case DT_VERNEEDNUM:
		return "DT_VERNEEDNUM: Number of needed versions";

	/* Processor-specific extensions (Sun-compatible) */
	case DT_AUXILIARY:
		return "DT_AUXILIARY: Shared object to load before this one (auxiliary filter)";

	default:
		/* Generic fallback for OS/processor ranges */
		if (tag >= DT_LOOS && tag <= DT_HIOS)
			return "DT_* (OS-specific): OS-specific dynamic tag";

		if (tag >= DT_LOPROC && tag <= DT_HIPROC)
			return "DT_* (processor-specific): Processor-specific dynamic tag";

		return "DT_UNKNOWN: Unknown dynamic tag";
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

void print_pt_load_ranges(const s_bin_ctx *ctx)
{
	size_t count = ctx->pt_load_count;
	printf("PT_LOAD NB: %ld\n", count);
	for (size_t i = 0; i < count; i++)
	{
		printf("start: 0x%016ld | end: 0x%016ld | executable: %-5s\n",
			   ctx->pt_loads[i].vaddr_start,
			   ctx->pt_loads[i].vaddr_end,
			   ctx->pt_loads[i].is_executable ? "true" : "false");
	}
}

void print_pt_load_range(const s_pt_load_range *range)
{
		printf("0x%016ld->0x%016ld | executable: %-5s\n",
			   range->vaddr_start,
			   range->vaddr_end,
			   range->is_executable ? "true" : "false");
}