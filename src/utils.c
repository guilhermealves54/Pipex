/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gribeiro <gribeiro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 16:43:14 by gribeiro          #+#    #+#             */
/*   Updated: 2025/03/15 23:20:19 by gribeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_mem(t_pipex *px)
{
	int	i;

	if (px->cmd_a)
	{
		i = 0;
		while (px->cmd_a[i])
			free (px->cmd_a[i++]);
		free (px->cmd_a);
	}
	if (px->cmd_b)
	{
		i = 0;
		while (px->cmd_b[i])
			free (px->cmd_b[i++]);
		free (px->cmd_b);
	}
	if (px->path_a)
		free (px->path_a);
	if (px->path_b)
		free (px->path_b);
}

int	closefds(t_pipex *px, int n, char *errmsg)
{
	if (errmsg)
		perror (errmsg);
	if (n >= 1)
		if (px->infile >= 0)
			close (px->infile);
	if (n >= 2)
		if (px->infile >= 0)
			close (px->outfile);
	if (n == 4)
	{
		close (px->fd[0]);
		close (px->fd[1]);
	}
	return (1);
}

char	**extractpaths(char **envp)
{
	char	**paths;
	char	*path_line;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp (envp[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	if (!envp)
		return (NULL);
	path_line = ft_strdup (envp[i] + 5);
	if (!path_line)
		return (NULL);
	paths = ft_split (path_line, ':');
	if (!paths)
		return (free (path_line), NULL);
	return (free (path_line), paths);
}

static void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free (paths[i++]);
	free (paths);
}

char	*findpath(char **envp, char **cmd)
{
	char	**paths;
	char	*path;
	char	*temp;
	int		i;

	if (!cmd[0])
		return (NULL);
	paths = extractpaths (envp);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin (paths[i], "/");
		temp = path;
		path = ft_strjoin (path, cmd[0]);
		free (temp);
		if (access (path, X_OK) == 0)
			return (free_paths (paths), path);
		free (path);
		i++;
	}
	return (free_paths (paths), NULL);
}
