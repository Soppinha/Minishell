/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:10:49 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:14:20 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_step(char *s, t_exp *e, char *quote)
{
	if (!*quote && (*s == '\'' || *s == '"'))
	{
		*quote = *s;
		e->has_word = 1;
		return (1);
	}
	if (*quote && *s == *quote)
	{
		*quote = 0;
		return (1);
	}
	if (*s == '$' && *quote != '\'')
		return (expand_var(s, e, *quote == '"'));
	if (append_char(e, glob_mark(*s, *quote)) < 0)
		return (-1);
	return (1);
}

int	expand_word(char *word, t_shell *sh, t_list **out)
{
	t_exp	e;
	char	quote;
	int		i;
	int		ret;

	ft_bzero(&e, sizeof(t_exp));
	e.sh = sh;
	quote = 0;
	i = 0;
	ret = 0;
	while (ret >= 0 && word[i])
	{
		ret = expand_step(word + i, &e, &quote);
		i += ret;
	}
	if (ret >= 0)
		ret = flush_word(&e);
	free(e.buf);
	if (ret < 0)
		ft_lstclear(&e.words, free);
	else
		*out = e.words;
	return (ret);
}
