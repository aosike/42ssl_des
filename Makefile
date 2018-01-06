# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agundry <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/01/06 11:55:14 by agundry           #+#    #+#              #
#    Updated: 2018/01/06 13:06:37 by agundry          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ft_ssl

CC		=	gcc

CFLAGS	=	-Wall -Wextra -Werror

SRC		=	ft_base64.c \
			ft_des.c	\
			ft_ssl.c	\

OBJ		=	$(patsubst %.c,%.o,$(SRC))

INC		=	ft_ssl_des.h

$(NAME)	:
			make -C libft/ re
			$(CC) $(CFLAGS) -c $(SRC) -I libft/inc/ $(INC)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -L libft/ -lft

all		:	$(NAME)

clean	:	
			make -C libft/ clean
			rm -f $(OBJ)

fclean	:	
			make -C libft/ fclean
			rm -f $(OBJ) $(NAME)

re		:	fclean all
