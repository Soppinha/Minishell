/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:24:43 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:28:21 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_nodes(t_env **arr, int n)
{
	t_env	*tmp;
	int		i;
	int		j;

	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - 1 - i)
		{
			if (ft_strcmp(arr[j]->key, arr[j + 1]->key) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_node(t_env *node)
{
	int	i;

	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	ft_putstr_fd(node->key, STDOUT_FILENO);
	if (node->value)
	{
		ft_putstr_fd("=\"", STDOUT_FILENO);
		i = 0;
		while (node->value[i])
		{
			if (ft_strchr("\"\\$`", node->value[i]))
				ft_putchar_fd('\\', STDOUT_FILENO);
			ft_putchar_fd(node->value[i], STDOUT_FILENO);
			i++;
		}
		ft_putstr_fd("\"", STDOUT_FILENO);
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
}

int	print_export(t_env *env)
{
	t_env	**arr;
	int		n;
	int		i;

	n = env_size(env);
	arr = malloc(sizeof(t_env *) * (n + 1));
	if (!arr)
		return (EXIT_FAILURE_GEN);
	i = 0;
	while (env)
	{
		arr[i++] = env;
		env = env->next;
	}
	sort_nodes(arr, n);
	i = 0;
	while (i < n)
	{
		if (ft_strcmp(arr[i]->key, "_") != 0)
			print_node(arr[i]);
		i++;
	}
	free(arr);
	return (EXIT_OK);
}
