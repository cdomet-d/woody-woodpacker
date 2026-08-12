/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdomet-d <cdomet-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 15:20:53 by cdomet-d          #+#    #+#             */
/*   Updated: 2024/10/31 14:38:36 by cdomet-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h> // for malloc
# include <unistd.h> // for write
# include <stdint.h> // for SIZE_MAX
# include <stdarg.h> // for variadic functions handling
# include <limits.h> // NULL, MAX_INT, etc...
# include <stdbool.h>

/* GNL */

# define MAX_FD	1024
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

/* PRINT_F */
# define LOWER_HEXA "0123456789abcdef"
# define UPPER_HEXA "0123456789ABCDEF"
# define DEC "0123456789"

# define DEL 127

typedef unsigned long long int ULLONG;

// Double array iterators
typedef struct s_co
{
	ssize_t	i;
	ssize_t	j;
}	t_co;

/* Simple chained list*/
typedef struct s_list
{
	void			*data;
	struct s_list	*next;
}	t_list;

/* utilities */
// allocates and returns a trimmed copy of s1, excluding the "set" characters
char	*ft_strtrim(char const *s1, char const *set);
// allocates and returns a copy of s, starting at index start
char	*ft_substr(char const *s, unsigned int start, size_t len);
// allocates and returns a concatenation of s1 and s2
char	*ft_strjoin(char *s1, char *s2);
// allocates and returns a NULL terminated **char separating *s by c.
char	**ft_split(char const *s, char c);
// allocates and return a string representing n
char	*ft_itoa(int n);
// applies f to each character in s. Return an allocated string resulting from
// modifications
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
// applies f to each character in s
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
// free char **
void	free_darr(char **darr);

/* libc like */
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strrchr(const char *s, int c);
int		ft_atoi(char *nptr);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit_negative(int c);
int		ft_isdigit(int c);
int		ft_isprint(int c);
int		ft_lstsize(t_list *lst);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_tolower(int c);
int		ft_toupper(int c);
size_t	ft_arrlen(char **arr);
size_t	ft_isspace(char c);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);
ssize_t	ft_putchar_fd(char c, int fd);
ssize_t	ft_putendl_fd(char *s, int fd);
ssize_t	ft_putstr_fd(char *s, int fd);
ULLONG	ft_atoll(char *nptr);
void	*ft_calloc(size_t nmemb, size_t size);
void	*ft_memchr(const void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	*perr(int error_code, char *error_message);
void	ft_bzero(void *s, size_t n);
void	ft_putnbr_fd(int n, int fd);
void	print_darr(char **arr, bool verbose);

/* ft_printf */
int		ft_printf(const char *str, ...) __attribute__ ((format (printf, 1, 2)));
ssize_t	ft_p_putnbr(size_t nbr, int *count);
ssize_t	ft_parsef(const char *s, va_list arg_list, size_t *count, ssize_t *len);
ssize_t	ft_putnbr_base(int long nbr, char *base, int *count);
ssize_t	ft_u_putnbr_base(unsigned int nbr, char *base, int *count);
ssize_t	printc(va_list arg_list);
ssize_t	printd(va_list arg_list, char flag);
ssize_t	printp(va_list arg_list);
ssize_t	printpercent(void);
ssize_t	printstr(va_list arg_list);
ssize_t	printx(va_list arg_list, char flag);
ssize_t	read_str(char flag, va_list arg_list);

/* get_next_line */
// char	*ft_buffer_alloc(void);
char	*ft_free(char *s1, char *s2);
char	*ft_join_buff(char *buff, char *read);
char	*ft_read(char *buff, int *check, int fd);
char	*ft_split_buff(char *buff, char *leftover);
char	*get_next_line(int fd);
ssize_t	ft_len(const char *s, char c);

/* Chained Lists */
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstfree(t_list **lst);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list	*ft_lstnew(void *content);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstiter(t_list *lst, void (*f)(void *));

#endif
