/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 02:24:43 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 02:27:30 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_code(char *s, int *code)
{
	unsigned long long	n;
	unsigned long long	limit;
	int					i;

	i = 0;
	while (is_space(s[i]))
		i++;
	limit = LLONG_MAX;
	if (s[i] == '-')
		limit = (unsigned long long)LLONG_MAX + 1;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!ft_isdigit(s[i]))
		return (0);
	n = 0;
	while (ft_isdigit(s[i]) && n <= (limit - (s[i] - '0')) / 10)
		n = n * 10 + (s[i++] - '0');
	while (is_space(s[i]))
		i++;
	if (limit != LLONG_MAX)
		n = -n;
	*code = (unsigned char)n;
	return (s[i] == '\0');
}

int	bi_exit(char **argv, t_shell *sh)
{
	int	code;

	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", STDERR_FILENO);
	if (!argv[1])
		shell_exit(sh, sh->last_status);
	if (!parse_code(argv[1], &code))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		shell_exit(sh, EXIT_SYNTAX);
	}
	if (argv[2])
		return (ms_error("exit", "too many arguments", EXIT_FAILURE_GEN));
	shell_exit(sh, code);
	return (code);
}
