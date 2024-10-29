/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:21:33 by rreimann          #+#    #+#             */
/*   Updated: 2024/10/29 18:51:32 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// 
int	add_read_characters_to_buffer(char **buffer, char *add_buffer, ssize_t num_of_chars)
{
	
}

// Here we pass in the pointer to the memory address,
// So that we can change the memory this pointer is pointing to
// 
int	read_into_buffer(int fd, char **buffer)
{
	ssize_t	read_status;
	char	*read_buffer[BUFFER_SIZE];
	int		add_status;

	read_status = read(fd, *read_buffer, BUFFER_SIZE);
	if (read_status == -1)
		return (-1);
	else if (read_status == 0)
		return (0);
	add_status = add_read_characters_to_buffer(buffer, read_buffer, read_status);
	if (add_status == -1)
		return (-1);
	return (add_status);
}

// Cuts string out of `buffer`. For the amount of characters specified
// Returns the string and removes that part from `buffer`
char	*extract_line_out_of_buffer(char **buffer, int length)
{
	return (NULL);
}

void	free_if_possible(char *freeable_thing)
{
	if (freeable_thing != NULL)
		free(freeable_thing);
}

char	*get_next_line(int fd)
{
	static char	**buffer;
	char		*line;
	int			next_line_status;

	// -1 if string isn't allocated, or new line was not found
	while (contains_new_line(*buffer) == -1)
	{
		next_line_status = read_into_buffer(fd, buffer);
		// -1 means that next line did not work
		// 0 means that we reached the end of file
		if (next_line_status == -1)
		{
			free_if_possible(*buffer);
			return (NULL);
		}
		// What if buffer has 0 characters to begin with, because the last line was read in exactly
		// 
		else if (next_line_status == 0)
		{
			line = extract_line_out_of_buffer(buffer, next_line_status);
			free_if_possible(*buffer);
			if (line == NULL)
				return (NULL);
			return (line);
		}
	}

	// Check if we have the end of line in the `buffer`
	// If we have the end of line, cut out a string until the end as `line`
	// Remove that section until the END from the `buffer`
	// And return the `line`

	// If we don't have `END`...
	// Read the BUFFER_SIZE bytes from the file
	// Repeat the same thing until EOF
	// Save them into the buffer
}

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*next_line;
	int		index;

	fd = open("file.txt", O_RDONLY);
	index = 0;
	while (index < 5)
	{
		next_line = get_next_line(fd);
		printf("Line: '%s'\n", next_line);
	}
	close(fd);
	return (0);
}
