/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 21:08:39 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:06:14 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	run_line(t_shell *sh, char *line)
{
	int	status;

	status = lexer(line, &sh->tokens);
	if (status == EXIT_OK)
		status = parser(sh->tokens, &sh->ast);
	free_tokens(sh->tokens);
	sh->tokens = NULL;
	set_signals(MODE_HEREDOC);
	if (status == EXIT_OK)
		status = collect_heredocs(sh->ast, sh);
	set_signals(MODE_EXEC);
	g_signal = 0;
	if (status == EXIT_OK)
		status = exec_ast(sh->ast, sh);
	free_ast(sh->ast);
	sh->ast = NULL;
	return (status);
}

static void	handle_line(t_shell *sh, char *line, char **last)
{
	if (line_is_empty(line))
		return ;
	if (!is_last_history(line, last))
	{
		add_history(line);
		save_history(line);
	}
	sh->last_status = run_line(sh, line);
}

void	shell_loop(t_shell *sh)
{
	char	*line;
	char	*last;

	last = NULL;
	while (1)
	{
		set_signals(MODE_PROMPT);
		line = read_input(sh->env);
		if (g_signal == SIGINT)
		{
			sh->last_status = EXIT_SIG_BASE + SIGINT;
			g_signal = 0;
		}
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				ft_putendl_fd("exit", STDERR_FILENO);
			break ;
		}
		handle_line(sh, line, &last);
		free(line);
	}
	free(last);
}
