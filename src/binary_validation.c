#include "woody.h"
#include <stdbool.h>
#include "libft.h"

bool is_valid_magic(const unsigned char *ident)
{
	const unsigned char ELF_MAGIC[4] = {0x7F, 'E', 'L', 'F'};
	return ft_memcmp(ident, ELF_MAGIC, 4) == 0;
}

bool is_valid_format(const int ei_class)
{
	return ei_class == ELFCLASS64 ? true : false;
}

bool is_valid_machine(const int e_machine)
{
	return e_machine == EM_X86_64 ? true : false;
}


// unsigned char *extract_binary_text(const int e_machine)
// {
// }