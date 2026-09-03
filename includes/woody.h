#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <elf.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include "libft.h"

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
	Elf64_Off phdr_offset;
	Elf64_Half phdr_count;
	Elf64_Half phdr_size;
} s_pdhr_info;

typedef struct xphdr
{
	Elf64_Addr txt_vaddress;
	Elf64_Off cave_offset;
	Elf64_Off txt_offset;
	Elf64_Xword *mem_size_addr;
	Elf64_Xword *txt_size_addr;
	Elf64_Xword cave_lenght;
	Elf64_Xword txt_size_val;
	unsigned char *txt_data;
} s_xphdr;

/* a simple struct to store our binary informations across the project
The Variable types (Elf64_...) are typedefs on fixed width types.
It's safer to use those, because the ELF format specifies byte width for every fields.
Using the typedefs protects us from byte lenght mismatches on different architectures.
*/
typedef struct bin_ctx
{
	/* Pointer to e_entry. We update it with the vadr of the stub appended to the executable PT_LOAD
	We will use it at the end of the stub to launch the regular execution.
	Holds `e_entry`*/
	Elf64_Addr *program_entrypoint;
	/* A backup of the original entrypoint; we store it in order to jump back to the original program execution once the stub has run*/
	Elf64_Addr original_entrypoint;
	/* Holds information on the executable PT_LOAD and the code cave*/
	s_xphdr xphdr;

	unsigned char key[16];

} s_bin_ctx;

// logging
bool _perror(const char *error);
bool _psuccess(const char *mess);

// printers
void _plog(const char *mess);
void print_ehdr(const char *ftype, const char *fclass, const Elf64_Addr entrypoint, const s_pdhr_info *iphdr);
void print_phdr(const Elf64_Phdr *phdr, const int i);
void print_xphdr(const s_xphdr *xphdr);
void print_struct(const s_xphdr *hdr);
void hexdump(const s_xphdr *xphdr);

// validation
bool is_safe_offset(const s_bin_ctx *ctx);
bool is_valid_magic(const unsigned char *ident);
bool is_valid_format(const int ei_class);
bool is_valid_machine(const int e_machine);
bool validate_format(Elf64_Ehdr *ehdr, s_bin_ctx *ctx, s_pdhr_info *phdr_info);

// header recovery
bool find_xphdr(Elf64_Phdr *phdr, const s_pdhr_info *phdr_info, s_bin_ctx *ctx);

// header modification
bool insert_stub(void *file_map, s_bin_ctx *ctx);

// cipher
void encrypt_text(unsigned char *key, unsigned char *text, Elf64_Xword text_size);
bool create_cipher_key(unsigned char *key);