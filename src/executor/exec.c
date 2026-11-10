/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:17:48 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:21:40 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_child(pid_t pid)
{
	int	wstatus;
	int	status;

	if (waitpid(pid, &wstatus, 0) < 0)
		return (ms_error("waitpid", strerror(errno), EXIT_FAILURE_GEN));
	status = status_from_wait(wstatus);
	if (status == EXIT_SIG_BASE + SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	else if (status == EXIT_SIG_BASE + SIGQUIT)
		ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
	return (status);
}

static int	run_builtin_parent(t_cmd *cmd, t_shell *sh)
{
	int	status;

	sh->saved_fd[0] = dup(STDIN_FILENO);
	sh->saved_fd[1] = dup(STDOUT_FILENO);
	status = apply_redirs(cmd->redirs);
	if (status == EXIT_OK)
		status = run_builtin(cmd->argv, sh);
	restore_fds(sh);
	return (status);
}

static int	exec_cmd(t_ast *node, t_shell *sh)
{
	pid_t	pid;
	int		status;

	status = expand_cmd(node->cmd, sh);
	if (status != EXIT_OK)
		return (status);
	if (node->cmd->argv[0] && is_builtin(node->cmd->argv[0]))
		return (run_builtin_parent(node->cmd, sh));
	pid = fork();
	if (pid < 0)
		return (ms_error("fork", strerror(errno), EXIT_FAILURE_GEN));
	if (pid == 0)
		child_exec(node->cmd, sh);
	return (wait_child(pid));
}

static int	exec_and_or(t_ast *node, t_shell *sh)
{
	int	status;

	status = exec_ast(node->left, sh);
	sh->last_status = status;
	if ((node->type == N_AND && status == EXIT_OK)
		|| (node->type == N_OR && status != EXIT_OK))
		status = exec_ast(node->right, sh);
	return (status);
}

int	exec_ast(t_ast *node, t_shell *sh)
{
	static const t_exec_fn	table[] = {exec_cmd, exec_pipe, exec_and_or,
		exec_and_or, exec_subshell};

	if (!node)
		return (EXIT_OK);
	return (table[node->type](node, sh));
}
