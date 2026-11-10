/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 01:30:00 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 01:30:00 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_error(char *ctx, char *msg, int code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (ctx)
	{
		ft_putstr_fd(ctx, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(msg, STDERR_FILENO);
	return (code);
}

int	status_from_wait(int wstatus)
{
	if (WIFSIGNALED(wstatus))
		return (EXIT_SIG_BASE + WTERMSIG(wstatus));
	return (WEXITSTATUS(wstatus));
}

void	restore_fds(t_shell *sh)
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if (sh->saved_fd[i] >= 0)
		{
			dup2(sh->saved_fd[i], i);
			close(sh->saved_fd[i]);
			sh->saved_fd[i] = -1;
		}
		i++;
	}
}

void	shell_exit(t_shell *sh, int code)
{
	restore_fds(sh);
	free_env(sh->env);
	sh->env = NULL;
	free_tokens(sh->tokens);
	sh->tokens = NULL;
	free_ast(sh->ast);
	sh->ast = NULL;
	rl_clear_history();
	exit(code);
}
