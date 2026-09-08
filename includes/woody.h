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
	Elf64_Addr v_addr;
	Elf64_Off cave_offset;
	Elf64_Off hdr_offset;
	Elf64_Xword *fsizse_addr;
	Elf64_Xword *mem_size_addr;
	Elf64_Xword cave_lenght;
	Elf64_Xword fsize_val;
	unsigned char *encrypted_data;
} s_xphdr;

typedef struct pt_load_range
{
	bool is_executable;
	Elf64_Addr vaddr_end;
	Elf64_Addr vaddr_start;
	Elf64_Off offset;
} s_pt_load_range;

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
	unsigned char key[16];
} s_bin_exec_seg;

typedef struct dt_rela
{
	Elf64_Xword entry_count;
	Elf64_Xword sz;
	Elf64_Addr vaddr;
	Elf64_Off offset;
} s_rela_arr;

typedef struct dyn_info
{
	s_xphdr dynhdr;
	size_t pt_load_count;
	s_pt_load_range *pt_loads;
} s_dyn_info;

// logging
bool _perror(const char *error);
bool _psuccess(const char *mess);

// printers
void _plog(const char *mess);
void print_ehdr(const char *ftype, const char *fclass, const Elf64_Addr entrypoint, const s_pdhr_info *iphdr);
void print_phdr(const Elf64_Phdr *phdr, const int i);
void print_xphdr(const s_xphdr *xphdr);
void print_xphdr_struct(const s_xphdr *hdr);
void print_pt_load_ranges(const s_dyn_info *i_dyn);
const char *dtag_value(Elf64_Sxword tag);
void print_pt_load_range(const s_pt_load_range *range);

// validation
bool is_safe_offset(const s_bin_exec_seg *exec_seg);
bool validate_format(Elf64_Ehdr *ehdr, s_bin_exec_seg *exec_seg, s_pdhr_info *info);

// header recovery
bool get_dynamic_and_executable_headers(Elf64_Phdr *filemap, const s_pdhr_info *hdr_info,
										s_bin_exec_seg *exec_seg, s_dyn_info *dyn_info);
// header parsing
size_t compute_cave_lenght(Elf64_Xword txt_size);
bool is_safe_cave(size_t self, Elf64_Phdr *filemap, const s_pdhr_info *info, s_bin_exec_seg *exec_seg);
void set_encryption_data(s_bin_exec_seg *exec_seg, Elf64_Phdr phdr, const s_pdhr_info *info);
size_t get_pt_load_count(Elf64_Phdr *filemap, s_pdhr_info *info);
bool init_rela_info(const s_dyn_info *dyn_info, s_rela_arr *rela_arr, Elf64_Dyn *dyn);
bool validate_init_array(Elf64_Rela *rela_arr, const s_dyn_info *pt_loads, s_rela_arr *init_arr_info);

// header modification
bool insert_stub(void *file_map, s_bin_exec_seg *exec_seg);

// cipher
void encrypt_text(unsigned char *key, unsigned char *text, Elf64_Xword text_size);
bool create_cipher_key(unsigned char *key);