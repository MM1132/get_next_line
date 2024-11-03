/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:21:33 by rreimann          #+#    #+#             */
/*   Updated: 2024/11/03 19:27:35 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Test imports
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include "get_next_line.h"

// Here we pass in the pointer to the memory address,
// So that we can change the memory this pointer is pointing to
// int	read_into_buffer(int fd, char **buffer)
// {
// 	ssize_t	read_status;
// 	char	read_buffer[BUFFER_SIZE];
// 	size_t	add_status;

// 	read_status = read(fd, read_buffer, BUFFER_SIZE);
// 	if (read_status == -1)
// 		return (-1);
// 	else if (read_status == 0)
// 		return (0);
// 	add_status = add_read_characters_to_buffer(buffer, read_buffer, (size_t)read_status);
// 	if (add_status == (size_t)-1)
// 		return (-1);
// 	return (add_status);
// }

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

// Here we already definitely know that the buffer is defined
// So we just get the line out of the read function
char	*general_checking_loop(int fd, char **buffer)
{
	char	read_buffer[BUFFER_SIZE];
	ssize_t	read_status;
	size_t	add_status;

	while (1)
	{
		printf("Start of while\n");
		if (buffer == NULL)
		{
			printf("Buffer is NULL\n");
			read_status = read(fd, read_buffer, BUFFER_SIZE);
			if (read_status == 0 || read_status == -1)
				return (NULL);
			buffer = (char **)malloc(sizeof(char *));
			if (buffer == NULL)
				return (NULL);
			*buffer = 0;
			add_status = add_read_characters_to_buffer(buffer, read_buffer, (size_t)read_status);
			if (add_status == (size_t)-1)
				return (free(buffer), NULL);
		}
		else if (contains_new_line(*buffer))
		{
			char *line = extract_line_out_of_buffer(buffer, contains_new_line(*buffer));
			printf("Buffer after cut: '%s'\n", *buffer);
			return (line);
		}
		else
		{
			printf("Buffer in else: '%s'\n", *buffer);
			read_status = read(fd, read_buffer, BUFFER_SIZE);
			if (read_status == 0)
			{
				if (string_length(*buffer) == 0)
					return (free(buffer), NULL);
				return (extract_line_out_of_buffer(buffer, string_length(*buffer)));
			}
			else if (read_status == -1)
				return (free(buffer), NULL);
			add_status = add_read_characters_to_buffer(buffer, read_buffer, (size_t)read_status);
			if (add_status == (size_t)-1)
				return (free(buffer), NULL);
		}
	}
}

char	*get_next_line(int fd)
{
	static char	**buffer;
	char		*line;

	line = general_checking_loop(fd, buffer);
	printf("Buffer after line return: '%s'\n", *buffer);
	if (line == NULL)
		return (NULL);
	return (line);
}

int	main(void)
{
	char	*line;

	// We read the file and get the file descriptor of it
	int fd = open("file.txt", O_RDONLY);

	while (1) {
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		printf("Line found: '%s'", line);
	}

	// // Allocate memory for the buffer string, 1 means that there will be room only for the null terminator
	// char *buffer_string = (char *)malloc(sizeof(char) * 1);
	// // Set the null terminator
	// buffer_string[0] = 0;
	// // Create a buffer and assign the biffer_string's memory address for its value
	// char **buffer = &buffer_string;

	// // Loop through 8 times
	// for (int i = 0; i < 18; i++) {
	// 	// You read the bytes
	// 	int read_into_buffer_status = read_into_buffer(fd, buffer);
	// 	// Error handling
	// 	if (read_into_buffer_status == -1)
	// 	{
	// 		printf("read_into_buffer_status was -1\n");
	// 		break;
	// 	}
	// 	// Print the current buffer
	// 	// Check if the buffer now contains a new line
	// 	int new_line_index = contains_new_line(*buffer);
	// 	printf("%3d: ", new_line_index);
	// 	printf("'%s'\n", *buffer);
	// 	// If we DO have a new line within the buffer, cut off the line from the buffer until the new line character
	// 	if (new_line_index != -1)
	// 	{
	// 		char *line = extract_line_out_of_buffer(buffer, new_line_index);
	// 		printf("Line found: '%s'\n", line);
	// 		printf("Buffer after: '%s'\n", *buffer);
	// 	}
	// }

	// free(buffer_string);
	return (0);
}
