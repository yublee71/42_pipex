#include "libft.h"
#include "pipex.h"
#include <stdio.h>

// extern char **environ;

int	main(void)
{
	// char *args[] = {"awk", "{count++} END {print count}", "input", NULL};
	// execve("/usr/bin/awk", args, environ);
	char *str = "'yoyo'";
	printf("%s\n", str);
	str = ft_strtrim(str, "'");
	printf("%s\n", str);
}
