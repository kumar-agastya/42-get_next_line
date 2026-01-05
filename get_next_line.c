/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agastya-kumar <agastya-kumar@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:00:00 by agastya           #+#    #+#             */
/*   Updated: 2025/12/22 03:00:30 by agastya-kum      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_and_store(int fd, char *store)
{
	char	*buf;
	ssize_t	bytes;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	bytes = 1;
	while (!ft_strchr(store, '\n') && bytes > 0)
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes < 0)
		{
			free(buf);
			free(store);
			return (NULL);
		}
		buf[bytes] = '\0';
		store = ft_strjoin(store, buf);
	}
	free(buf);
	return (store);
}

static char	*extract_line(char *store)
{
	size_t	i;

	if (!store || !store[0])
		return (NULL);
	i = 0;
	while (store[i] && store[i] != '\n')
		i++;
	if (store[i] == '\n')
		i++;
	return (ft_substr(store, 0, i));
}

static char	*clean_store(char *store)
{
	size_t	i;
	size_t	len;
	char	*new;

	i = 0;
	while (store[i] && store[i] != '\n')
		i++;
	if (!store[i])
	{
		free(store);
		return (NULL);
	}
	len = ft_strlen(store + i + 1);
	if (len == 0)
	{
		free(store);
		return (NULL);
	}
	new = ft_substr(store, i + 1, len);
	free(store);
	return (new);
}


char	*get_next_line(int fd)
{
	static char	*store;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	store = read_and_store(fd, store);
	if (!store)
		return (NULL);
	line = extract_line(store);
	if (!line)
	{
		free(store);
		store = NULL;
		return (NULL);
	}
	store = clean_store(store);
	return (line);
}
