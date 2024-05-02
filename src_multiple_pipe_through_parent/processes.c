/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:34:30 by yublee            #+#    #+#             */
/*   Updated: 2024/05/02 14:31:26 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_process(int fd1[2], int fd2[2], char *argv)
{
	char	**args;
	// char	line[1000];

//read using fd1
//write using fd2
	close(fd1[1]);
	dup2(fd1[0], 0);
	close(fd1[0]);

	// while(fgets(line, 1000, stdin))
	// {
	// 	printf("is it here");
	// 	puts(line);
	// }

	close(fd2[0]);
	dup2(fd2[1], 1);
	close(fd2[1]);

	args = get_args(argv);
	if (!args[0])
	{
		free_str_array(args);
		exit_with_error("command not found", 127);
	}
	if (execve(args[0], args, environ) == -1)
	{
		free_str_array(args);
		exit_with_error("execve", EXIT_FAILURE);
	}
	exit(0);
}
