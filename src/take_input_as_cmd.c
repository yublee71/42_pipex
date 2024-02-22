#include "pipex.h"

char**	get_command(char *str)
{
	char	**splited_command; // needs freeing later

	splited_command = ft_split(str, ' ');
	if (!splited_command)
		return (NULL);
}