CC		= gcc

# FT_SAN = -fsanitize=address
FT_SAN 	= 
CFLAGS	= -Wall -Wextra -Werror $(FT_SAN)

AR		= ar -rcs
RM		= /bin/rm -rf

NAME_S	= pipex

SRCS_S	=	src/pipex.c

OBJS_S	=  $(SRCS_S:.c=.o)

LIBFT_PATH  = ./libftmod/libft
LIBFT	= $(LIBFT_PATH)/libft.a

all:		$(NAME_S) 

$(NAME_S):	$(LIBFT) $(OBJS_S)
			$(CC) $(CFLAGS) $(LIBFT) $(OBJS_S) -o $(NAME_S)


# bonus:		$(NAME_C)

.c.o:
			$(CC) $(CFLAGS) -c $< -o $(<:.c=.o) 

$(LIBFT):	
			make -C $(LIBFT_PATH) all

clean:
			make -C $(LIBFT_PATH) clean
			$(RM) $(OBJS_S)

fclean:		clean
			make -C $(LIBFT_PATH) fclean
			$(RM) $(NAME_S)

re:			fclean all

.PHONY:		all clean fclean re pipex bonus