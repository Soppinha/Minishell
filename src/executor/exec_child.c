/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:17:48 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:18:55 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_fail(char *path, char **envp, t_cmd *cmd, t_shell *sh)
{
	struct stat	st;
	char		*msg;
	int			code;

	code = EXIT_NOT_EXEC;
	if (errno == ENOENT)
		code = EXIT_NOT_FOUND;
	msg = strerror(errno);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		msg = "Is a directory";
	ms_error(cmd->argv[0], msg, code);
	free(path);
	ft_free_array(envp);
	shell_exit(sh, code);
}

void	child_exec(t_cmd *cmd, t_shell *sh)
{
	char	*path;
	char	**envp;

	set_signals(MODE_CHILD);
	if (apply_redirs(cmd->redirs) != EXIT_OK)
		shell_exit(sh, EXIT_FAILURE_GEN);
	if (!cmd->argv[0])
		shell_exit(sh, EXIT_OK);
	path = find_path(cmd->argv[0], sh->env);
	if (!path)
		shell_exit(sh, ms_error(cmd->argv[0], "command not found",
				EXIT_NOT_FOUND));
	envp = env_to_array(sh->env);
	if (envp)
		execve(path, cmd->argv, envp);
	exec_fail(path, envp, cmd, sh);
}
