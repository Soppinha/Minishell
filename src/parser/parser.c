/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 01:29:47 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:05:46 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ast	*parse_pipeline(t_token **cur)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_command(cur);
	while (left && *cur && (*cur)->type == PIPE)
	{
		*cur = (*cur)->next;
		right = parse_command(cur);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = new_ast(N_PIPE, left, right);
	}
	return (left);
}

static t_ast	*parse_and_or(t_token **cur)
{
	t_ast	*left;
	t_ast	*right;
	t_node	type;

	left = parse_pipeline(cur);
	while (left && *cur && ((*cur)->type == AND || (*cur)->type == OR))
	{
		type = N_OR;
		if ((*cur)->type == AND)
			type = N_AND;
		*cur = (*cur)->next;
		right = parse_pipeline(cur);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = new_ast(type, left, right);
	}
	return (left);
}

t_ast	*parse_subshell(t_token **cur)
{
	t_ast	*inner;
	t_ast	*node;

	*cur = (*cur)->next;
	inner = parse_and_or(cur);
	if (!inner)
		return (NULL);
	if (!*cur || (*cur)->type != RPAREN)
	{
		syntax_error(*cur);
		free_ast(inner);
		return (NULL);
	}
	*cur = (*cur)->next;
	node = new_ast(N_SUBSHELL, inner, NULL);
	while (node && *cur && is_redir((*cur)->type))
	{
		if (parse_redir(cur, &node->redirs) < 0)
		{
			free_ast(node);
			return (NULL);
		}
	}
	return (node);
}

void	syntax_error(t_token *tok)
{
	ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
	ft_putstr_fd("near unexpected token `", STDERR_FILENO);
	if (tok)
		ft_putstr_fd(tok->value, STDERR_FILENO);
	else
		ft_putstr_fd("newline", STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

int	parser(t_token *tokens, t_ast **ast)
{
	t_token	*cur;

	*ast = NULL;
	if (!tokens)
		return (EXIT_OK);
	cur = tokens;
	*ast = parse_and_or(&cur);
	if (*ast && cur)
	{
		syntax_error(cur);
		free_ast(*ast);
		*ast = NULL;
	}
	if (!*ast)
		return (EXIT_SYNTAX);
	return (EXIT_OK);
}
