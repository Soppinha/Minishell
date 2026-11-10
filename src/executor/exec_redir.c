/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:17:48 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:20:33 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_redir(t_redir *r, int *target)
{
	static const t_redir_op	ops[] = {
	{IN, O_RDONLY, STDIN_FILENO},
	{OUT, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO},
	{APPEND, O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO},
	{HEREDOC, 0, STDIN_FILENO}};
	int						i;

	i = 0;
	while (ops[i].type != r->type)
		i++;
	*target = ops[i].target;
	if (r->type == HEREDOC)
		return (r->fd);
	return (open(r->file, ops[i].flags, 0644));
}

int	apply_redirs(t_redir *r)
{
	int	fd;
	int	target;
	int	ret;

	while (r)
	{
		fd = open_redir(r, &target);
		if (fd < 0)
			return (ms_error(r->file, strerror(errno), EXIT_FAILURE_GEN));
		ret = dup2(fd, target);
		close(fd);
		if (r->type == HEREDOC)
			r->fd = -1;
		if (ret < 0)
			return (ms_error("dup2", strerror(errno), EXIT_FAILURE_GEN));
		r = r->next;
	}
	return (EXIT_OK);
}
