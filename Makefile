NAME := filler

FLAGS := -Wall -Werror -Wextra

LIBS := libft/libft.a

SRC_FILES :=	main.c			\
				parse_map.c		\
				parse_piece.c	\
				placement.c

SRCS := $(addprefix ./src/, $(SRC_FILES))

OBJS := $(SRCS:.c=.o)

INC := .

.PHONY := all clean fclean re

all: $(NAME)

$(NAME):
	@echo "compiling libft..."
	@make -C libft/
	@gcc $(FLAGS) $(SRCS) -I$(INC) $(LIBS) -o $(NAME)
	@echo "Done!"

clean:
	@echo "deleting files..."
	@/bin/rm -f rm $(OFILES)
	@make -C libft/ clean

fclean: clean
	@/bin/rm -f rm $(NAME)
	@make -C libft/ fclean

new:
	@/bin/rm -f rm $(NAME)
	@gcc $(FLAGS) $(SRCS) -I$(INC) $(LIBS) -o $(NAME)
	@echo "Done!"

debug:
	@/bin/rm -f rm $@
	@gcc -g $(FLAGS) $(SRCS) -I$(INC) $(LIBS) -o $@

re: fclean all
