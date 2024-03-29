NAME		=	ft_ping

CC			=	gcc
FLAGS		=	-Wall -Wextra -Werror

LIBFT_DIR	=	./libft
LIBFT_LIB	=	libft.a

HEADER		=	ft_ping.h
SRC			=	ft_ping.c \
				parser.c \
				socket.c \
				send_v4.c \
				readloop.c \
				proc_v4.c \
				ft_exit.c

OBJ_DIR		=	obj
OBJ			=	$(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))

RM_DIR		=	rm -rf
RM_FILE		=	rm -f

${NAME}		:	${OBJ}
				@make -C ${LIBFT_DIR}/ all
				$(CC) -o $(NAME) $(OBJ) $(FLAGS) ${LIBFT_DIR}/${LIBFT_LIB}
				@echo "\tCompiling...\t" [ $(NAME) ] $(SUCCESS)
									
$(OBJ)		:	| $(OBJ_DIR)

$(OBJ_DIR)	:
				@mkdir -p $(OBJ_DIR)
			
$(OBJ_DIR)/%.o :	%.c $(HEADER) Makefile
					@${CC} $(FLAGS) -o $@ -c $<

#COLORS
C_NO="\033[00m"
C_OK="\033[32m"
C_GOOD="\033[32m"

#DEBUG
SUCCESS		=	$(C_GOOD)SUCCESS$(C_NO)
OK			=	$(C_OK)OK$(C_NO)
			
all			:	$(NAME)

bonus		:	$(NAME)

clean		:
				@${RM_DIR} ${OBJ_DIR}
				@echo "\tCleaning...\t" [ $(NAME) ] $(OK)
		
fclean		:	clean
				@${RM_FILE} $(NAME)
				@echo "\tDeleting...\t" [ $(NAME) ] $(OK)
		
re			:	fclean all

.PHONY: all clean fclean re bonus
