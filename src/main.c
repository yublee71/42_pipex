/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/04/16 00:38:22 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char *argv[])
{
	int		fd[2];
	pid_t	pid;

	if (argc < 5)
		exit_with_error("bad arguments", EXIT_FAILURE);
	if (pipe(fd) < 0)
		exit_with_error("pipe error", EXIT_FAILURE);
	pid = fork();
	if (pid < 0)
		exit_with_error("fork error", EXIT_FAILURE);
	else if (pid == 0)
		child_process(fd, argv);
	else
		parent_process(fd, argv);
	exit (EXIT_SUCCESS);
}
