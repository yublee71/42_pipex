#include "pipex.h"

int	main(int argc, char *argv[])
{
	int		fd[2];
	int		fd_output;
	char	*args[4];
	pid_t	pid;
	// char	buf[BUFFERSIZE] = {0};
	
	if (argc < 2)
		exit (1);

	if (pipe(fd) < 0)
	{
		perror("pipe error");
		exit(1);
	}

	pid = fork();

	if (pid < 0)
	{
		perror("fork error");
		exit(1);
	}

	else if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);

		args[0] = argv[2];
		args[1] = argv[3];
		args[2] = argv[1];
		args[3] = NULL;

		if (execve(args[0], args, NULL) == -1)
		{
			perror("execve");
			exit (1);
		}
	}

	else
	{
		close(fd[1]);
		dup2(fd[0], 0);
		fd_output = open(argv[6], O_WRONLY | O_TRUNC);
		dup2(fd_output, 1);

		args[0] = argv[4];
		args[1] = argv[5];
		args[2] = NULL;
		args[3] = NULL;

		if (execve(args[0], args, NULL) == -1)
		{
			perror("execve");
			exit (1);
		}
		// read(0, buf, BUFFERSIZE);
		// printf("%s\n", buf);
	}
}