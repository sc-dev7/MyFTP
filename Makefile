##
## EPITECH PROJECT, 2022
## Makefile
## File description:
## sc
##

SRC	=	main.c	\
		server.c	\
		run.c	\
		command.c	\
		command_2.c	\

NAME	=	myftp

all:
	gcc	-o	$(NAME)	$(SRC)	-g3

clean:
	rm	-f	*#

fclean:	clean
	rm	-f	$(NAME)

re:	fclean all
