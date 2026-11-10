/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:24:43 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:25:49 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_error(char *path)
{
	char	*msg;

	msg = strerror(errno);
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
	return (EXIT_FAILURE_GEN);
}

static int	update_pwd(t_shell *sh, char *oldpwd)
{
	char	cwd[PATH_MAX];

	if (oldpwd && set_env(&sh->env, "OLDPWD", oldpwd) != 0)
		return (EXIT_FAILURE_GEN);
	if (!getcwd(cwd, PATH_MAX))
		return (ms_error("cd", strerror(errno), EXIT_FAILURE_GEN));
	if (set_env(&sh->env, "PWD", cwd) != 0)
		return (EXIT_FAILURE_GEN);
	return (EXIT_OK);
}

int	bi_cd(char **argv, t_shell *sh)
{
	char	oldpwd[PATH_MAX];
	char	*target;
	char	*old;

	if (argv[1] && argv[2])
		return (ms_error("cd", "too many arguments", EXIT_FAILURE_GEN));
	target = argv[1];
	if (!target)
	{
		target = get_env(sh->env, "HOME");
		if (!target)
			return (ms_error("cd", "HOME not set", EXIT_FAILURE_GEN));
	}
	if (!target[0])
		return (EXIT_OK);
	old = NULL;
	if (getcwd(oldpwd, PATH_MAX))
		old = oldpwd;
	if (chdir(target) < 0)
		return (cd_error(target));
	return (update_pwd(sh, old));
}
