#include "pipex.h"

//./pipex infile "ls -l" "wc -l" outfile

void	free_str_array(char* str_array[])
{
	int	i;

	i = 0;
	while (str_array[i])
		free(str_array[i++]);
	free(str_array);
}

char **get_args(char *argv)
{
	char	**args;
	int		i;
	int		flag;
	char	*path;
	char	**paths;
	char	*temp;

	flag = 0;
	i = 0;
	args = ft_split(argv, ' '); //needs free
	while (!ft_strnstr(environ[i], "PATH", 4))
		i++;
	path = ft_strnstr(environ[i], "PATH", 4) + 5;
	paths = ft_split(path, ':'); //needs free
	i = 0;
	while (paths[i])
	{
		temp = paths[i];
		paths[i] = ft_strjoin(paths[i], "/");
		free(temp);
		temp = paths[i];
		paths[i] = ft_strjoin(paths[i], args[0]);
		free(temp);
		if (!access(paths[i], X_OK))
		{
			temp = args[0];
			args[0] = paths[i];
			free(temp);
			flag = 1;
			break;
		}
		i++;
	}
	if (!flag)
		args[0] = NULL;
	return (args);
}