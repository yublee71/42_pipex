#include "pipex.h"

char **get_args(char *argv)
{
	char	**args;
	int		i;
	int		flag;
	char	*path;
	char	**paths;

	flag = 0;
	i = 0;
	args = ft_split(argv, ' ');
	while (!ft_strnstr(environ[i], "PATH", 4))
		i++;
	path = ft_strnstr(environ[i], "PATH", 4) + 5;
	paths = ft_split(path, ':');
	i = 0;
	while (paths[i])
	{
		paths[i] = ft_strjoin(paths[i], "/");
		if (!access(ft_strjoin(paths[i], args[0]), X_OK))
		{
			args[0] = ft_strjoin(paths[i], args[0]);
			flag = 1;
			break;
		}
		i++;
	}
	if (!flag)
		args[0] = NULL;
	return (args);
}