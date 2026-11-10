/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 21:07:58 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/09 21:07:59 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	build_prompt_str(char *prompt, int len, char *user, char *path)
{
	ft_strlcpy(prompt, "\001\033[1;32m\002", len);
	ft_strlcat(prompt, user, len);
	ft_strlcat(prompt, "\001\033[0m\002:\001\033[1;34m\002", len);
	ft_strlcat(prompt, path, len);
	ft_strlcat(prompt, "\001\033[0m\002$ ", len);
}

static char	*get_display_path(char *cwd, t_env *env)
{
	char	*home;
	char	*path;
	int		len;

	home = get_env(env, "HOME");
	len = 0;
	if (home)
		len = ft_strlen(home);
	if (home && len > 0 && ft_strncmp(cwd, home, len) == 0
		&& (cwd[len] == '/' || cwd[len] == '\0'))
		return (ft_strjoin("~", cwd + len));
	path = ft_strdup(cwd);
	return (path);
}

static char	*build_prompt(t_env *env)
{
	char	cwd[PATH_MAX];
	char	*user;
	char	*display_path;
	char	*prompt;
	int		len;

	user = get_env(env, "USER");
	if (!user)
		user = "user";
	if (!getcwd(cwd, PATH_MAX))
		ft_strlcpy(cwd, "?", 2);
	display_path = get_display_path(cwd, env);
	if (!display_path)
		return (NULL);
	len = ft_strlen(user) + ft_strlen(display_path) + 40;
	prompt = malloc(len);
	if (!prompt)
	{
		free(display_path);
		return (NULL);
	}
	build_prompt_str(prompt, len, user, display_path);
	free(display_path);
	return (prompt);
}

char	*read_input(t_env *env)
{
	char	*prompt;
	char	*line;

	if (!isatty(STDIN_FILENO))
		return (ft_gnl_trim(STDIN_FILENO));
	prompt = build_prompt(env);
	line = readline(prompt);
	free(prompt);
	return (line);
}
