ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

#########
# Files #
#########	

INCLDS_DIR  =   includes/
INCLUDES    =   malloc.h \
				printf.h \
				utils.h \

SRCS_DIR    =   srcs/
SRCS        =	malloc.c \
				free.c \
				realloc.c \
				show_alloc_mem.c \
				utils/printf.c \
				utils/utils.c \

OBJS_DIR    =   objs/
OBJS        :=  ${addprefix ${OBJS_DIR}, ${SRCS:.c=.o}}


############
# Commands #
############

CC          =   gcc
CFLAGS      =   -Wall -Werror -Wextra -fPIC
LIBFLAGS	=   -shared
NAME        =   libft_malloc_${HOSTTYPE}.so
SYMLINK     =   libft_malloc.so
RM          =   rm -rf

#########
# Rules #
#########

${OBJS_DIR}%.o:   ${SRCS_DIR}%.c $(addprefix ${INCLDS_DIR}, ${INCLUDES}) Makefile
			@mkdir -p $(dir $@)
			$(CC) ${CFLAGS} -I ${INCLDS_DIR} -c $< -o $@ 

all:        ${NAME}

${NAME}:    ${OBJS}
			$(CC) ${CFLAGS} ${LIBFLAGS} -o $(NAME) ${OBJS}
			rm -f $(SYMLINK)
			ln -s $(NAME) $(SYMLINK)

clean:
			${RM} ${OBJS_DIR} a.out

fclean:     clean
			${RM} ${NAME}  ${SYMLINK}

re:         fclean all

.PHONY:     all clean fclean re