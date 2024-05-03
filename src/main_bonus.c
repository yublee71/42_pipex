/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/03 01:57:24 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static t_info	get_info(int argc, char **argv, char **env, int is_heredoc)
{
	t_info	info;

	info.output = argv[argc - 1];
	info.env = env;
	if (is_heredoc == 0)
	{
		info.here_doc = 0;
		info.here_doc_end = NULL;
		info.input = argv[1];
		info.cmd_cnt = argc - 3;
	}
	else 
	{
		info.here_doc = 1;
		info.here_doc_end = argv[2];
		info.input = NULL;
		info.cmd_cnt = argc - 4;
	}
	return (info);
}

static int	exit_status(void)
{
	int	exit_status;
	int	status;

	exit_status = 0;
	status = 0;
	while (wait(&status) != -1)
	{
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
	}
	return (exit_status);
}

int	main(int argc, char *argv[], char **env)
{
	int		fds[2][2];
	int		is_heredoc;
	t_info	info;

	if (argc < 5)
		exit_with_error("bad arguments", EXIT_FAILURE);
	is_heredoc = 0;
	if (!ft_strncmp(argv[1], "here_doc", 8))
		is_heredoc = 1;
	info = get_info(argc, argv, env, is_heredoc);
	pipex(info, fds, argv);
	exit(exit_status());
}
