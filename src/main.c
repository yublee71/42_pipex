/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/02 23:19:09 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

typedef struct s_pipe
{
	pid_t	*pids;
}	t_pipe;

static t_pipe pipe_initialize(int cmd_cnt)
{
	t_pipe	pipes;

	pipes.pids = (pid_t *)malloc(cmd_cnt * sizeof(pid_t));
	if (!pipes.pids)
	{
		exit_with_error("malloc", EXIT_FAILURE);
		
	}
	return (pipes);
}

int main(int argc, char *argv[], char **env)
{
	int		cmd_cnt;
	t_pipe	pipes;
	int		fd_input;
	int		fd_output;
	int		status;
	int		exit_status;
	char	*tmp;
	char	**args;

	cmd_cnt = argc - 3;
	if (cmd_cnt < 2)
		exit_with_error("bad arguments", EXIT_FAILURE);
	pipes = pipe_initialize(cmd_cnt);
	int	fds[2][2];
	for (int i = 0; i < cmd_cnt; i++)
	{
		if (i == 0)
			pipe(fds[1]);
		else
		{
			fds[0][0] = fds[1][0];
			fds[0][1] = fds[1][1];
			pipe(fds[1]);
		}
		pipes.pids[i] = fork();
		if (pipes.pids[i] < 0)
		{
			exit_with_error("fork", EXIT_FAILURE);
			exit(EXIT_FAILURE);
		}
		if (pipes.pids[i] == 0)
		{
			if (i != 0)
			{
				close(fds[0][1]);
				dup2(fds[0][0], STDIN_FILENO);
				close(fds[0][0]);
			}
			else
			{
				fd_input = open(argv[1], O_RDONLY);
				if (fd_input < 0)
					exit_with_error("input", EXIT_FAILURE);
				if (dup2(fd_input, STDIN_FILENO) < 0)
					exit_with_error("dup2", EXIT_FAILURE);
				close(fd_input);
			}
			if (i != cmd_cnt - 1)
			{
				close(fds[1][0]);
				dup2(fds[1][1], STDOUT_FILENO);
				close(fds[1][1]);
			}
			else
			{
				fd_output = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
				if (fd_output < 0)
					exit_with_error("output", EXIT_FAILURE);
				if (dup2(fd_output, STDOUT_FILENO) < 0)
					exit_with_error("dup2", EXIT_FAILURE);
				close(fd_output);
			}
			args = get_args(argv[i + 2], env);
			if (access(args[0], X_OK))
			{
				tmp = ft_strdup(args[0]);
				free_str_array(args);
				exit_with_error(tmp, 127);
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
				close(fds[0][0]);
			if (i != cmd_cnt - 1)
				close(fds[1][1]);
		}
	}
	exit_status = 0;
	for (int i = 0; i < cmd_cnt; i++)
	{
		waitpid(pipes.pids[i], &status, 0);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
	}
	exit(exit_status);
}
