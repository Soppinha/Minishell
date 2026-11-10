/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:10:49 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:13:29 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_split(t_exp *e, char *val)
{
	int	i;

	if (!val)
		return (0);
	i = 0;
	while (val[i])
	{
		if (is_space(val[i]))
		{
			if (flush_word(e) < 0)
				return (-1);
		}
		else if (append_char(e, glob_mark(val[i], 0)) < 0)
			return (-1);
		i++;
	}
	return (0);
}

static int	expand_status(t_exp *e)
{
	char	*num;
	int		ret;

	num = ft_itoa(e->sh->last_status);
	if (!num)
		return (-1);
	ret = append_str(e, num);
	free(num);
	if (ret < 0)
		return (-1);
	return (2);
}

static int	expand_name(char *s, t_exp *e, int in_dquote)
{
	char	*name;
	char	*val;
	int		len;
	int		ret;

	len = 1;
	while (ft_isalnum(s[len]) || s[len] == '_')
		len++;
	name = ft_substr(s, 1, len - 1);
	if (!name)
		return (-1);
	val = get_env(e->sh->env, name);
	free(name);
	if (in_dquote)
		ret = append_str(e, val);
	else
		ret = append_split(e, val);
	if (ret < 0)
		return (-1);
	return (len);
}

int	expand_var(char *s, t_exp *e, int in_dquote)
{
	if (s[1] == '?')
		return (expand_status(e));
	if (ft_isalpha(s[1]) || s[1] == '_')
		return (expand_name(s, e, in_dquote));
	if (ft_isdigit(s[1]))
		return (2);
	if (!in_dquote && (s[1] == '\'' || s[1] == '"'))
		return (1);
	if (append_char(e, '$') < 0)
		return (-1);
	return (1);
}
