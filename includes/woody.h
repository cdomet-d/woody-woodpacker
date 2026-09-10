#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <elf.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include "libft.h"

#define RED "\033[38;2;179;1;30m"
#define GREEN "\033[0;32m"
#define INFO "\033[38;2;255;20;147m"
#define RESET "\033[0m"

#define x86_64_PAGE_SZ 4096
#define KEY_LENGHT 16
#define HEXKEY_LENGHT 33

typedef struct phdr_info
{
	Elf64_Off phdr_offset;
	Elf64_Half phdr_count;
	Elf64_Half phdr_size;
} s_hdr_info;

typedef struct xphdr
{
	Elf64_Addr v_addr;
	Elf64_Off cave_offset;
	Elf64_Off hdr_offset;
	Elf64_Xword *fsizse_addr;
	Elf64_Xword *mem_size_addr;
	Elf64_Xword cave_lenght;
	Elf64_Xword fsize_val;
	unsigned char *encrypted_data;
} s_xphdr;

/* a simple struct to store our binary informations across the project
The Variable types (Elf64_...) are typedefs on fixed width types.
It's safer to use those, because the ELF format specifies byte width for every fields.
Using the typedefs protects us from byte lenght mismatches on different architectures.
*/
typedef struct bin_exec_seg
{
	Elf64_Addr *program_entrypoint;
	Elf64_Addr original_entrypoint;
	s_xphdr xphdr;
	unsigned char key[KEY_LENGHT];
	unsigned char hexkey[HEXKEY_LENGHT];
} s_exec_seg;

// logging
bool _perror(const char *error);
bool _psuccess(const char *mess);

// printers
void _plog(const char *mess);
void print_ehdr(const char *ftype, const char *fclass, const Elf64_Addr entrypoint, const s_hdr_info *iphdr);
void print_phdr(const Elf64_Phdr *phdr, const int i);
void print_xphdr(const s_xphdr *xphdr);
void print_xphdr_struct(const s_xphdr *hdr);

// validation
bool is_safe_offset(const s_exec_seg *exec_seg);
bool validate_format(Elf64_Ehdr *ehdr);

// initialisation
void init_program_info(Elf64_Ehdr *ehdr, s_exec_seg *exec_seg, s_hdr_info *hdr_info);

bool init_exec_seg(Elf64_Phdr *filemap, s_hdr_info *hdr_info, s_exec_seg *exec_seg);

// header parsing
size_t compute_cave_lenght(Elf64_Xword txt_size);
bool is_safe_cave(size_t self, Elf64_Phdr *filemap, const s_hdr_info *info, s_exec_seg *exec_seg);

// header modification
bool insert_stub(void *file_map, s_exec_seg *exec_seg);

// cipher
void encrypt_text(unsigned char *key, unsigned char *text, Elf64_Xword text_size);
bool create_cipher_key(unsigned char *key);
void format_key_to_hex(unsigned char *key, unsigned char *hexkey);
