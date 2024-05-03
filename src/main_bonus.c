/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/03 14:27:07 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static t_info	get_info(int argc, char **argv, char **env, int is_heredoc)
{
	t_info	info;

	info.output = argv[argc - 1];
	info.env = env;
	info.cmd_cnt = argc - 3;
	if (is_heredoc == 0)
	{
		info.here_doc = 0;
		info.input = argv[1];
	}
	else
	{
		info.here_doc = 1;
		info.input = NULL;
	}
	return (info);
}

static void	exit_status(void)
{
	int	tmp;

	tmp = errno;
	while (wait(NULL) != -1)
		;
	errno = tmp;
	return ;
}

int	main(int argc, char *argv[], char **env)
{
	int		fds[2][2];
	int		is_heredoc;
	int		status;
	t_info	info;
	pid_t	pid;

	if (argc < 5)
		exit_with_error("bad arguments", EXIT_FAILURE);
	is_heredoc = 0;
	if (!ft_strncmp(argv[1], "here_doc", 8))
		is_heredoc = 1;
	info = get_info(argc, argv, env, is_heredoc);
	pid = pipex(info, fds, argv);
	waitpid(pid, &status, 0);
	exit_status();
	exit(WEXITSTATUS(status));
}
