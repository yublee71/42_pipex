#include "pipex.h"

void	exit_with_error(const char *str, int errno)
{
	perror(str);
	exit(errno);
}

void	free_str_array(char** array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	child_process(int fd[2], char *argv[])
{
	int		fd_input;
	char	**args;

	close(fd[0]);
	dup2(fd[1], 1);
	fd_input = open(argv[1], O_RDONLY);
	if (fd_input == -1)
		exit_with_error("open input", EXIT_FAILURE);
	dup2(fd_input, 0);
	args = get_args(argv[2]);
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
	free_str_array(args);
}

void	parent_process(int	fd[2], char *argv[])
{
	int		fd_output;
	char	**args;

	close(fd[1]);
	dup2(fd[0], 0);
	fd_output = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fd_output == -1)
		exit_with_error("open output", EXIT_FAILURE);
	dup2(fd_output, 1);
	args = get_args(argv[3]);
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
	free_str_array(args);
}