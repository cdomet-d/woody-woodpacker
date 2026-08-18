#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <elf.h>
#include <stdbool.h>

#define GREEN "\033[48;2;124;204;87m"
#define INFO "\033[48;2;204;204;255m"
#define RED "\033[48;2;205;41;73m"
#define RESET "\033[0m"
#define STUB_SIZE 20 // UPDATE STUB_SIZE HERE AS IT CHANGES
#define x86_64_PAGE_SZ 4096

/* Info on the program headers, such as the offset to reach them and how many there are
	Members:
	`Elf64_Off phdr_offset;`
	`Elf64_Half phdr_count;`
*/
typedef struct phdr_info
{
	/*  The program header offset.
	Starting from the beginning of the header (offset 0),
	we need to jump `phdr_offset` bytes to find the program headers.
	Holds `e_phoff` */
	Elf64_Off phdr_offset;

	/*The total number of program headers.
	We need it to loop through them and find the executable PT_LOAD.
	Holds `e_phnum` */
	Elf64_Half phdr_count;

} s_pdhr_info;

typedef struct xphdr
{
	/*	The text part offset relative to byte 0 of the file on disk
	It holds the value of `p_offset` */
	Elf64_Off txt_offset;

	/* The virtual adress of the text section. When the program is loaded,
	the kernel will place the text starting at that adress. It can be fixed (with non-PIE executable)
	or must be calculated with an offset if the binary is PIE
	It holds the value of `p_vaddr` */
	Elf64_Addr txt_vaddress;

	/* The size of the text section of the program header.
	It holds the value of `p_filesz` */
	Elf64_Xword txt_size;

	/* Raw .text values for encryption */
	unsigned char *txt_data;

	/* The offset needed to reach the start of the code cave.
	It's equal to txt_offset + txt_size*/
	Elf64_Off cave_offset;

	/* The size of the code cave */
	size_t cave_lenght;
} s_xphdr;

/* a simple struct to store our binary informations across the project
The Variable types (Elf64_...) are typedefs on fixed width types.
It's safer to use those, because the ELF format specifies byte width for every fields.
Using the typedefs protects us from byte lenght mismatches on different architectures.
*/
typedef struct bin_ctx
{
	/* Original entrypoint of the given binary.
	We will use it at the end of the stub to launch the regular execution.
	Holds `e_entry`*/
	Elf64_Addr original_entrypoint;

	/* Holds information on the executable PT_LOAD and the code cave*/
	s_xphdr xphdr;

} s_bin_ctx;

// logging
bool _perror(const char *error);
bool _psuccess(const char *mess);

// printers
void _plog(const char *mess);
void print_ehdr(const char *ftype, const char *fclass, const Elf64_Addr entrypoint, const s_pdhr_info *iphdr);
void print_phdr(const Elf64_Phdr *phdr, const int i);
void print_xphdr(const s_xphdr *xphdr);

// parsing
bool is_valid_magic(const unsigned char *ident);
bool is_valid_format(const int ei_class);
bool is_valid_machine(const int e_machine);
bool validate_format(const Elf64_Ehdr *ehdr, s_bin_ctx *ctx, s_pdhr_info *phdr_info);

// header recovery
bool get_xphdr(const Elf64_Phdr *phdr, const s_pdhr_info *phdr_info, s_bin_ctx *ctx);

// header modification
bool realloc_headers(const Elf64_Ehdr *ehdr, s_bin_ctx *ctx);
