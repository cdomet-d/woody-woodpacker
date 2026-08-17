NAME:= woody_woodpacker
HEADERS:= -I includes/ -I libft/
LIB:=libft/
INCLUDE:= -L $(LIB) -lft

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

$(NAME): $(LIB)libft.a $(OBJS) $(BUILD_DIR)stub.o
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)  $(INCLUDE)
	
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

$(BUILD_DIR)stub.o: $(SRC_DIR)stub.nasm
	nasm -f elf64 $(SRC_DIR)stub.nasm -o $(BUILD_DIR)stub.o

FORCE : 

.PHONY : clean fclean all re FORCE
