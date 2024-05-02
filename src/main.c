/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/02 18:44:17 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[], char **env)
{
	int		cmd_cnt;
	cmd_cnt = argc - 3;

	if (cmd_cnt < 2)
		exit_with_error("bad arguments", EXIT_FAILURE);

	char	**args;
	int		pipe_fd[cmd_cnt - 1][2];
	pid_t	pids[cmd_cnt];
	int		fd_input;
	int		fd_output;

	for (int i = 0; i < cmd_cnt - 1; i++)
	{
		if (pipe(pipe_fd[i]) < 0)
			exit_with_error("pipe", EXIT_FAILURE);
	}

	for (int i = 0; i < cmd_cnt; i++)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			exit_with_error("fork", EXIT_FAILURE);
			exit(EXIT_FAILURE);
		}
		if (pids[i] == 0)
		{
			if (i != 0)
			{
				close(pipe_fd[i - 1][1]);
				dup2(pipe_fd[i - 1][0], STDIN_FILENO);
				close(pipe_fd[i - 1][0]);
			}
			else
			{
				fd_input = open(argv[1], O_RDONLY);
				if (fd_input < 0)
					exit_with_error("input open", EXIT_FAILURE);
				if (dup2(fd_input, STDIN_FILENO) < 0)
					exit_with_error("dup2", EXIT_FAILURE);
				close(fd_input);
			}
			if (i != cmd_cnt - 1)
			{
				close(pipe_fd[i][0]);
				dup2(pipe_fd[i][1], STDOUT_FILENO);
				close(pipe_fd[i][1]);
			}
			else
			{
				fd_output = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
				if (fd_output < 0)
					exit_with_error("output open", EXIT_FAILURE);
				if (dup2(fd_output, STDOUT_FILENO) < 0)
					exit_with_error("dup2", EXIT_FAILURE);
				close(fd_output);
			}
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
		}
		else
		{
			if (i != 0)
				close(pipe_fd[i - 1][0]);
			if (i != cmd_cnt - 1)
				close(pipe_fd[i][1]);
		}
	}
	while (wait(NULL) != -1)
		;
	exit(EXIT_SUCCESS);
}
