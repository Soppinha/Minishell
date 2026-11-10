/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo_pwd_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:24:43 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:26:35 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_n_flag(char *s)
{
	int	i;

	if (s[0] != '-' || s[1] != 'n')
		return (0);
	i = 1;
	while (s[i] == 'n')
		i++;
	return (s[i] == '\0');
}

int	bi_echo(char **argv, t_shell *sh)
{
	int	i;
	int	newline;

	(void)sh;
	i = 1;
	newline = 1;
	while (argv[i] && is_n_flag(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_OK);
}

int	bi_pwd(char **argv, t_shell *sh)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	(void)argv;
	if (getcwd(cwd, PATH_MAX))
		pwd = cwd;
	else
		pwd = get_env(sh->env, "PWD");
	if (!pwd)
		return (ms_error("pwd", strerror(errno), EXIT_FAILURE_GEN));
	ft_putendl_fd(pwd, STDOUT_FILENO);
	return (EXIT_OK);
}

int	bi_env(char **argv, t_shell *sh)
{
	t_env	*cur;

	if (argv[1])
		return (ms_error("env", "too many arguments", EXIT_FAILURE_GEN));
	cur = sh->env;
	while (cur)
	{
		if (cur->value)
		{
			ft_putstr_fd(cur->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putendl_fd(cur->value, STDOUT_FILENO);
		}
		cur = cur->next;
	}
	return (EXIT_OK);
}
