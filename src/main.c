/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/03 00:15:14 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static t_info	get_info(int argc, char **argv, char **env)
{
	t_info	info;

	info.cmd_cnt = argc - 3;
	info.input = argv[1];
	info.output = argv[argc - 1];
	info.env = env;
	return (info);
}

static int	exit_status(void)
{
	int	exit_status;
	int	status;

	exit_status = 0;
	status = 0;
	while (wait(&status) != -1)
	{
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
	}
	return (exit_status);
}

int	main(int argc, char *argv[], char **env)
{
	int		fds[2][2];
	t_info	info;

	if (argc < 5)
		exit_with_error("bad arguments", EXIT_FAILURE);
	info = get_info(argc, argv, env);
	pipex(info, fds, argv);
	exit(exit_status());
}
