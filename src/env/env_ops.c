/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 21:13:34 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 00:40:38 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_update(t_env *node, char *value)
{
	char	*dup;

	if (!value)
		return (0);
	dup = ft_strdup(value);
	if (!dup)
		return (1);
	free(node->value);
	node->value = dup;
	return (0);
}

static t_env	*env_create(char *key, char *value)
{
	char	*k;
	char	*v;
	t_env	*node;

	k = ft_strdup(key);
	v = NULL;
	if (value)
		v = ft_strdup(value);
	node = env_new_node(k, v);
	if (value && !v)
	{
		env_free_node(node);
		return (NULL);
	}
	return (node);
}

char	*get_env(t_env *env, char *key)
{
	if (!key)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	set_env(t_env **env, char *key, char *value)
{
	t_env	*cur;
	t_env	*node;

	if (!env || !key)
		return (1);
	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
			return (env_update(cur, value));
		if (!cur->next)
			break ;
		cur = cur->next;
	}
	node = env_create(key, value);
	if (!node)
		return (1);
	if (!cur)
		*env = node;
	else
		cur->next = node;
	return (0);
}

void	unset_env(t_env **env, char *key)
{
	t_env	*cur;
	t_env	*prev;

	if (!env || !key)
		return ;
	prev = NULL;
	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
			env_free_node(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}
