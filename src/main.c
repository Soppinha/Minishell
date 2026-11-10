/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 21:08:12 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/09 21:08:13 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;

	(void)argc;
	(void)argv;
	ft_bzero(&sh, sizeof(sh));
	sh.saved_fd[0] = -1;
	sh.saved_fd[1] = -1;
	sh.env = env_init(envp);
	if (!sh.env && envp && envp[0])
		return (ms_error(NULL, "failed to load environment", EXIT_FAILURE_GEN));
	load_history();
	shell_loop(&sh);
	shell_exit(&sh, sh.last_status);
	return (EXIT_OK);
}
