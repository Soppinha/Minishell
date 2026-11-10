/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 21:13:31 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/09 22:29:24 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_to_array(t_env *env)
{
	char	**arr;
	int		i;

	arr = ft_calloc(env_size(env) + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->value)
		{
			arr[i] = ft_strjoin3(env->key, "=", env->value);
			if (!arr[i])
			{
				ft_free_array(arr);
				return (NULL);
			}
			i++;
		}
		env = env->next;
	}
	return (arr);
}

void	env_free_node(t_env *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}

void	free_env(t_env *env)
{
	t_env	*next;

	while (env)
	{
		next = env->next;
		env_free_node(env);
		env = next;
	}
}

int	env_size(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}
