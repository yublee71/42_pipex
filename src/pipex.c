/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/03 15:47:21 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	execute_cmd(char *cmd, t_info info, int i)
{
	char	**args;
	char	*buf;

	if (info.here_doc && i == 0)
	{
		while (1)
		{
			buf = get_next_line(0);
			if (!buf)
				exit_with_error("malloc", EXIT_FAILURE);
			if (!ft_strncmp(buf, cmd, ft_strlen(cmd) - 1))
				break ;
			write(1, buf, ft_strlen(buf));
			free(buf);
		}
		exit(EXIT_SUCCESS);
	}
	args = get_args(cmd, info.env);
	if (execve(args[0], args, info.env) == -1)
	{
		free_str_array(args);
		exit_with_error("execve", EXIT_FAILURE);
	}
}

static void	get_output(t_info info)
{
	int	fd_output;

	if (!info.here_doc)
		fd_output = open(info.output, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else
		fd_output = open(info.output, O_WRONLY | O_APPEND | O_CREAT, 0777);
	if (fd_output < 0)
		exit_with_error("output", EXIT_FAILURE);
	if (dup2(fd_output, STDOUT_FILENO) < 0)
		exit_with_error("dup2", EXIT_FAILURE);
	close(fd_output);
}

static void	get_input(t_info info)
{
	int	fd_input;

	if (info.here_doc)
		return ;
	fd_input = open(info.input, O_RDONLY);
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
		get_input(info);
	if (i != info.cmd_cnt - 1)
	{
		close(fds[1][READ_END]);
		if (dup2(fds[1][WRITE_END], STDOUT_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE);
		close(fds[1][WRITE_END]);
	}
	else
		get_output(info);
	execute_cmd(cmd, info, i);
}

pid_t	pipex(t_info info, int fds[2][2], char **argv)
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
			exit_with_error("fork", EXIT_FAILURE);
		if (pid == 0)
			child_process(i, fds, argv[i + 2], info);
		if (i != 0)
			close(fds[0][READ_END]);
		if (i != info.cmd_cnt - 1)
			close(fds[1][WRITE_END]);
	}
	return (pid);
}
