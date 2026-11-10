/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 00:49:06 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 01:21:40 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	word_len(char *s)
{
	int		len;
	char	quote;

	len = 0;
	while (s[len] && !is_space(s[len]) && !is_operator(s + len))
	{
		if (s[len] == '\'' || s[len] == '"')
		{
			quote = s[len];
			len++;
			while (s[len] && s[len] != quote)
				len++;
			if (!s[len])
				return (-quote);
		}
		len++;
	}
	return (len);
}

static int	unclosed_quote(char quote)
{
	ft_putstr_fd("minishell: unexpected EOF while looking for matching `",
		STDERR_FILENO);
	ft_putchar_fd(quote, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	return (-1);
}

int	lexer_word(char *line, int i, t_token **tokens)
{
	int	len;

	len = word_len(line + i);
	if (len < 0)
		return (unclosed_quote(-len));
	if (push_token(tokens, ft_substr(line, i, len), WORD) < 0)
		return (-1);
	return (i + len);
}
