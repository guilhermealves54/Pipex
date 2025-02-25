/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gribeiro <gribeiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:39:06 by gribeiro          #+#    #+#             */
/*   Updated: 2025/02/25 13:42:47 by gribeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	child1(int *fd, char **argv)
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
	if (execlp (argv[2], argv[2], NULL) == -1)
		return (perror ("Error executing cmd 1.\n"), 1);
	return (0);
}

int	child2(int *fd, char **argv)
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
	if (execlp (argv[3], argv[3], NULL) == -1)
		return (perror ("Error executing cmd 2.\n"), 1);
	return (0);
}

int	fork_proc(int *fd, char **argv)
{
	pid_t	pid1;
	pid_t	pid2;
	int		sts;

	pid1 = fork ();
	if (pid1 < 0)
		return (perror ("Error creating a new process.\n"), 1);
	if (pid1 == 0)
		if (child1 (fd, argv) == 1)
			return (1);
	pid2 = fork ();
	if (pid2 < 0)
		return (perror ("Error creating a new process.\n"), 1);
	if (pid2 == 0)
		if (child2 (fd, argv) == 1)
			return (1);
	close (fd[0]);
	close (fd[1]);
	if (waitpid (pid1, &sts, 0) == -1)
		return (perror ("Error waiting for child1.\n"), 1);
	if (waitpid (pid2, &sts, 0) == -1)
		return (perror ("Error waiting for child2.\n"), 1);
	return (0);
}

int	main(int argc, char **argv)
{
	int	n;
	int	fd[2];

	n = argc - 1;
	while (n > 0)
		if (argc != 5 || argv[n--][0] == '\0')
			return (perror ("Invalid arguments.\n"), 1);
	if 	(pipe (fd) == -1)
		return (perror ("Pipe creation failed.\n"), 1);
	if (fork_proc (fd, argv))
		return (1);
	return (0);
}
