#include "pipex.h"

void	ft_fork(char *cmd, char *option, char *env)
{
	pid_t	pid;

	pid = fork(); // Create a new process
	if (pid == 0)
	{
		execve(cmd, option, env);
		perror("execve");
	}
	else if (pid > 0)
	{
		wait(NULL);
		printf("Child process completed.\n");
	}
	else
	{
		perror("fork");
		return (1);
	}
	return (0);
}