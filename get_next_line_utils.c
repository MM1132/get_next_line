/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:21:56 by rreimann          #+#    #+#             */
/*   Updated: 2024/10/29 17:17:43 by rreimann         ###   ########.fr       */
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
