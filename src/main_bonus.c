/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/04/18 18:01:36 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char *argv[])
{
	int		fd[2];
	int		i;
	pid_t	*pids;

	pids = (pid_t *)malloc(sizeof(pid_t)*(argc - 3);
	i = 0;
	if (argc < 5)
		exit_with_error("bad arguments", EXIT_FAILURE);
	if (pipe(fd) < 0)
		exit_with_error("pipe error", EXIT_FAILURE);
	pids[0] = fork();
	if (pids[0] < 0)
		exit_with_error("fork error", EXIT_FAILURE);
	else if (pids[0] > 0)
	{
		parent_process(fd, argv);
		while (i < argc - 3 && pids[i++] > 0)
		{
			pids[i] = fork();
			ft_printf("%d\n", pids[i]);
		}
	}

	exit (EXIT_SUCCESS);
}
