#include "pipex.h"

void	exit_with_error(const char *str, int errno)
{
	perror(str);
	exit(errno);
}