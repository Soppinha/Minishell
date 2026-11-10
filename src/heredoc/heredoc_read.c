/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:33:30 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 16:27:18 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*read_line_hd(void)
{
	if (isatty(STDIN_FILENO))
		return (readline("> "));
	return (ft_gnl_trim(STDIN_FILENO));
}

static int	write_line_hd(int fd, char *line, t_redir *r, t_shell *sh)
{
	char	*out;

	out = line;
	if (!r->quoted)
		out = expand_heredoc_line(line, sh);
	if (!out)
		return (-1);
	ft_putendl_fd(out, fd);
	if (out != line)
		free(out);
	return (0);
}

static int	eof_warning(char *delim)
{
	ft_putstr_fd("minishell: warning: here-document delimited ", 2);
	ft_putstr_fd("by end-of-file (wanted `", 2);
	ft_putstr_fd(delim, 2);
	ft_putendl_fd("')", 2);
	return (0);
}

static int	heredoc_loop(int fd, char *delim, t_redir *r, t_shell *sh)
{
	char	*line;
	int		ret;

	while (1)
	{
		line = read_line_hd();
		if (g_signal == SIGINT)
		{
			free(line);
			return (-2);
		}
		if (!line)
			return (eof_warning(delim));
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			return (0);
		}
		ret = write_line_hd(fd, line, r, sh);
		free(line);
		if (ret < 0)
			return (-1);
	}
}

int	read_heredoc(t_redir *r, t_shell *sh)
{
	int		fd[2];
	char	*delim;
	int		ret;

	delim = remove_quotes(r->file);
	if (!delim)
		return (EXIT_FAILURE_GEN);
	if (pipe(fd) < 0)
	{
		free(delim);
		return (ms_error("pipe", strerror(errno), EXIT_FAILURE_GEN));
	}
	ret = heredoc_loop(fd[1], delim, r, sh);
	free(delim);
	close(fd[1]);
	if (ret < 0)
	{
		close(fd[0]);
		if (ret == -2)
			return (EXIT_SIG_BASE + SIGINT);
		return (EXIT_FAILURE_GEN);
	}
	r->fd = fd[0];
	return (EXIT_OK);
}
