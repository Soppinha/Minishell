/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:24:43 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:29:46 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ident_error(char *cmd, char *arg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (EXIT_FAILURE_GEN);
}

int	bi_unset(char **argv, t_shell *sh)
{
	int	status;
	int	i;

	status = EXIT_OK;
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]) || ft_strchr(argv[i], '='))
			status = ident_error("unset", argv[i]);
		else
			unset_env(&sh->env, argv[i]);
		i++;
	}
	return (status);
}
