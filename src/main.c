/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/01 21:11:07 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char *argv[])
{
	int		fd[2];
	int		cmd_cnt;
	int		i;
	pid_t	pid;

	cmd_cnt = argc - 3;
	i = 0;
	if (cmd_cnt < cmd_cnt)
		exit_with_error("bad arguments", EXIT_FAILURE);
	if (pipe(fd) < 0)
		exit_with_error("pipe error", EXIT_FAILURE);
	while (i < cmd_cnt)
	{
		pid = fork();
		if (pid < 0)
			exit_with_error("fork error", EXIT_FAILURE);
		else if (pid == 0)
			child_process(fd, argv);
		i++;
	}
	while (wait(NULL) != -1)
		;
	exit (EXIT_SUCCESS);
}
