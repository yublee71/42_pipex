/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/02 23:33:59 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	child_process(int i, int fds[2][2], char *input, char *output, char *cmd, char **env, int cmd_cnt)
{
	char	*tmp;
	char	**args;
	int		fd_input;
	int		fd_output;

	if (i != 0)
	{
		close(fds[0][1]);
		dup2(fds[0][0], STDIN_FILENO);
		close(fds[0][0]);
	}
	else
	{
		fd_input = open(input, O_RDONLY);
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
		fd_output = open(output, O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (fd_output < 0)
			exit_with_error("output", EXIT_FAILURE);
		if (dup2(fd_output, STDOUT_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE);
		close(fd_output);
	}
	args = get_args(cmd, env);
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

int main(int argc, char *argv[], char **env)
{
	int		cmd_cnt;
	int		fds[2][2];
	pid_t	pid;
	int		status;
	int		exit_status;

	cmd_cnt = argc - 3;
	if (cmd_cnt < 2)
		exit_with_error("bad arguments", EXIT_FAILURE);
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
		pid = fork();
		if (pid < 0)
		{
			exit_with_error("fork", EXIT_FAILURE);
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
			child_process(i, fds, argv[1], argv[argc - 1], argv[i + 2], env, cmd_cnt);
		else
		{
			if (i != 0)
				close(fds[0][0]);
			if (i != cmd_cnt - 1)
				close(fds[1][1]);
		}
	}
	exit_status = 0;
	while (wait(&status) != -1)
	{	
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
	}
	exit(exit_status);
}
