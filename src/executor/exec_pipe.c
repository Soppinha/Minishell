/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:22:00 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:22:19 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pipe_child(t_ast *node, t_shell *sh, int fd[2], int end)
{
	set_signals(MODE_CHILD);
	dup2(fd[end], end);
	close(fd[0]);
	close(fd[1]);
	shell_exit(sh, exec_ast(node, sh));
}

int	exec_pipe(t_ast *node, t_shell *sh)
{
	int		fd[2];
	pid_t	left;
	pid_t	right;

	if (pipe(fd) < 0)
		return (ms_error("pipe", strerror(errno), EXIT_FAILURE_GEN));
	left = fork();
	if (left == 0)
		pipe_child(node->left, sh, fd, STDOUT_FILENO);
	right = -1;
	if (left > 0)
		right = fork();
	if (right == 0)
		pipe_child(node->right, sh, fd, STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	if (left > 0)
		waitpid(left, NULL, 0);
	if (left < 0 || right < 0)
		return (ms_error("fork", strerror(errno), EXIT_FAILURE_GEN));
	return (wait_child(right));
}

int	exec_subshell(t_ast *node, t_shell *sh)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (ms_error("fork", strerror(errno), EXIT_FAILURE_GEN));
	if (pid == 0)
	{
		set_signals(MODE_CHILD);
		status = expand_redirs(node->redirs, sh);
		if (status == EXIT_OK)
			status = apply_redirs(node->redirs);
		if (status == EXIT_OK)
			status = exec_ast(node->left, sh);
		shell_exit(sh, status);
	}
	return (wait_child(pid));
}
