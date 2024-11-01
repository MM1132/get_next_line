/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:21:56 by rreimann          #+#    #+#             */
/*   Updated: 2024/10/30 15:33:26 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	contains_new_line(char *str)
{
	int	index;

	index = 0;
	if (str == NULL)
		return (-1);
	while (str[index] != 0)
	{
		if (str[index] == '\n')
			return (index);
		index++;
	}
	return (-1);
}

size_t	string_length(char *str)
{
	size_t	length;

	if (str == NULL)
		return (0);
	length = 0;
	while (str[length] != 0)
		length++;
	return (length);
}

// Return -1 if anything fails
// Otherwise return the number of characters that are now in the buffer
// We expect buffer not to be NULL
size_t	add_read_characters_to_buffer(char **buffer, char *add_buffer, ssize_t add_buffer_length)
{

	char	*new_buffer;
	size_t	old_buffer_length;
	size_t	index;

	if (add_buffer == NULL || buffer == NULL)
		return (-1);
	old_buffer_length = string_length(*buffer);
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
