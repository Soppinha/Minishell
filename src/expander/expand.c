/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:10:49 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:15:10 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_argv(t_cmd *cmd, t_shell *sh)
{
	t_list	*all;
	t_list	*words;
	char	**argv;
	int		i;

	all = NULL;
	i = 0;
	while (cmd->argv[i] && expand_word(cmd->argv[i], sh, &words) == 0)
	{
		ft_lstadd_back(&all, words);
		i++;
	}
	argv = NULL;
	if (!cmd->argv[i])
		argv = (char **)ft_lst_to_array(all);
	if (!argv)
	{
		ft_lstclear(&all, free);
		return (EXIT_FAILURE_GEN);
	}
	ft_free_array(cmd->argv);
	cmd->argv = argv;
	return (EXIT_OK);
}

static int	expand_redir_file(t_redir *r, t_shell *sh)
{
	t_list	*words;
	char	*new;

	if (r->type == HEREDOC)
		return (EXIT_OK);
	if (expand_word(r->file, sh, &words) < 0)
		return (EXIT_FAILURE_GEN);
	if (!words || words->next)
	{
		ft_lstclear(&words, free);
		return (ms_error(r->file, "ambiguous redirect", EXIT_FAILURE_GEN));
	}
	new = words->content;
	free(words);
	free(r->file);
	r->file = new;
	return (EXIT_OK);
}

int	expand_redirs(t_redir *redirs, t_shell *sh)
{
	int	status;

	while (redirs)
	{
		status = expand_redir_file(redirs, sh);
		if (status != EXIT_OK)
			return (status);
		redirs = redirs->next;
	}
	return (EXIT_OK);
}

int	expand_cmd(t_cmd *cmd, t_shell *sh)
{
	int	status;

	if (!cmd)
		return (EXIT_OK);
	status = expand_argv(cmd, sh);
	if (status == EXIT_OK)
		status = expand_redirs(cmd->redirs, sh);
	return (status);
}
