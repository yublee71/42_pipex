/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:34:39 by yublee            #+#    #+#             */
/*   Updated: 2024/04/16 02:00:32 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	join_path(char **paths, char **args)
{
	int		i;
	char	*temp;

	i = 0;
	while (paths[i])
	{
		temp = paths[i];
		paths[i] = ft_strjoin(paths[i], "/");
		free(temp);
		temp = paths[i];
		paths[i] = ft_strjoin(paths[i], *args);
		free(temp);
		i++;
	}
}

void	check_path(char **paths, char **args)
{
	int		i;
	int		flag;
	char	*temp;

	i = 0;
	flag = 0;
	while (paths[i])
	{
		if (!access(paths[i], X_OK))
		{
			temp = *args;
			*args = ft_strdup(paths[i]);
			free(temp);
			flag = 1;
			break ;
		}
		i++;
	}
	if (!flag)
	{
		free(*args);
		*args = NULL;
	}
}

char	**get_args(char *argv)
{
	char	**args;
	int		i;
	char	*path;
	char	**paths;

	i = 0;
	args = ft_split(argv, ' ');
	if (!access(args[0], X_OK))
		return (args);
	while (!ft_strnstr(environ[i], "PATH", 4))
		i++;
	path = ft_strnstr(environ[i], "PATH", 4) + 5;
	paths = ft_split(path, ':');
	join_path(paths, &args[0]);
	check_path(paths, &args[0]);
	free_str_array(paths);
	return (args);
}
