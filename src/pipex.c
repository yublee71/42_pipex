/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/04 21:37:18 by yublee           ###   ########.fr       */
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
				exit_with_error("malloc", EXIT_FAILURE, info);
			if (!ft_strncmp(buf, cmd, ft_strlen(cmd) - 1))
				break ;
			write(1, buf, ft_strlen(buf));
			free(buf);
		}
		exit(EXIT_SUCCESS);
	}
	args = get_args(cmd, info.env, info);
	if (execve(args[0], args, info.env) == -1)
	{
		free_str_array(args);
		exit_with_error("execve", EXIT_FAILURE, info);
	}
}

static void	get_output(t_info info)
{
	int	fd_output;

	if (!info.here_doc)
		fd_output = open(info.output, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	else
		fd_output = open(info.output, O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (fd_output < 0)
		exit_with_error("output", EXIT_FAILURE, info);
	if (dup2(fd_output, STDOUT_FILENO) < 0)
		exit_with_error("dup2", EXIT_FAILURE, info);
	close(fd_output);
}

static void	get_input(t_info info)
{
	int	fd_input;

	if (info.here_doc)
		return ;
	fd_input = open(info.input, O_RDONLY);
	if (fd_input < 0)
		exit_with_error("input", EXIT_FAILURE, info);
	if (dup2(fd_input, STDIN_FILENO) < 0)
		exit_with_error("dup2", EXIT_FAILURE, info);
	close(fd_input);
}

static void	child_process(int i, int **fds, char *cmd, t_info info)
{
	if (i != 0)
	{
		close(fds[i - 1][WRITE_END]);
		if (dup2(fds[i - 1][READ_END], STDIN_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE, info);
		close(fds[i - 1][READ_END]);
	}
	else
		get_input(info);
	if (i != info.cmd_cnt - 1)
	{
		close(fds[i][READ_END]);
		if (dup2(fds[i][WRITE_END], STDOUT_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE, info);
		close(fds[i][WRITE_END]);
	}
	else
		get_output(info);
	execute_cmd(cmd, info, i);
}

void	pipex(t_info info, char **argv)
{
	int		i;
	pid_t	pid;
	int		status;
	// int		max_status;

	i = -1;
	// max_status = 0;
	while (++i < info.cmd_cnt)
	{
		pid = fork();
		if (pid < 0)
			exit_with_error("fork", EXIT_FAILURE, info);
		if (pid == 0)
			child_process(i, info.fds, argv[i + 2], info);
		if (i != 0)
			close(info.fds[i - 1][READ_END]);
		if (i != info.cmd_cnt - 1)
			close(info.fds[i][WRITE_END]);
	}
	// printf("pid: %d", getpid());
	// free_fds(info.fds, info.cmd_cnt - 1);
	while (wait(&status) != -1)
	{
		// printf("what : %d\n", WEXITSTATUS(status));
		// if (max_status < WEXITSTATUS(status))
		// 	max_status = WEXITSTATUS(status);
		;
	}
	// printf("max : %d\n", max_status);
	free_fds(info.fds, info.cmd_cnt - 1);
	// exit (status);
	// errno = max_status;
	// free_fds(info.fds, info.cmd_cnt - 1);
	// return (max_status);
}
