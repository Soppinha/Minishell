/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:33:30 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 16:26:22 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	heredoc_step(char *s, t_exp *e)
{
	if (*s == '$')
		return (expand_var(s, e, 1));
	if (append_char(e, *s) < 0)
		return (-1);
	return (1);
}

char	*expand_heredoc_line(char *line, t_shell *sh)
{
	t_exp	e;
	int		i;
	int		ret;

	ft_bzero(&e, sizeof(t_exp));
	e.sh = sh;
	i = 0;
	ret = 0;
	while (ret >= 0 && line[i])
	{
		ret = heredoc_step(line + i, &e);
		i += ret;
	}
	if (ret < 0)
	{
		free(e.buf);
		return (NULL);
	}
	if (!e.buf)
		return (ft_strdup(""));
	return (e.buf);
}
