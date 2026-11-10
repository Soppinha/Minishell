/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:17:48 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:19:49 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*search_in_path(char *cmd, char *path)
{
	char	**dirs;
	char	*full;
	int		i;

	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	full = NULL;
	i = 0;
	while (dirs[i])
	{
		full = ft_strjoin3(dirs[i], "/", cmd);
		if (!full || access(full, X_OK) == 0)
			break ;
		free(full);
		full = NULL;
		i++;
	}
	ft_free_array(dirs);
	return (full);
}

char	*find_path(char *cmd, t_env *env)
{
	char	*path;

	if (!cmd[0] || !ft_strcmp(cmd, ".") || !ft_strcmp(cmd, ".."))
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path = get_env(env, "PATH");
	if (!path || !path[0])
		return (ft_strdup(cmd));
	return (search_in_path(cmd, path));
}
