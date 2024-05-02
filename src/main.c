/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/02 23:54:54 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

typedef struct s_info
{
	char	*input;
	char	*output;
	int		cmd_cnt;
	char	**env;
}	t_info;

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

static void	child_process(int i, int fds[2][2], char *cmd, t_info info)
{
	int		fd_input;
	int		fd_output;

	if (i != 0)
	{
		close(fds[0][1]);
		if (dup2(fds[0][0], STDIN_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE);
		close(fds[0][0]);
	}
	else
	{
		fd_input = open(info.input, O_RDONLY);
		if (fd_input < 0)
			exit_with_error("input", EXIT_FAILURE);
		if (dup2(fd_input, STDIN_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE);
		close(fd_input);
	}
	if (i != info.cmd_cnt - 1)
	{
		close(fds[1][0]);
		if (dup2(fds[1][1], STDOUT_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE);
		close(fds[1][1]);
	}
	else
	{
		fd_output = open(info.output, O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (fd_output < 0)
			exit_with_error("output", EXIT_FAILURE);
		if (dup2(fd_output, STDOUT_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE);
		close(fd_output);
	}
	execute_cmd(cmd, info);
}

static t_info	get_info(int argc, char **argv, char **env)
{
	t_info	info;

	info.cmd_cnt = argc - 3;
	info.input = argv[1];
	info.output = argv[argc - 1];
	info.env = env;
	return (info);
}

int main(int argc, char *argv[], char **env)
{
	int		fds[2][2];
	pid_t	pid;
	t_info	info;
	int		i;
	int		status;
	int		exit_status;

	if (argc < 5)
		exit_with_error("bad arguments", EXIT_FAILURE);
	info = get_info(argc, argv, env);
	i = 0;
	while(i < info.cmd_cnt)
	{
		if (i != 0)
		{
			fds[0][0] = fds[1][0];
			fds[0][1] = fds[1][1];
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
		else
		{
			if (i != 0)
				close(fds[0][0]);
			if (i != info.cmd_cnt - 1)
				close(fds[1][1]);
		}
		i++;
	}
	exit_status = 0;
	while (wait(&status) != -1)
	{	
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
	}
	exit(exit_status);
}
