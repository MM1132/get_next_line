/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:21:33 by rreimann          #+#    #+#             */
/*   Updated: 2024/11/10 17:51:14 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Test imports
#include <sys/stat.h>
#include <fcntl.h>

#include "get_next_line.h"

static char	*compose_and_free(char main_buffer[BUFFER_SIZE], char *temp_buffer)
{
	char	*line;

	line = compose_line_from_buffers(main_buffer, temp_buffer);
	return (free(temp_buffer), line);
}

static int	either_buffer_contains_new_line(char main_buffer[BUFFER_SIZE],
			char *temp_buffer)
{
	return (get_new_line_index(main_buffer, BUFFER_SIZE) != -1 || \
		get_new_line_index(temp_buffer, string_length(temp_buffer, -1)) != -1);
}

static char	*general_checking_loop(int fd, char main_buffer[BUFFER_SIZE])
{
	char	read_buffer[BUFFER_SIZE];
	char	*temp_buffer;
	ssize_t	read_status;

	temp_buffer = (char *)malloc(sizeof(char));
	if (temp_buffer == NULL)
		return (NULL);
	temp_buffer[0] = 0;
	while (1)
	{
		if (either_buffer_contains_new_line(main_buffer, temp_buffer))
			return (compose_and_free(main_buffer, temp_buffer));
		else
		{
			read_status = read(fd, read_buffer, BUFFER_SIZE);
			if (read_status >= 0 && read_status <= BUFFER_SIZE)
				if (add_to_buffer(&temp_buffer,
						read_buffer, (size_t)read_status) == (size_t) - 1)
					return (free(temp_buffer), NULL);
			if (read_status >= 0 && read_status < BUFFER_SIZE)
				return (compose_and_free(main_buffer, temp_buffer));
			else if (read_status == -1)
				return (free(temp_buffer), NULL);
		}
	}
}

char	*return_line(char *line, char main_buffer[BUFFER_SIZE],
			char *temp_buffer, size_t length)
{
	add_to_buffer(&line, main_buffer, BUFFER_SIZE);
	add_to_buffer(&line, temp_buffer, length);
	set_remaining_buffer(main_buffer, temp_buffer);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	main_buffer[BUFFER_SIZE];
	int			index;
	char		*line;

	line = general_checking_loop(fd, main_buffer);
	if (line == NULL)
	{
		index = 0;
		while (index < BUFFER_SIZE)
			main_buffer[index++] = 0;
		return (NULL);
	}
	return (line);
}
