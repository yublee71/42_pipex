#include "pipex.h"

//./pipex infile "ls -l" "wc -l" outfile

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
		i++;
	}

	i = 0;
	while (paths[i])
	{
		if (!access(paths[i], X_OK))
		{
			temp = args[0];
			args[0] = ft_strdup(paths[i]);
			free(temp);
			flag = 1;
			break;
		}
		i++;
	}
	if (!flag)
	{
		free(args[0]);
		args[0] = NULL;
	}

	free_str_array(paths);
	return (args);
}

// int main()
// {
// 	char *argv = "grep a";
// 	char **args = get_args(argv);
// 	// for(int i = 0; args[i]; i++)
// 	// 	printf("%s\n", args[i]);
// 	free_str_array(args);
// }