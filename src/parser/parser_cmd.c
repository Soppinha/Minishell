/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 01:31:50 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:09:10 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir(t_tok type)
{
	return (type == IN || type == OUT || type == APPEND || type == HEREDOC);
}

static int	count_args(t_token *tok)
{
	int	count;

	count = 0;
	while (tok && (tok->type == WORD || is_redir(tok->type)))
	{
		if (tok->type == WORD)
			count++;
		else if (tok->next && tok->next->type == WORD)
			tok = tok->next;
		else
			break ;
		tok = tok->next;
	}
	return (count);
}

static int	fill_cmd(t_token **cur, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (*cur && ((*cur)->type == WORD || is_redir((*cur)->type)))
	{
		if ((*cur)->type == WORD)
		{
			cmd->argv[i] = ft_strdup((*cur)->value);
			if (!cmd->argv[i])
				return (-1);
			i++;
			*cur = (*cur)->next;
		}
		else if (parse_redir(cur, &cmd->redirs) < 0)
			return (-1);
	}
	return (0);
}

int	parse_redir(t_token **cur, t_redir **redirs)
{
	t_token	*file;
	t_redir	*redir;
	t_redir	*last;

	file = (*cur)->next;
	if (!file || file->type != WORD)
	{
		syntax_error(file);
		return (-1);
	}
	redir = new_redir((*cur)->type, file->value);
	if (!redir)
		return (-1);
	if (!*redirs)
		*redirs = redir;
	else
	{
		last = *redirs;
		while (last->next)
			last = last->next;
		last->next = redir;
	}
	*cur = file->next;
	return (0);
}

t_ast	*parse_command(t_token **cur)
{
	t_ast	*ast;

	if (*cur && (*cur)->type == LPAREN)
		return (parse_subshell(cur));
	if (!*cur || ((*cur)->type != WORD && !is_redir((*cur)->type)))
	{
		syntax_error(*cur);
		return (NULL);
	}
	ast = new_ast(N_CMD, NULL, NULL);
	if (!ast)
		return (NULL);
	ast->cmd = ft_calloc(1, sizeof(t_cmd));
	if (ast->cmd)
		ast->cmd->argv = ft_calloc(count_args(*cur) + 1, sizeof(char *));
	if (!ast->cmd || !ast->cmd->argv || fill_cmd(cur, ast->cmd) < 0)
	{
		free_ast(ast);
		return (NULL);
	}
	return (ast);
}
