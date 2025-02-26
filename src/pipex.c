/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gribeiro <gribeiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:39:06 by gribeiro          #+#    #+#             */
/*   Updated: 2025/02/26 02:15:37 by gribeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	child1(int *fd, char **argv, char **cmd_a, char *path_a)
{
	int	infile;

	infile = open (argv[1], O_RDONLY);
	if (infile < 0)
		return (perror ("Error loading infile.\n"), 1);
	if (dup2 (infile, STDIN_FILENO) == -1)
		return (perror ("Error.\n"), 1);
	if (dup2 (fd[1], STDOUT_FILENO) == -1)
		return (perror ("Error.\n"), 1);
	close (infile);
	close (fd[0]);
	close (fd[1]);
	if (execve (path_a, cmd_a, NULL) == -1)
		return (perror ("Error executing cmd 1.\n"), 1);
	return (0);
}

int	child2(int *fd, char **argv, char **cmd_b, char *path_b)
{
	int	outfile;

	outfile = open (argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
		return (perror ("Error loading outfile.\n"), 1);
	if (dup2 (fd[0], STDIN_FILENO) == -1)
		return (perror ("Error.\n"), 1);
	if (dup2 (outfile, STDOUT_FILENO) == -1)
		return (perror ("Error.\n"), 1);
	close (outfile);
	close (fd[0]);
	close (fd[1]);
	if (execve (path_b, cmd_b, NULL) == -1)
		return (perror ("Error executing cmd 2.\n"), 1);
	return (0);
}

int	fork_proc(char **argv, char **cmd_a, char **cmd_b, char *path_a, char *path_b)
{
	pid_t	pid1;
	pid_t	pid2;
	int		fd[2];
	int		sts;

	if 	(pipe (fd) == -1)
		return (perror ("Pipe creation failed.\n"), 1);
	if ((pid1 = fork ()) < 0)
		return (perror ("Error creating a new process.\n"), 1);
	if (pid1 == 0)
		if (child1 (fd, argv, cmd_a, path_a) == 1)
			return (1);
	if ((pid2 = fork ()) < 0)
		return (perror ("Error creating a new process.\n"), 1);
	if (pid2 == 0)
		if (child2 (fd, argv, cmd_b, path_b) == 1)
			return (1);
	close (fd[0]);
	close (fd[1]);
	if (waitpid (pid1, &sts, 0) == -1)
		return (perror ("Error waiting for child1.\n"), 1);
	if (waitpid (pid2, &sts, 0) == -1)
		return (perror ("Error waiting for child2.\n"), 1);
	return (0);
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
	path_line = ft_strdup (envp[i] + 5);
	if (!path_line)
		return (NULL);
	paths = ft_split (path_line, ':');
	if (!paths)
		return (free (path_line), NULL);
	return (free (path_line), paths);
}
static void free_paths(char **paths)
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

void	free_part(int n, char **cmd_a, char **cmd_b, char *path_a)
{
	int	i;
	
	if (n >= 1)
	{
		i = 0;
		while (cmd_a[i])
			free (cmd_a[i++]);
		free (cmd_a);
	}
	if (n >= 2)
	{
		i = 0;
		while (cmd_b[i])
			free (cmd_b[i++]);
		free (cmd_b);
	}
	if (n == 3)
		free (path_a);
}

void	free_mem(char **cmd_a, char **cmd_b, char *path_a, char *path_b)
{
	int	i;
	
	i = 0;
	while (cmd_a[i])
		free (cmd_a[i++]);
	free (cmd_a);
	i = 0;
	while (cmd_b[i])
		free (cmd_b[i++]);
	free (cmd_b);
	free (path_a);
	free (path_b);
}

int	main(int argc, char **argv, char **envp)
{
	int		n;
	char	**cmd_a;
	char	**cmd_b;
	char	*path_a;
	char	*path_b;

	n = argc - 1;
	while (n > 0)
		if (argc != 5 || argv[n--][0] == '\0')
			return (perror ("Invalid arguments.\n"), 1);
	path_a = NULL;
	cmd_a = ft_split (argv[2], ' ');
	if (!cmd_a)
		return (perror ("Memory error.\n"), 1);
	cmd_b = ft_split (argv[3], ' ');
	if (!cmd_b)
	{
		free_part (1, cmd_a, cmd_b, path_a);
		return (perror ("Memory error.\n"), 1);
	}
	path_a = findpath (envp, cmd_a);
	if (!path_a)
	{
		free_part (2, cmd_a, cmd_b, path_a);
		return (perror ("Error / Cmd1 not found.\n"), 1);
	}
	path_b = findpath (envp, cmd_b);
	if (!path_b)
	{
		free_part (3, cmd_a, cmd_b, path_a);
		return (perror ("Error / Cmd2 not found.\n"), 1);
	}
	if (fork_proc (argv, cmd_a, cmd_b, path_a, path_b))
		return (free_mem (cmd_a, cmd_b, path_a, path_b), 1);
	return (free_mem (cmd_a, cmd_b, path_a, path_b), 0);
}
