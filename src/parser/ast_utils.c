/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 01:32:37 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 01:51:52 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*new_ast(t_node type, t_ast *left, t_ast *right)
{
	t_ast	*ast;

	ast = ft_calloc(1, sizeof(t_ast));
	if (!ast)
	{
		free_ast(left);
		free_ast(right);
		return (NULL);
	}
	ast->type = type;
	ast->left = left;
	ast->right = right;
	return (ast);
}

t_redir	*new_redir(t_tok type, char *word)
{
	t_redir	*redir;

	redir = ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->file = ft_strdup(word);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->type = type;
	redir->fd = -1;
	redir->quoted = (ft_strchr(word, '\'') || ft_strchr(word, '"'));
	return (redir);
}

static void	free_redirs(t_redir *redirs)
{
	t_redir	*next;

	while (redirs)
	{
		next = redirs->next;
		free(redirs->file);
		if (redirs->fd >= 0)
			close(redirs->fd);
		free(redirs);
		redirs = next;
	}
}

static void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	ft_free_array(cmd->argv);
	free_redirs(cmd->redirs);
	free(cmd);
}

void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	free_ast(ast->left);
	free_ast(ast->right);
	free_cmd(ast->cmd);
	free_redirs(ast->redirs);
	free(ast);
}
