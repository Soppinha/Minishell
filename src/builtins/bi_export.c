/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:24:43 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:28:54 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(char *s)
{
	int	i;

	if (!ft_isalpha(s[0]) && s[0] != '_')
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	export_one(char *arg, t_shell *sh)
{
	char	*eq;
	char	*key;
	int		ret;

	if (!is_valid_identifier(arg))
		return (ident_error("export", arg));
	eq = ft_strchr(arg, '=');
	if (!eq)
		return (set_env(&sh->env, arg, NULL));
	key = ft_substr(arg, 0, eq - arg);
	if (!key)
		return (EXIT_FAILURE_GEN);
	ret = set_env(&sh->env, key, eq + 1);
	free(key);
	return (ret);
}

int	bi_export(char **argv, t_shell *sh)
{
	int	status;
	int	i;

	if (!argv[1])
		return (print_export(sh->env));
	status = EXIT_OK;
	i = 1;
	while (argv[i])
	{
		if (export_one(argv[i], sh) != EXIT_OK)
			status = EXIT_FAILURE_GEN;
		i++;
	}
	return (status);
}
