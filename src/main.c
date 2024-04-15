#include "pipex.h"

int	main(int argc, char *argv[])
{
	int		fd[2];
	// int		fd_input;
	// int		fd_output;
	// char	**args;
	pid_t	pid;
	
	if (argc < 5)
		exit_with_error("bad arguments", EXIT_FAILURE);
	if (pipe(fd) < 0)
		exit_with_error("pipe error", EXIT_FAILURE);
	pid = fork();
	if (pid < 0)
		exit_with_error("fork error", EXIT_FAILURE);
	else if (pid == 0)
		child_process(fd, argv);
	// {
	// 	close(fd[0]);
	// 	dup2(fd[1], 1);
	// 	fd_input = open(argv[1], O_RDONLY);
	// 	dup2(fd_input, 0);

	// 	args = get_args(argv[2]);

	// 	if (execve(args[0], args, environ) == -1)
	// 	{
	// 		perror("execve");
	// 		exit (EXIT_FAILURE);
	// 	}
	// }
	else
		parent_process(fd, argv);
	// {
	// 	close(fd[1]);
	// 	dup2(fd[0], 0);
	// 	fd_output = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	// 	dup2(fd_output, 1);

	// 	args = get_args(argv[3]);

	// 	if (execve(args[0], args, environ) == -1)
	// 	{
	// 		perror("execve");
	// 		exit (EXIT_FAILURE);
	// 	}
	// }
	exit (0);
}