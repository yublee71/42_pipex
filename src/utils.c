#include "pipex.h"

void	exit_with_error(const char *str, int errno)
{
	perror(str);
	exit(errno);
}

void	child_process(int fd[2], char *argv[])
{
	int		fd_input;
	char	**args;

	close(fd[0]);
	dup2(fd[1], 1);
	fd_input = open(argv[1], O_RDONLY);
	dup2(fd_input, 0);
	args = get_args(argv[2]);
	if (execve(args[0], args, environ) == -1)
		exit_with_error("execve", EXIT_FAILURE);
}

void	parent_process(int	fd[2], char *argv[])
{
	int		fd_output;
	char	**args;

	close(fd[1]);
	dup2(fd[0], 0);
	fd_output = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	dup2(fd_output, 1);
	args = get_args(argv[3]);
	if (execve(args[0], args, environ) == -1)
		exit_with_error("execve", EXIT_FAILURE);
}