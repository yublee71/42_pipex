/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/02 17:24:26 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char *argv[], char **env)
{
	int		pipe_to_child[2];
	int		pipe_from_child[2];
	int		fd_input;
	int		fd_output;
	int		cmd_cnt;
	pid_t	pid;
	char	buf1[2] = {0};
	char	buf2[2] = {0};
	char	**args;
	int		i = 0;

	cmd_cnt = argc - 3;
	if (cmd_cnt < 2)
		exit_with_error("bad arguments", EXIT_FAILURE);
	fd_input = open(argv[1], O_RDONLY);
	if (fd_input < 0)
		exit_with_error("open error", EXIT_FAILURE);
	while (i < cmd_cnt)
	{
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
			args = get_args(argv[i + 2]);
			if (!args[0])
			{
				free_str_array(args);
				exit_with_error("command not found", 127);
			}
			if (execve(args[0], args, env) == -1)
			{
				free_str_array(args);
				exit_with_error("execve", EXIT_FAILURE);
			}
			exit(0);
		}

		//write to child
		close(pipe_to_child[READ_END]);
		if (i != 0)
		{
			fd_output = open(argv[argc - 1], O_RDONLY);
			while (read(fd_output, buf1, 1) > 0)
				write(pipe_to_child[WRITE_END], buf1, 1);
		}
		else
		{
			while (read(fd_input, buf1, 1) > 0)
				write(pipe_to_child[WRITE_END], buf1, 1);
		}
		close(pipe_to_child[WRITE_END]);

		//read from child
		close(pipe_from_child[WRITE_END]);
		fd_output = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (fd_output < 0)
			exit_with_error("open error", EXIT_FAILURE);
		while (read(pipe_from_child[READ_END], buf2, 1) > 0)
			write(fd_output, buf2, 1);
		close(pipe_from_child[READ_END]);
		i++;
	}
	exit (EXIT_SUCCESS);
}
