/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 00:50:00 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:31:08 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

# define EXIT_OK			0
# define EXIT_FAILURE_GEN	1
# define EXIT_SYNTAX		2
# define EXIT_NOT_EXEC		126
# define EXIT_NOT_FOUND		127
# define EXIT_SIG_BASE		128

# define WILD_MARK		'\x1f'

# define MODE_PROMPT		0
# define MODE_EXEC		1
# define MODE_CHILD		2
# define MODE_HEREDOC	3

extern volatile sig_atomic_t	g_signal;

typedef enum e_tok
{
	WORD,
	PIPE,
	IN,
	OUT,
	APPEND,
	HEREDOC,
	AND,
	OR,
	LPAREN,
	RPAREN
}	t_tok;

typedef enum e_node
{
	N_CMD,
	N_PIPE,
	N_AND,
	N_OR,
	N_SUBSHELL
}	t_node;

typedef struct s_token
{
	char			*value;
	t_tok			type;
	struct s_token	*next;
}	t_token;

typedef struct s_op
{
	const char	*str;
	t_tok		type;
}	t_op;

typedef struct s_redir
{
	t_tok			type;
	char			*file;
	int				quoted;
	int				fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
}	t_cmd;

typedef struct s_ast
{
	t_node			type;
	t_cmd			*cmd;
	t_redir			*redirs;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env			*env;
	t_ast			*ast;
	t_token			*tokens;
	int				last_status;
	int				saved_fd[2];
}	t_shell;

typedef struct s_exp
{
	char			*buf;
	int				has_word;
	t_list			*words;
	t_shell			*sh;
}	t_exp;

typedef int						(*t_bi_fn)(char **argv, t_shell *sh);

typedef struct s_builtin
{
	char			*name;
	t_bi_fn			fn;
}	t_builtin;

typedef int						(*t_exec_fn)(t_ast *node, t_shell *sh);

typedef struct s_sigmode
{
	void			(*on_int)(int);
	void			(*on_quit)(int);
	int				(*hook)(void);
}	t_sigmode;

typedef struct s_redir_op
{
	t_tok			type;
	int				flags;
	int				target;
}	t_redir_op;

void	shell_loop(t_shell *sh);

char	*read_input(t_env *env);

int		line_is_empty(char *line);
int		is_last_history(char *line, char **last);
void	load_history(void);
void	save_history(char *line);

t_env	*env_init(char **envp);
t_env	*env_new_node(char *key, char *value);
char	*get_env(t_env *env, char *key);
int		set_env(t_env **env, char *key, char *value);
void	unset_env(t_env **env, char *key);
char	**env_to_array(t_env *env);
void	env_free_node(t_env *node);
void	free_env(t_env *env);
int		env_size(t_env *env);

int		lexer(char *line, t_token **tokens);
int		is_space(char c);
int		lexer_word(char *line, int i, t_token **tokens);
int		is_operator(char *s);
int		lexer_operator(char *line, int i, t_token **tokens);
int		push_token(t_token **tokens, char *value, t_tok type);
void	free_tokens(t_token *tokens);

int		parser(t_token *tokens, t_ast **ast);
t_ast	*parse_subshell(t_token **cur);
void	syntax_error(t_token *tok);
int		is_redir(t_tok type);
int		parse_redir(t_token **cur, t_redir **redirs);
t_ast	*parse_command(t_token **cur);
t_ast	*new_ast(t_node type, t_ast *left, t_ast *right);
t_redir	*new_redir(t_tok type, char *word);
void	free_ast(t_ast *ast);

int		ms_error(char *ctx, char *msg, int code);
int		status_from_wait(int wstatus);
void	shell_exit(t_shell *sh, int code);
void	restore_fds(t_shell *sh);

int		expand_cmd(t_cmd *cmd, t_shell *sh);
int		expand_redirs(t_redir *redirs, t_shell *sh);
int		expand_word(char *word, t_shell *sh, t_list **out);
int		expand_var(char *s, t_exp *e, int in_dquote);
int		append_char(t_exp *e, char c);
int		append_str(t_exp *e, char *s);
int		flush_word(t_exp *e);
char	*remove_quotes(char *s);
char	glob_mark(char c, char quote);
int		glob_flush(t_exp *e);

int		exec_ast(t_ast *node, t_shell *sh);
int		wait_child(pid_t pid);
void	child_exec(t_cmd *cmd, t_shell *sh);
char	*find_path(char *cmd, t_env *env);
int		apply_redirs(t_redir *r);
int		exec_pipe(t_ast *node, t_shell *sh);
int		exec_subshell(t_ast *node, t_shell *sh);

int		is_builtin(char *name);
int		run_builtin(char **argv, t_shell *sh);
int		bi_echo(char **argv, t_shell *sh);
int		bi_pwd(char **argv, t_shell *sh);
int		bi_env(char **argv, t_shell *sh);
int		bi_cd(char **argv, t_shell *sh);
int		is_valid_identifier(char *s);
int		bi_export(char **argv, t_shell *sh);
int		print_export(t_env *env);
int		ident_error(char *cmd, char *arg);
int		bi_unset(char **argv, t_shell *sh);
int		bi_exit(char **argv, t_shell *sh);

int		collect_heredocs(t_ast *node, t_shell *sh);
int		read_heredoc(t_redir *r, t_shell *sh);
char	*expand_heredoc_line(char *line, t_shell *sh);

void	set_signals(int mode);

#endif
