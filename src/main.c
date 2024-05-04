/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/04 19:16:32 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static t_info	get_info(int argc, char **argv, char **env)
{
	t_info	info;

	info.output = argv[argc - 1];
	info.env = env;
	info.cmd_cnt = argc - 3;
	info.here_doc = !ft_strncmp(argv[1], "here_doc", 8);
	if (info.here_doc == 0)
		info.input = argv[1];
	else
		info.input = NULL;
	return (info);
}

static void	free_fds(int **fds, int i)
{
	int	j;

	j = 0;
	while (j < i)
		free(fds[j++]);
	free(fds);
}

static int	**create_pipeline(int cnt)

{
	int	**fds;
	int	i;

	fds = (int **)malloc(cnt * sizeof(int *));
	if (!fds)
		exit_with_error("malloc", EXIT_FAILURE);
	i = 0;
	while (i < cnt)
	{
		fds[i] = malloc(2 * sizeof(int));
		if (!fds[i])
		{
			free_fds(fds, i);
			exit_with_error("malloc", EXIT_FAILURE);
		}
		if (pipe(fds[i]) < 0)
		{
			free_fds(fds, i + 1);
			exit_with_error("pipe", EXIT_FAILURE);
		}
		i++;
	}
	return (fds);
}

static void	exit_status(t_info info)
{
	int	tmp;

	tmp = errno;
	free_fds(info.fds, info.cmd_cnt - 1);
	while (wait(NULL) != -1)
		;
	errno = tmp;
	return ;
}

int	main(int argc, char *argv[], char **env)
{
	int		status;
	t_info	info;
	pid_t	pid;

	if (argc < 5)
		exit_with_error("bad arguments", EXIT_FAILURE);
	info = get_info(argc, argv, env);
	info.fds = create_pipeline(info.cmd_cnt - 1);
	pid = pipex(info, argv);
	
	waitpid(pid, &status, 0);
	exit_status(info);
	exit(WEXITSTATUS(status));
}
