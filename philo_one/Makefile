NAME			= philo_one

SRCS_FILE		= main.c utils.c philosopher.c parsing.c

SRCS			= $(addprefix $(SRCS_DIR), $(SRCS_FILE))
OBJS			= $(addprefix $(OBJS_DIR), $(SRCS_FILE:.c=.o))

SRCS_DIR		= ./srcs/
OBJS_DIR		= ./objs/
INCLUDES_DIR	= ./

HEADER			= $(INCLUDES_DIR)philo_one.h

CFLAGS			= -Wall -Werror -Wextra -fsanitize=address,undefined
CC				= gcc

INCLUDES	= $(addprefix -I, $(INCLUDES_DIR))

# COLORS
_BLACK		= "\033[30m"
_RED		= "\033[31m"
_GREEN		= "\033[32m"
_YELLOW		= "\033[33m"
_BLUE		= "\033[34m"
_VIOLET		= "\033[35m"
_CYAN		= "\033[36m"
_WHITE		= "\033[37m"
_END		= "\033[0m"
_CLEAR		= "\033[2K"
_HIDE_CURS	= "\033[?25l"
_SHOW_CURS	= "\033[?25h"
_UP			= "\033[A"
_CUT		= "\033[k"

.PHONY: all launch clean fclean re norme title bonus
.SILENT:

all: launch

launch:
	echo $(_CLEAR)$(_YELLOW)"building - "$(_GREEN)$(NAME)$(_END)
	$(MAKE) $(NAME)
	echo $(_GREEN)"\nDone."$(_END)$(_SHOW_CURS)

$(OBJS_DIR):
	mkdir $@

FORCE:

$(NAME): $(OBJS_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lpthread -o $(NAME)

$(OBJS): $(OBJS_DIR)%.o: $(SRCS_DIR)%.c $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	printf $<

clean:
	$(RM) -r $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re:
	$(MAKE) -s fclean
	$(MAKE) -s
