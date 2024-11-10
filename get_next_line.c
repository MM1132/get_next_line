/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:21:33 by rreimann          #+#    #+#             */
/*   Updated: 2024/11/10 16:26:14 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Test imports
#include <sys/stat.h>
#include <fcntl.h>

#include "get_next_line.h"

static char	*general_checking_loop(int fd, char main_buffer[BUFFER_SIZE])
{
	char	read_buffer[BUFFER_SIZE];
	char	*temp_buffer;
	char	*line;
	ssize_t	read_status;
	size_t	add_status;

	temp_buffer = (char *)malloc(sizeof(char));
	if (temp_buffer == NULL)
		return (NULL);
	temp_buffer[0] = 0;
	while (1)
	{
		//printf(" - While loop starting\n");
		if (get_new_line_index(main_buffer, BUFFER_SIZE) != -1 ||\
			get_new_line_index(temp_buffer, string_length(temp_buffer, -1)) != -1)
		{
			//printf(" - Either of the buffers contains a new line\n");
			//printf(":main_buffer: '%s'\n", main_buffer);
			//printf(":temp_buffer: '%s'\n", temp_buffer);

			//printf(" - Composing new line from buffers...\n");
			line = compose_line_from_buffers(main_buffer, temp_buffer);
			//printf(":main_buffer: '%s'\n", main_buffer);
			return (free(temp_buffer), line);
		}
		else
		{
			//printf(" - Buffers didn't contain new lines, reading...\n");

			read_status = read(fd, read_buffer, BUFFER_SIZE);
			// If reading worked, add characters to the temp buffer
			if (read_status >= 0 && read_status <= BUFFER_SIZE)
			{
				//printf(" - We read %zd bytes\n", read_status);

				add_status = add_read_characters_to_buffer(&temp_buffer, read_buffer, (size_t)read_status);
				if (add_status == (size_t)-1)
					return (free(temp_buffer), NULL);
				//printf(":temp_buffer after adding: '%s'\n", temp_buffer);
			}
			// If we read less than the BUFFER_SIZE, which means that we reached EOF
			// Then get the line and return it
			if (read_status >= 0 && read_status < BUFFER_SIZE)
			{
				//printf(" - We read less than BUFFER_SIZE, getting line...\n");

				line = compose_line_from_buffers(main_buffer, temp_buffer);
				return (free(temp_buffer), line);
			}
			// in case of a read error
			// return NULL
			else if (read_status == -1)
			{
				//printf(" - Oh no! There was a read error, returning NULL...\n");
				return (free(temp_buffer), NULL);
			}
		}
	}
}

char	*get_next_line(int fd)
{
	static char	main_buffer[BUFFER_SIZE];
	char		*line;

	line = general_checking_loop(fd, main_buffer);
	if (line == NULL)
		return (NULL);
	return (line);
}
