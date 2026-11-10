/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 00:49:21 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 01:10:12 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	match_operator(char *s, t_tok *type)
{
	static const t_op	ops[] = {
	{"||", OR}, {"&&", AND}, {"<<", HEREDOC}, {">>", APPEND},
	{"|", PIPE}, {"<", IN}, {">", OUT}, {"(", LPAREN}, {")", RPAREN},
	{NULL, WORD}};
	int					i;
	int					len;

	i = 0;
	while (ops[i].str)
	{
		len = ft_strlen(ops[i].str);
		if (ft_strncmp(s, ops[i].str, len) == 0)
		{
			if (type)
				*type = ops[i].type;
			return (len);
		}
		i++;
	}
	return (0);
}

int	is_operator(char *s)
{
	return (match_operator(s, NULL) > 0);
}

int	lexer_operator(char *line, int i, t_token **tokens)
{
	int		len;
	t_tok	type;

	len = match_operator(line + i, &type);
	if (len == 0)
		return (-1);
	if (push_token(tokens, ft_substr(line, i, len), type) < 0)
		return (-1);
	return (i + len);
}
