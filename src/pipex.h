/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gribeiro <gribeiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:39:02 by gribeiro          #+#    #+#             */
/*   Updated: 2025/03/17 15:32:46 by gribeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

// Libs
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdio.h>

// Struct
typedef struct s_pipex
{
	pid_t	pid1;
	pid_t	pid2;
	int		fd[2];
	int		infile;
	int		outfile;
	char	*path_a;
	char	*path_b;
	char	**cmd_a;
	char	**cmd_b;
	char	**argv;
}	t_pipex;

// Libft functions
char	**ft_split(const char *s, char c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_strdup(const char *s1);
void	ft_putstr_fd(char *s, int fd);

// Utility functions
void	free_mem(t_pipex *px);
int		closefds(t_pipex *px, int n, char *errmsg);
char	**extractpaths(char **envp);
char	*findpath(char **envp, char **cmd);

#endif