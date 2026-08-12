NAME := woody_woodpacker
BUILD_DIR := .dir_build/
SDIR:= src/
HEADERS := includes/

CC := gcc
CFLAGS := -Werror -Wextra -Wall -g3 
CPPFLAGS = -MMD -MP -I $(HEADERS)
MAKEFLAGS += --no-print-directory

SRC +=	main.c \
		error_handlers.c \
		printers.c \
		cipher.c

OBJS:= $(addprefix $(BUILD_DIR),$(SRC:%.c=%.o))
DEPS:= $(OBJS:%.o=%.d)

RM := rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	
$(BUILD_DIR)%.o: $(SDIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

 -include $(DEPS)

clean:
	$(RM) $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME)
	
re: fclean all

# FORCE : 

.PHONY : clean fclean all re FORCE
