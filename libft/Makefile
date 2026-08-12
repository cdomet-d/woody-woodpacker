# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cdomet-d <cdomet-d@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/07 10:23:52 by cdomet-d          #+#    #+#              #
#    Updated: 2024/10/31 14:23:09 by cdomet-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc
CFLAGS = -Werror -Wextra -Wall
CPPFLAGS = -MMD -MP

BUILD_DIR := .lib_build
SRCS := ft_atoi.c \
		ft_atoll.c \
		ft_bzero.c \
		ft_calloc.c \
		free_darr.c \
		ft_isalnum.c \
		ft_isalpha.c \
		ft_isascii.c \
		ft_isdigit_negative.c \
		ft_isdigit.c \
		ft_isprint.c \
		ft_isspace.c \
		ft_itoa.c \
		ft_lstadd_back.c \
		ft_lstclear.c \
		ft_lstdelone.c \
		ft_lstfree.c \
		ft_lstiter.c \
		ft_lstmap.c \
		ft_lstnew.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_memset.c \
		ft_print_chars.c \
		ft_print_nums.c \
		ft_printf.c \
		ft_putchar_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_putnbrs.c \
		ft_putstr_fd.c \
		ft_split.c \
		ft_strchr.c \
		ft_strdup.c \
		ft_striteri.c \
		ft_strjoin.c \
		ft_strlcat.c \
		ft_strlcpy.c \
		ft_strlen.c \
		ft_strmapi.c \
		ft_strncmp.c \
		ft_strnstr.c \
		ft_strrchr.c \
		ft_strtrim.c \
		ft_substr.c \
		ft_tolower.c \
		ft_toupper.c \
		get_next_line_utils.c \
		get_next_line.c \
 		ft_lstadd_front.c \
 		ft_lstlast.c \
 		ft_lstsize.c \
		ft_arrlen.c \
		print_darr.c \
		
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
				
DEPS := $(OBJS:%.o=%.d)
RM := rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(RESET)"
	@echo "$(PINK)$(BOLD) \t • • ┈┈┈┈ ๑ ⋅ ⋯ ୨ \t making libft \t ୧ ⋯ ⋅ ๑ ┈┈┈┈ • •$(RESET)"
	@echo "$(FAINT)"
	ar -rcs $(NAME) $(OBJS)
	@echo "$(RESET)"

$(BUILD_DIR)/%.o:%.c libft.h Makefile
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

 -include $(DEPS)

clean: 
	$(RM) $(BUILD_DIR)
fclean: clean
	$(RM) $(NAME)
re: fclean all

# Colors
BLACK=\033[30m
GREEN=\033[32m
PURPLE=\033[35m
PINK=\033[38;5;206m.

RESET=\033[0m
FAINT=\033[2m
UNDERLINE=\033[4m
BOLD=\033[1m


.PHONY : clean fclean all re BLACK PURPLE GREEN