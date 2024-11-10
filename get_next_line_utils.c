/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:21:56 by rreimann          #+#    #+#             */
/*   Updated: 2024/11/10 16:26:54 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	get_new_line_index(char *str, int len)
{
	int	index;

	index = 0;
	if (str == NULL)
		return (-1);
	while (index < len)
	{
		if (str[index] == 0)
			return (-1);
		if (str[index] == '\n')
			return (index);
		index++;
	}
	return (-1);
}

size_t	string_length(char *str, size_t max_len)
{
	size_t	length;

	if (str == NULL)
		return (0);
	length = 0;
	while (str[length] != 0)
	{
		if (max_len != (size_t)-1 && length >= max_len)
			return (length);
		length++;
	}
	return (length);
}

// Return -1 if anything fails
// Otherwise return the number of characters that are now in the buffer
// We expect buffer not to be NULL
size_t	add_read_characters_to_buffer(char **buffer, char *add_buffer, size_t add_buffer_length)
{
	char	*new_buffer;
	size_t	old_buffer_length;
	size_t	index;

	if (add_buffer == NULL || buffer == NULL)
		return (-1);
	old_buffer_length = string_length(*buffer, -1);
	// So we allocate for the new buffer
	size_t memory_allocation_size = old_buffer_length + add_buffer_length + 1;
	new_buffer = (char *)malloc(sizeof(char) * memory_allocation_size);
	if (new_buffer == NULL)
		return (-1);
	// Add things from the old buffer
	index = 0;
	while (index < old_buffer_length)
	{
		new_buffer[index] = (*buffer)[index];
		index++;
	}
	// Add things from the add buffer
	index = 0;
	while (index < add_buffer_length)
	{
		new_buffer[old_buffer_length + index] = add_buffer[index];
		index++;
	}
	// Add the null terminator
	new_buffer[old_buffer_length + add_buffer_length] = 0;
	// Finally do the memory things and return
	free(*buffer);
	*buffer = new_buffer;
	return (old_buffer_length + add_buffer_length);
}

static void	set_remaining_buffer(char main_buffer[BUFFER_SIZE], char *temp_buffer)
{
	int		new_line_index;
	int		temp_buffer_length;
	int		index;

	index = 0;
	while (index < BUFFER_SIZE)
		main_buffer[index++] = 0;
	temp_buffer_length = string_length(temp_buffer, -1);
	new_line_index = get_new_line_index(temp_buffer, temp_buffer_length);
	if (new_line_index == -1)
		new_line_index = temp_buffer_length - 1;
	index = 0;
	// "Thank you for this amazing evaluation\nIt w" => 37, length: 42
	//printf(" - Setting remaining characters for the main_buffer...\n");
	while (index + new_line_index + 1 < temp_buffer_length)
	{
		//printf(":temp_buffer[%d]: %c\n", index, temp_buffer[index + new_line_index + 1]);
		main_buffer[index] = temp_buffer[index + new_line_index + 1];
		index++;
	}
}

// This should put the two buffers together and return the line
// Keep the remaining buffer in the main buffer
char	*compose_line_from_buffers(char main_buffer[BUFFER_SIZE], char *temp_buffer)
{
	char	*line;
	int		main_new_line_index;
	int		temp_new_line_index;

	if (string_length(main_buffer, BUFFER_SIZE) == 0 && string_length(temp_buffer, -1) == 0)
		return (NULL);

	// Allocate memory for the empty line
	line = (char *)malloc(sizeof(char));
	if (line == NULL)
		return (NULL);
	line[0] = 0;

	main_new_line_index = get_new_line_index(main_buffer, BUFFER_SIZE);
	temp_new_line_index = get_new_line_index(temp_buffer, string_length(temp_buffer, -1));
	// If there is a new line in the main buffer
	if (main_new_line_index != -1)
	{
		add_read_characters_to_buffer(&line, main_buffer, main_new_line_index + 1);
		// Zero out the main buffer
		main_buffer[main_new_line_index] = 0;
		return (line);
	}
	else if (temp_new_line_index != -1)
	{
		add_read_characters_to_buffer(&line, main_buffer, BUFFER_SIZE);
		add_read_characters_to_buffer(&line, temp_buffer, temp_new_line_index + 1);
		set_remaining_buffer(main_buffer, temp_buffer);
		return (line);
	}
	else
	{
		add_read_characters_to_buffer(&line, main_buffer, BUFFER_SIZE);
		add_read_characters_to_buffer(&line, temp_buffer, string_length(temp_buffer, -1));
		set_remaining_buffer(main_buffer, temp_buffer);
		return (line);
	}
	return (free(line), NULL);
}
