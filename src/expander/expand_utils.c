/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:10:49 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:12:13 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_str(t_exp *e, char *s)
{
	char	*tmp;

	if (!s || !*s)
		return (0);
	if (!e->buf)
		tmp = ft_strdup(s);
	else
		tmp = ft_strjoin(e->buf, s);
	if (!tmp)
		return (-1);
	free(e->buf);
	e->buf = tmp;
	e->has_word = 1;
	return (0);
}

int	append_char(t_exp *e, char c)
{
	char	s[2];

	s[0] = c;
	s[1] = '\0';
	return (append_str(e, s));
}

int	flush_word(t_exp *e)
{
	t_list	*node;

	if (!e->has_word)
		return (0);
	if (!e->buf)
		e->buf = ft_strdup("");
	if (!e->buf)
		return (-1);
	if (ft_strchr(e->buf, WILD_MARK))
		return (glob_flush(e));
	node = ft_lstnew(e->buf);
	if (!node)
		return (-1);
	ft_lstadd_back(&e->words, node);
	e->buf = NULL;
	e->has_word = 0;
	return (0);
}

char	*remove_quotes(char *s)
{
	char	*res;
	char	quote;
	int		i;
	int		j;

	res = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (!res)
		return (NULL);
	quote = 0;
	i = 0;
	j = 0;
	while (s[i])
	{
		if (!quote && (s[i] == '\'' || s[i] == '"'))
			quote = s[i];
		else if (quote && s[i] == quote)
			quote = 0;
		else
			res[j++] = s[i];
		i++;
	}
	return (res);
}

char	glob_mark(char c, char quote)
{
	if (c == '*' && !quote)
		return (WILD_MARK);
	return (c);
}
