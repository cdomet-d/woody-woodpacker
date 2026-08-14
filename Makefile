NAME:= woody_woodpacker
BUILD_DIR:= .dir_build/
HEADERS:= -I includes/ -I libft/
LIB:=libft/
INCLUDE:= -L $(LIB) -lft
DSRC:= src/

CC := gcc
CFLAGS := -Werror -Wextra -Wall -g3 
CPPFLAGS = -MMD -MP $(HEADERS)
MAKEFLAGS += --no-print-directory

SRC +=	main.c \
		logging.c \
		printers.c \
		binary_validation.c

OBJS:= $(addprefix $(BUILD_DIR),$(SRC:%.c=%.o))
DEPS:= $(OBJS:%.o=%.d)

RM := rm -rf

all: lib $(NAME)

$(NAME): $(LIB)libft.a $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)  $(INCLUDE)
	
$(BUILD_DIR)%.o: $(DSRC)%.c
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

FORCE : 

.PHONY : clean fclean all re FORCE
