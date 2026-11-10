NAME		=	minishell
CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror
INCLUDES	=	-Iinclude -I$(LIBFT_DIR)/include
LDLIBS		=	-L$(LIBFT_DIR) -lft -lreadline
RM			=	rm -rf

LIBFT_DIR	=	libft
LIBFT		=	$(LIBFT_DIR)/libft.a

SRC_DIR		=	src
OBJ_DIR		=	obj

SRCS_FILES	=	main.c \
				shell/loop.c \
				shell/prompt.c \
				shell/history.c \
				env/env_init.c \
				env/env_ops.c \
				env/env_utils.c \
				lexer/lexer.c \
				lexer/lexer_word.c \
				lexer/lexer_op.c \
				lexer/token_utils.c \
				parser/parser.c \
				parser/parser_cmd.c \
				parser/ast_utils.c \
				expander/expand.c \
				expander/expand_word.c \
				expander/expand_var.c \
				expander/expand_utils.c \
				expander/expand_glob.c \
				executor/exec.c \
				executor/exec_child.c \
				executor/exec_path.c \
				executor/exec_pipe.c \
				executor/exec_redir.c \
				builtins/builtins.c \
				builtins/bi_echo_pwd_env.c \
				builtins/bi_cd.c \
				builtins/bi_export.c \
				builtins/bi_export_print.c \
				builtins/bi_unset.c \
				builtins/bi_exit.c \
				heredoc/heredoc.c \
				heredoc/heredoc_read.c \
				heredoc/heredoc_expand.c \
				signals/signals.c \
				utils/exit_utils.c

SRCS		=	$(addprefix $(SRC_DIR)/, $(SRCS_FILES))
OBJS		=	$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all:		$(NAME)

$(NAME):	$(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)
	@printf "\033[1;34mminishell: done\033[0m\n"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c include/minishell.h
	@mkdir -p $(dir $@)
	@printf "\033[1;32mCompiling \033[1;34m$<\033[0m\n"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory
	@$(RM) $(OBJ_DIR)

fclean:		clean
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory
	@$(RM) $(NAME)

re:			fclean all

bonus:		all

.PHONY:		all clean fclean re bonus
