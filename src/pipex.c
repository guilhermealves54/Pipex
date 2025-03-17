/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gribeiro <gribeiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:39:06 by gribeiro          #+#    #+#             */
/*   Updated: 2025/03/17 19:08:51 by gribeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	fill_strc(t_pipex *px, char **envp, char **argv);
static int	fork_proc(t_pipex *px, char **envp);
static int	child1(t_pipex *px, char **envp);
static int	child2(t_pipex *px, char **envp);

int	main(int argc, char **argv, char **envp)
{
	t_pipex	px;
	int		sts2;
	int		sts1;

	px.pid1 = -1;
	px.pid2 = -1;
	if (argc != 5)
		return (write (2, "Pipex: Invalid arguments.\n", 27), 1);
	fill_strc (&px, envp, argv);
	if (pipe (px.fd) == -1)
		return (closefds (&px, 2, "Pipex: Pipe creation failed.\n"));
	fork_proc (&px, envp);
	closefds (&px, 4, NULL);
	sts1 = 0;
	sts2 = 0;
	if (px.pid1 > 0 && px.pid2 > 0)
	{
		waitpid (px.pid1, &sts1, 0);
		waitpid (px.pid2, &sts2, 0);
	}
	return (free_mem (&px), WEXITSTATUS(sts2));
}

static void	fill_strc(t_pipex *px, char **envp, char **argv)
{
	px->infile = open (argv[1], O_RDONLY);
	if (px->infile < 0)
	{
		ft_putstr_fd (argv[1], 2);
		perror(" ");
	}
	px->outfile = open (argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	px->cmd_a = ft_split (argv[2], ' ');
	px->cmd_b = ft_split (argv[3], ' ');
	px->path_a = findpath (envp, px->cmd_a);
	px->path_b = findpath (envp, px->cmd_b);
	px->argv = argv;
}

static int	fork_proc(t_pipex *px, char **envp)
{
	px->pid1 = fork ();
	if (px->pid1 < 0)
		return (perror ("Error: "), 1);
	if (px->pid1 == 0)
		child1 (px, envp);
	px->pid2 = fork ();
	if (px->pid2 < 0)
		return (perror ("Error: "), 1);
	if (px->pid2 == 0)
		child2 (px, envp);
	return (0);
}

static int	child1(t_pipex *px, char **envp)
{
	if (px->infile == -1)
	{
		closefds (px, 4, NULL);
		free_mem (px);
		exit(1);
	}
	if ((!px->cmd_a || !px->path_a) && (access (px->cmd_a[0], X_OK) != 0))
	{
		ft_putstr_fd ("Command not found: ", 2);
		ft_putstr_fd (px->cmd_a[0], 2);
		ft_putstr_fd ("\n", 2);
		closefds (px, 4, NULL);
		free_mem (px);
		exit (127);
	}
	dup2 (px->infile, STDIN_FILENO);
	dup2 (px->fd[1], STDOUT_FILENO);
	closefds (px, 4, NULL);
	if (access (px->cmd_a[0], X_OK) == 0)
		execve (px->cmd_a[0], px->cmd_a, envp);
	execve (px->path_a, px->cmd_a, envp);
	free_mem (px);
	exit (1);
}

static int	child2(t_pipex *px, char **envp)
{
	if (px->outfile < 0)
	{
		ft_putstr_fd (px->argv[4], 2);
		perror(" ");
		closefds (px, 4, NULL);
		free_mem (px);
		exit(1);
	}
	if ((!px->cmd_b || !px->path_b)  && (access (px->cmd_b[0], X_OK) != 0))
	{
		ft_putstr_fd ("Command not found: ", 2);
		ft_putstr_fd (px->cmd_b[0], 2);
		ft_putstr_fd ("\n", 2);
		closefds (px, 4, NULL);
		free_mem (px);
		exit (127);
	}
	dup2 (px->fd[0], STDIN_FILENO);
	dup2 (px->outfile, STDOUT_FILENO);
	closefds (px, 4, NULL);
	if (access (px->cmd_b[0], X_OK) == 0)
		execve (px->cmd_b[0], px->cmd_a, envp);
	execve (px->path_b, px->cmd_b, envp);
	free_mem (px);
	exit (1);
}
