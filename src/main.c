/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:08:33 by yublee            #+#    #+#             */
/*   Updated: 2024/02/22 11:34:05 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char* argv[])
{
	int		fd;
	char	c[10];
	if (argc)
		fd = open(argv[1], O_RDONLY);
	read(fd, c, 10);
	ft_printf("%s\n",c);
	close(fd);
}