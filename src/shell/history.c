/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 21:08:06 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/09 21:08:07 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	line_is_empty(char *line)
{
	int	i;

	i = 0;
	while (is_space(line[i]))
		i++;
	return (line[i] == '\0');
}

int	is_last_history(char *line, char **last)
{
	if (*last && ft_strcmp(*last, line) == 0)
		return (1);
	free(*last);
	*last = ft_strdup(line);
	return (0);
}

static char	*get_history_path(void)
{
	char	*home;

	home = getenv("HOME");
	if (!home)
		return (NULL);
	return (ft_strjoin(home, "/.minishell_history"));
}

void	load_history(void)
{
	char	*path;
	char	*line;
	int		fd;

	path = get_history_path();
	if (!path)
		return ;
	fd = open(path, O_RDONLY);
	free(path);
	if (fd < 0)
		return ;
	line = ft_gnl_trim(fd);
	while (line)
	{
		if (!line_is_empty(line))
			add_history(line);
		free(line);
		line = ft_gnl_trim(fd);
	}
	close(fd);
}

void	save_history(char *line)
{
	char	*path;
	int		fd;

	if (!line)
		return ;
	path = get_history_path();
	if (!path)
		return ;
	fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0644);
	free(path);
	if (fd < 0)
		return ;
	ft_putendl_fd(line, fd);
	close(fd);
}
