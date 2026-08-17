NAME:= woody_woodpacker
HEADERS:= -I includes/ -I libft/
LIB:=libft/
INCLUDE:= -L $(LIB) -lft
STUB=$(BUILD_DIR)stub.o

BUILD_DIR:= .dir_build/
SRC_DIR:= src/
CC := gcc
CFLAGS := -Werror -Wextra -Wall -g3 
CPPFLAGS = -MMD -MP $(HEADERS)
# MAKEFLAGS += --no-print-directory

SRC +=	main.c \
		logging.c \
		printers.c \
		binary_validation.c

OBJS:= $(addprefix $(BUILD_DIR),$(SRC:%.c=%.o))
DEPS:= $(OBJS:%.o=%.d)


RM := rm -rf

all: lib $(NAME)

$(NAME): $(LIB)libft.a $(OBJS) $(STUB)
	$(CC) $(CFLAGS) $(OBJS) $(STUB) -o $(NAME)  $(INCLUDE)
	
$(BUILD_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

 -include $(DEPS)

lib:
	make -C $(LIB)

clean:
	make -C $(LIB) $@
	$(RM) $(BUILD_DIR)

fclean: clean
	make -C $(LIB) $@
	$(RM) $(NAME)
	
re: fclean all

run: all
	@./$(NAME) utils/64sample

vrun: all 
	valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) utils/64sample
$(STUB): $(SRC_DIR)stub.nasm
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 $(SRC_DIR)stub.nasm $(STUB)

FORCE : 

.PHONY : clean fclean all re FORCE
