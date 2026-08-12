#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <elf.h>
#include <stdbool.h>

#define GREEN "\033[48;2;124;204;87m"
#define INFO "\033[48;2;228;208;10m"
#define RED "\033[48;2;205;41;73m"
#define RESET "\033[0m"

// a simple struct to store our binary informations across the project
typedef struct bin_info
{
	/*
		Original entrypoint of the given binary.
		We will use it at the end of the stub to launch the regular execution
	*/
	Elf64_Addr original_entrypoint;
	/*
		The program header offset. 
		Starting from the beginning of the header (offset 0), 
		we need to jump `program_hdr_offset` bytes to find the program header
	*/
	Elf64_Off program_hdr_offset;
	/*
		The total number of program headers. 
		We need it to loop on each of them.
	*/
	Elf64_Half program_hdr_count;
} s_bin_info;

// logging
bool _perror(char *error);
bool _psuccess(char *mess);

// printers
void pehdr(Elf64_Ehdr *upckbin);
void _plog(char *mess);
void print_ascii(char *str);

// parsing
bool is_valid_magic(const unsigned char *ident);
bool is_valid_format(const int ei_class);
bool is_valid_machine(const int e_machine);
