#include "pipex.h"

int	main(int argc, char *argv[])
{
	int		fd[2];
	// int		fd_file;
	int		i;
	char	*args[argc];
	pid_t	pid;
	char	buf[BUFFERSIZE] = {0};
	
	if (argc < 2 && argv)
		return 1;

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
		// fd_file = open("./output.txt", O_RDWR | O_TRUNC);
		dup2(fd[1], 1);

		i = -1;
		while (++i < argc - 1)
			args[i] = argv[i + 1];
		args[i] = NULL;

		if (execve(args[0], args, NULL) == -1)
		{
			perror("execve");
			return 1;
		}
	}

	else
	{
		close(fd[1]);
		read(fd[0], buf, BUFFERSIZE);
		printf("%s\n", buf);
	}

	return 0;
}