/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/03 15:53:47 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static t_info	get_info(int argc, char **argv, char **env)
{
	t_info	info;

	info.output = argv[argc - 1];
	info.env = env;
	info.cmd_cnt = argc - 3;
	info.here_doc = !ft_strncmp(argv[1], "here_doc", 8);
	if (info.here_doc == 0)
		info.input = argv[1];
	else
		info.input = NULL;
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
	int		status;
	t_info	info;
	pid_t	pid;

	if (argc < 5)
		exit_with_error("bad arguments", EXIT_FAILURE);
	info = get_info(argc, argv, env);
	pid = pipex(info, fds, argv);
	waitpid(pid, &status, 0);
	exit_status();
	exit(WEXITSTATUS(status));
}
