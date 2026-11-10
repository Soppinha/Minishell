/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:33:30 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 16:27:51 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	collect_redirs(t_redir *r, t_shell *sh)
{
	int	status;

	while (r)
	{
		if (r->type == HEREDOC)
		{
			status = read_heredoc(r, sh);
			if (status != EXIT_OK)
				return (status);
		}
		r = r->next;
	}
	return (EXIT_OK);
}

int	collect_heredocs(t_ast *node, t_shell *sh)
{
	int	status;

	if (!node)
		return (EXIT_OK);
	status = collect_heredocs(node->left, sh);
	if (status == EXIT_OK)
		status = collect_heredocs(node->right, sh);
	if (status == EXIT_OK && node->cmd)
		status = collect_redirs(node->cmd->redirs, sh);
	if (status == EXIT_OK)
		status = collect_redirs(node->redirs, sh);
	return (status);
}
