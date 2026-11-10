/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 00:48:55 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 00:51:19 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lexer(char *line, t_token **tokens)
{
	int	i;

	*tokens = NULL;
	i = 0;
	while (i >= 0 && line[i])
	{
		if (is_space(line[i]))
			i++;
		else if (is_operator(line + i))
			i = lexer_operator(line, i, tokens);
		else
			i = lexer_word(line, i, tokens);
	}
	if (i < 0)
	{
		free_tokens(*tokens);
		*tokens = NULL;
		return (EXIT_SYNTAX);
	}
	return (EXIT_OK);
}
