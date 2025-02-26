/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gribeiro <gribeiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:47:22 by gribeiro          #+#    #+#             */
/*   Updated: 2025/02/26 00:26:32 by gribeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*ft_strdup(const char *s1)
{
	char	*tmp_str;
	size_t	i;

	i = 0;
	tmp_str = (char *) malloc ((ft_strlen (s1) + 1) * sizeof(char));
	if (tmp_str == NULL)
	{
		return (NULL);
	}
	while (s1[i] != '\0')
	{
		tmp_str[i] = s1[i];
		i++;
	}
	tmp_str[i] = '\0';
	return (tmp_str);
}
