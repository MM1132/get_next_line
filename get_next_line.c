/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:21:33 by rreimann          #+#    #+#             */
/*   Updated: 2024/11/09 16:30:17 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Test imports
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include "get_next_line.h"

// Return value: NULL means that something fucked up
// We assume that *buffer is not NULL
char	*remove_until_index(char **buffer, int new_line_index)
{
	ssize_t	buffer_length;
	ssize_t	new_length;
	char	*new_buffer;
	ssize_t	index;

	buffer_length = string_length(*buffer);
	new_length = buffer_length - new_line_index - 1;
	new_buffer = malloc(sizeof(char) * (new_length + 1));
	if (new_buffer == NULL)
		return (NULL);
	index = 0;
	while (index < new_length)
	{
		new_buffer[index] = (*buffer)[buffer_length - new_length + index];
		index++;
	}
	new_buffer[index] = 0;
	return (free(*buffer), new_buffer);
}

// Cuts string out of `buffer`. For the amount of characters specified
// Returns the string and removes that part from `buffer`
char	*extract_line_out_of_buffer(char **buffer, int new_line_index)
{
	char	*line;
	ssize_t	index;

	if (buffer == NULL)
		return (NULL);
	// "tere/nhead aega" index 4, but length 6
	// +2 for the new line itself, and the null terminator too
	line = malloc(sizeof(char) * (new_line_index + 2));
	if (line == NULL)
		return (NULL);
	index = 0;
	while (index < new_line_index + 1)
	{
		line[index] = (*buffer)[index];
		index++;
	}
	line[index] = 0;
	*buffer = remove_until_index(buffer, new_line_index);
	if (*buffer == NULL)
		return (free(line), NULL);
	return (line);
}

char	*add_buffers(char main_buffer[BUFFER_SIZE], char *temp_buffer)
{
	char	*new_buffer;
	int		main_buffer_new_line;
	int		temp_buffer_new_line;
	int		main_buffer_index;
	int		temp_buffer_index;

	//! This could lead to an error, because if the main buffer has no new line, the return will be 0
	main_buffer_new_line = main_buffer_contains_new_line(main_buffer);
	temp_buffer_new_line = temp_buffer_contains_new_line(temp_buffer);
	// "123\n0000" -> 3
	// "adadad\n" -> 6
	new_buffer = (char *)malloc(sizeof(char) * (main_buffer_new_line + temp_buffer_new_line + 1));
	if (new_buffer == NULL)
		return (NULL);
	main_buffer_index = 0;
	while (main_buffer_index < main_buffer_new_line)
	{
		new_buffer[main_buffer_index] = main_buffer[main_buffer_index];
		main_buffer_index++;
	}
	temp_buffer_index = 0;
	while (temp_buffer_index < temp_buffer_new_line)
	{
		new_buffer[main_buffer_index + temp_buffer_index] = temp_buffer[temp_buffer_index];
		temp_buffer_index++;
	}
	new_buffer[main_buffer_index + temp_buffer_index] = 0;
	return (new_buffer);
}

int	set_everything_after_first_line_into_main_buffer(char main_buffer[BUFFER_SIZE], char *added_buffers)
{
	int	main_buffer_index;

	main_buffer_index = 0;
	while (main_buffer_index < BUFFER_SIZE)
	{

	}
}

char	*compose_line_from_buffers(char main_buffer[BUFFER_SIZE], char *temp_buffer)
{
	char	*added_buffers;
	char	*line;
	int		main_buffer_set_result;

	printf("main_buffer: '%s'\n", main_buffer);
	printf("temp_buffer: '%s'\n", temp_buffer);
	
	// 1. Add main_buffer and temp_buffer together into one string
	added_buffers = add_buffers(main_buffer, temp_buffer);
	if (added_buffers == NULL)
		return (NULL);
	// 2. Get the line out of that string until the \n character
	line = extract_first_line(added_buffers);
	if (line == NULL)
		return (free(added_buffers), NULL);
	// 3. Keep the remaining characters in the main_buffer
	main_buffer_set_result = set_everything_after_first_line_into_main_buffer(main_buffer, added_buffers);
	if (main_buffer_set_result == -1)
		return (free(added_buffers), NULL);
	free(added_buffers);
	return (line);
}

char	*general_checking_loop(int fd, char main_buffer[BUFFER_SIZE])
{
	char	read_buffer[BUFFER_SIZE];
	char	*temp_buffer;
	char	*line;
	ssize_t	read_status;
	size_t	add_status;

	temp_buffer = (char *)malloc(sizeof(char));
	if (temp_buffer == NULL)
		return (NULL);
	temp_buffer[0] = 0; // Set the null terminator for the empty string
	while (1)
	{
		printf(" - While loop starting\n");
		if (get_new_line_index(main_buffer, BUFFER_SIZE) != -1 || get_new_line_index(temp_buffer, string_length(temp_buffer) != -1))
		{
			printf(" - Either of the buffers contains a new line\n");
			line = compose_line_from_buffers(&main_buffer, temp_buffer);
			return (free(temp_buffer), line);
		}
		else // if we don't have a new line in either buffer, read in something new with the read() function
		{
			printf(" - Buffers didn't contain new lines, reading...\n");
			read_status = read(fd, read_buffer, BUFFER_SIZE);
			if (read_status == 0) // we read nothing new
			{
				printf(" - We read 0 bytes\n");
				// This function will automatically return NULL if buffer is empty
				return (compose_line_from_buffers(main_buffer, NULL));
			}
			if (read_status == -1) // in case of a read error
			{
				printf(" - There was a read error\n");
				return (free(temp_buffer), NULL);
			}
			// But if everything went according to the plan
			// Then we take whatever we read and add it to the *temp_buffer
			add_status = add_read_characters_to_buffer(&temp_buffer, read_buffer, (size_t)read_status);
			if (add_status == (size_t)-1)
				return (free(temp_buffer), NULL);
			printf(" - Addig to the temp_buffer worked\n");
			printf(":temp_buffer: '%s'\n", temp_buffer);
			
			// Then we can simply go for the cycle again
		}
	}
	return (free(temp_buffer), NULL);
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
