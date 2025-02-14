/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gribeiro <gribeiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:39:06 by gribeiro          #+#    #+#             */
/*   Updated: 2025/02/13 21:08:01 by gribeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv)
{
	int	n;
	int	fd[2];

	n = argc - 1;
	while (n > 0)
		if (argc != 5 || argv[n--][0] == '\0')
			return (write (1, "Invalid arguments.\n", 19), 1);
	fd[0] = open (argv[1], O_RDONLY);
	fd[1] = open (argv[4], O_RDWR);
	if (fd[0] < 0 || fd[1] < 0)
		return (write (1, "Error loading file(s).\n", 24), 1);
	if 	(pipe (fd) == -1)
		return (write (1, "Pipe creation failed.\n", 23), 1);
	close(fd[0]);
	close(fd[1]);
	return (0);
}
