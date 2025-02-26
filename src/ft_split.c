/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gribeiro <gribeiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:46:58 by gribeiro          #+#    #+#             */
/*   Updated: 2025/02/25 23:07:05 by gribeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	words(const char *s, char c)
{
	size_t	i;
	int		res;

	i = 0;
	res = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
			res++;
		while (s[i] != '\0' && s[i] != c)
			i++;
	}
	return (res);
}

static char	*fill(const char *s, size_t i, size_t letter)
{
	char	*str;
	size_t	n;

	str = malloc((letter + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	n = 0;
	while (letter > 0)
	{
		str[n] = s[i];
		letter--;
		i++;
		n++;
	}
	str[n] = '\0';
	return (str);
}

static void	clean_mem(char **wordv, size_t word)
{
	while (word > 0)
	{
		free(wordv[word - 1]);
		word--;
	}
	free(wordv);
}

static char	**sep_word(char **wordv, const char *s, char c)
{
	size_t	i;
	size_t	letter;
	size_t	word;

	i = 0;
	word = 0;
	while (s[i] != '\0')
	{
		letter = 0;
		while (s[i] == c)
			i++;
		while (s[i + letter] != '\0' && s[i + letter] != c)
			letter++;
		if (letter > 0)
		{
			wordv[word] = fill (s, i, letter);
			if (wordv[word] == NULL)
				return (clean_mem (wordv, word), NULL);
			word++;
		}
		i += letter;
	}
	wordv[word] = NULL;
	return (wordv);
}

char	**ft_split(const char *s, char c)
{
	char	**wordv;

	if (s == NULL)
		return (NULL);
	wordv = malloc ((words(s, c) + 1) * sizeof(char *));
	if (wordv == NULL)
		return (NULL);
	if (sep_word (wordv, s, c) == NULL)
		return (NULL);
	return (wordv);
}
