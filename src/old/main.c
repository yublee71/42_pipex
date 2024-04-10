/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:08:33 by yublee            #+#    #+#             */
/*   Updated: 2024/04/10 12:57:51 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char* argv[], char* envp[])
{
	int		fd_file1;
	int		fd_file2;
	char	**cmd1;
	char	**cmd2;
	char	buffer[BUFFERSIZE];

	if (argc != 5)
		return (1);
	fd_file1 = open(argv[1], O_RDONLY);
	while (read(fd_file1, buffer, BUFFERSIZE) > 0)
		;
	close(fd_file1);
	fd_file2 = open(argv[3], O_WRONLY);
	//how to pass buffer as input?
	cmd1 = get_command(argv[2]);
	ft_fork(cmd1[0], cmd1[1], cmd1[2]);
	//how to write output to file2?
	cmd2 = get_command(argv[3]);
	ft_fork(cmd2[0], cmd2[1], cmd2[2]);
	write(buffer, fd_file2, BUFFERSIZE);
	return (0);
}