/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 17:00:00 by svaladar          #+#    #+#             */
/*   Updated: 2026/11/10 17:00:00 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_prompt(int sig)
{
	g_signal = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	sigint_heredoc(int sig)
{
	g_signal = sig;
	rl_done = 1;
}

static int	heredoc_hook(void)
{
	if (g_signal == SIGINT)
		rl_done = 1;
	return (0);
}

void	set_signals(int mode)
{
	static const t_sigmode	modes[] = {
	{sigint_prompt, SIG_IGN, NULL},
	{SIG_IGN, SIG_IGN, NULL},
	{SIG_DFL, SIG_DFL, NULL},
	{sigint_heredoc, SIG_IGN, heredoc_hook}};
	struct sigaction		sa;

	ft_bzero(&sa, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = modes[mode].on_int;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = modes[mode].on_quit;
	sigaction(SIGQUIT, &sa, NULL);
	rl_event_hook = modes[mode].hook;
}
