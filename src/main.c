/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/02 15:42:48 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char *argv[], char **env)
{
	int		pipe_to_child[2];
	int		pipe_from_child[2];
	int		fd_input;
	int		fd_output;
	pid_t	pid;
	char	buf1[1] = {0};
	char	buf2[2] = {0};
	char	*args[2] = {"ls", NULL};

	if (pipe(pipe_to_child) < 0)
		exit_with_error("pipe error", EXIT_FAILURE);
	if (pipe(pipe_from_child) < 0)
		exit_with_error("pipe error", EXIT_FAILURE);
	pid = fork();
	if (pid < 0)
		exit_with_error("fork error", EXIT_FAILURE);
	if (pid == 0)
	{
		close(pipe_to_child[WRITE_END]);
		close(pipe_from_child[READ_END]);
		dup2(pipe_to_child[READ_END], 0);
		dup2(pipe_from_child[WRITE_END], 1);
		close(pipe_to_child[READ_END]);
		close(pipe_from_child[WRITE_END]);
		execve("/usr/bin/ls", args, env);
	}
	close(pipe_to_child[READ_END]);
	close(pipe_from_child[WRITE_END]);

	fd_input = open(argv[1], O_RDONLY);
	if (fd_input < 0)
		exit_with_error("open error", EXIT_FAILURE);
	while (read(fd_input, buf1, 1) > 0)
		write(pipe_to_child[WRITE_END], buf1, 1);
	fd_output = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fd_output < 0)
		exit_with_error("open error", EXIT_FAILURE);
	close(pipe_to_child[WRITE_END]);
	while (read(pipe_from_child[READ_END], buf2, 1) > 0)
		write(fd_output, buf2, 1);
	close(pipe_from_child[READ_END]);
	while (wait(NULL) != -1)
		;
	exit (EXIT_SUCCESS);
}
