/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:24:43 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:30:27 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bi_fn	find_builtin(char *name)
{
	static const t_builtin	table[] = {
	{"echo", bi_echo}, {"cd", bi_cd}, {"pwd", bi_pwd},
	{"export", bi_export}, {"unset", bi_unset}, {"env", bi_env},
	{"exit", bi_exit}, {NULL, NULL}};
	int						i;

	i = 0;
	while (table[i].name)
	{
		if (ft_strcmp(name, table[i].name) == 0)
			return (table[i].fn);
		i++;
	}
	return (NULL);
}

int	is_builtin(char *name)
{
	return (find_builtin(name) != NULL);
}

int	run_builtin(char **argv, t_shell *sh)
{
	t_bi_fn	fn;

	fn = find_builtin(argv[0]);
	if (!fn)
		return (EXIT_NOT_FOUND);
	return (fn(argv, sh));
}
