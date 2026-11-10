/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_glob.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 19:00:00 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 19:00:00 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	match(char *pat, char *name)
{
	if (*pat == '\0')
		return (*name == '\0');
	if (*pat == WILD_MARK)
		return (match(pat + 1, name) || (*name && match(pat, name + 1)));
	return (*pat == *name && match(pat + 1, name + 1));
}

static int	is_candidate(char *pat, char *name)
{
	if (name[0] == '.' && pat[0] != '.')
		return (0);
	if (!ft_strcmp(name, ".") || !ft_strcmp(name, ".."))
		return (0);
	return (match(pat, name));
}

static int	add_match(t_list **out, char *name)
{
	t_list	**cur;
	t_list	*node;
	char	*dup;

	dup = ft_strdup(name);
	node = NULL;
	if (dup)
		node = ft_lstnew(dup);
	if (!node)
	{
		free(dup);
		return (-1);
	}
	cur = out;
	while (*cur && ft_strcmp((char *)(*cur)->content, name) < 0)
		cur = &(*cur)->next;
	node->next = *cur;
	*cur = node;
	return (0);
}

static int	collect_matches(char *pat, t_list **out)
{
	DIR				*dir;
	struct dirent	*ent;
	int				ret;

	*out = NULL;
	dir = opendir(".");
	if (!dir)
		return (0);
	ret = 0;
	ent = readdir(dir);
	while (ent && ret == 0)
	{
		if (is_candidate(pat, ent->d_name))
			ret = add_match(out, ent->d_name);
		ent = readdir(dir);
	}
	closedir(dir);
	if (ret < 0)
		ft_lstclear(out, free);
	return (ret);
}

int	glob_flush(t_exp *e)
{
	t_list	*matches;
	int		i;

	if (collect_matches(e->buf, &matches) < 0)
		return (-1);
	if (!matches)
	{
		i = 0;
		while (e->buf[i])
		{
			if (e->buf[i] == WILD_MARK)
				e->buf[i] = '*';
			i++;
		}
		matches = ft_lstnew(e->buf);
		if (!matches)
			return (-1);
	}
	else
		free(e->buf);
	ft_lstadd_back(&e->words, matches);
	e->buf = NULL;
	e->has_word = 0;
	return (0);
}
