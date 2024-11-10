/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:21:58 by rreimann          #+#    #+#             */
/*   Updated: 2024/11/10 17:52:04 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

char	*get_next_line(int fd);
int		get_new_line_index(char *str, int len);
size_t	string_length(char *str, size_t max_len);
size_t	add_to_buffer(char **buffer, char *add_buffer,
			size_t add_buffer_length);
char	*compose_line_from_buffers(char main_buffer[BUFFER_SIZE],
			char *temp_buffer);
char	*return_line(char *line, char main_buffer[BUFFER_SIZE],
			char *temp_buffer, size_t length);
void	set_remaining_buffer(char main_buffer[BUFFER_SIZE],
			char *temp_buffer);

#endif
