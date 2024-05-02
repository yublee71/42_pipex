/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/03 00:24:00 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	execute_cmd(char *cmd, t_info info)
{
	char	*tmp;
	char	**args;

	args = get_args(cmd, info.env);
	if (access(args[0], X_OK))
	{
		tmp = ft_strdup(args[0]);
		free_str_array(args);
		exit_with_error(tmp, 127);
	}
	if (execve(args[0], args, info.env) == -1)
	{
		free_str_array(args);
		exit_with_error("execve", EXIT_FAILURE);
	}
}

static void	get_output(char *output)
{
	int	fd_output;

	fd_output = open(output, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fd_output < 0)
		exit_with_error("output", EXIT_FAILURE);
	if (dup2(fd_output, STDOUT_FILENO) < 0)
		exit_with_error("dup2", EXIT_FAILURE);
	close(fd_output);
}

static void	get_input(char *input)
{
	int	fd_input;

	fd_input = open(input, O_RDONLY);
	if (fd_input < 0)
		exit_with_error("input", EXIT_FAILURE);
	if (dup2(fd_input, STDIN_FILENO) < 0)
		exit_with_error("dup2", EXIT_FAILURE);
	close(fd_input);
}

static void	child_process(int i, int fds[2][2], char *cmd, t_info info)
{
	if (i != 0)
	{
		close(fds[0][WRITE_END]);
		if (dup2(fds[0][READ_END], STDIN_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE);
		close(fds[0][READ_END]);
	}
	else
		get_input(info.input);
	if (i != info.cmd_cnt - 1)
	{
		close(fds[1][READ_END]);
		if (dup2(fds[1][WRITE_END], STDOUT_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE);
		close(fds[1][WRITE_END]);
	}
	else
		get_output(info.output);
	execute_cmd(cmd, info);
}

void	pipex(t_info info, int fds[2][2], char **argv)
{
	int		i;
	pid_t	pid;

	i = -1;
	while (++i < info.cmd_cnt)
	{
		if (i != 0)
		{
			fds[0][READ_END] = fds[1][READ_END];
			fds[0][WRITE_END] = fds[1][WRITE_END];
		}
		pipe(fds[1]);
		pid = fork();
		if (pid < 0)
		{
			exit_with_error("fork", EXIT_FAILURE);
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
			child_process(i, fds, argv[i + 2], info);
		if (i != 0)
			close(fds[0][READ_END]);
		if (i != info.cmd_cnt - 1)
			close(fds[1][WRITE_END]);
	}
}
