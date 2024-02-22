/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:08:33 by yublee            #+#    #+#             */
/*   Updated: 2024/02/22 12:42:29 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char* argv[])
{
	int		fd1;
	int		fd2;
	char	c[10];
    pid_t pid;

	if (argc != 5)
		return (1);

	get_command(argv[2]);
	get_command(argv[3]);
	fd1 = open(argv[1], O_RDONLY);
	read(fd1, c, 10);
	ft_printf("%s\n",c);
    pid = fork(); // Create a new process
    if (pid == 0)
	{
        char *args[] = {"/bin/ls", "-l", NULL}; // Example arguments for the "ls" command
        char *env[] = {NULL}; // No additional environment variables
        execve(args[0], args, env); // Replace the child process with "ls"
        // If execve returns, it means there was an error
        perror("execve");
    }
	else if (pid > 0)
	{
        // Wait for the child process to finish
        wait(NULL);
        printf("Child process completed.\n");
    }
	else
	{
        perror("fork");
        return 1;
    }
	close(fd1);
    return 0;
}