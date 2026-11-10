/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 21:13:37 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/09 22:14:46 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*env_node_from_str(char *str)
{
	char	*eq;
	char	*key;
	char	*value;
	t_env	*node;

	eq = ft_strchr(str, '=');
	value = NULL;
	if (eq)
	{
		key = ft_substr(str, 0, eq - str);
		value = ft_strdup(eq + 1);
	}
	else
		key = ft_strdup(str);
	node = env_new_node(key, value);
	if (eq && !value)
	{
		env_free_node(node);
		return (NULL);
	}
	return (node);
}

t_env	*env_new_node(char *key, char *value)
{
	t_env	*node;

	node = NULL;
	if (key)
		node = malloc(sizeof(t_env));
	if (!node)
	{
		free(key);
		free(value);
		return (NULL);
	}
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

t_env	*env_init(char **envp)
{
	t_env	*head;
	t_env	*tail;
	t_env	*node;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (envp && envp[i])
	{
		node = env_node_from_str(envp[i]);
		if (!node)
		{
			free_env(head);
			return (NULL);
		}
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
		i++;
	}
	return (head);
}
