/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/02 14:32:05 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char *argv[])
{
	int		fd1[2];
	int		fd2[2];
	// int		cmd_cnt;
	int		i;
	pid_t	pid;
	int		fd_input;
	int		fd_output;
	char	buf[100];
	
	// cmd_cnt = argc - 3;
	i = 0;
	// if (cmd_cnt < 2)
	// 	exit_with_error("bad arguments", EXIT_FAILURE);
	//fd1 will be given to child
	//fd2 is given by child
	printf("%d\n", argc);
	while (i < 1)
	{
		printf("%d\n", argc);
		if (pipe (fd1) < 0)
			exit_with_error("pipex error", EXIT_FAILURE);
		close(fd1[0]);
		if (i == 0)
		{
			fd_input = open(argv[1], O_RDONLY);
			if (fd_input == -1)
				exit_with_error("open input", EXIT_FAILURE);
			// read(fd_input, buf, 10);
			// write(fd1[1], buf, 10);
			while (read(fd_input, buf, 100) > 0)
			{
				write(1, buf, 4);
			}
			close(fd_input);
			write(fd1[1], buf, 4);
		}
		// else
		// {
		// 	dup2(fd2[0],fd1[1]);
		// 	close(fd2[0]);
		// }
		if (pipe (fd2) < 0)
			exit_with_error("pipex error", EXIT_FAILURE);
		close(fd2[1]);
		// else
		// {
		// 	while(read(fd2[0], buf, 1) > 0)
		// 	// 	write(1, buf, 1);
		// 		write(fd1[1], buf, 1);
		// 	// dup2(fd2[0], fd1[1]);
		// }
		pid = fork();
		if (pid < 0)
			exit_with_error("fork error", EXIT_FAILURE);
		else if (pid == 0)
			child_process(fd1, fd2, argv[i + 2]);
		printf("%d\n", argc);
		i++;
	}
	printf("%d\n", argc);
	fd_output = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fd_output == -1)
		exit_with_error("open output", EXIT_FAILURE);
	// while (read(fd2[0], buf, 100)> 0)
	// {
	// 	write(fd_output, buf, 1);
	// 	// write(1, buf, 1);
	// }
	close(fd1[1]);
	close(fd1[0]);
	close(fd2[0]);
	close(fd2[1]);
	close(fd_output);
	while (wait(NULL) != -1)
		;
	exit (EXIT_SUCCESS);
}
