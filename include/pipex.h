/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:55:33 by yublee            #+#    #+#             */
/*   Updated: 2024/04/16 00:38:50 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "libft.h"

extern char	**environ;

void	child_process(int fd[2], char *argv[]);
void	parent_process(int fd[2], char *argv[]);
char	**get_args(char *argv);
void	join_path(char **paths, char **args);
void	check_path(char **paths, char **args);
void	exit_with_error(const char *str, int errno);
void	free_str_array(char **array);