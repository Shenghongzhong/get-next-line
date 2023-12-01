/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:24:20 by szhong            #+#    #+#             */
/*   Updated: 2023/12/01 22:19:07 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"
#include "../libft/libft.h"

char	*get_next_line(int fd)
{
	char	*buffer;
	size_t	bytes;
	char	*temp;
	char	*result;
	static char	*extra_buff;

	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (buffer == NULL)
		return (NULL);
	temp = NULL;
	if (extra_buff)
	{
		temp = extra_buff;
		extra_buff = NULL;
	}
	while (1)
	{
		if (temp && ft_strchr(temp, 10))
			break;
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == 0)
			return (0);
		buffer[bytes] = '\0';
		if (!temp)
			temp = ft_strdup(buffer);
		else
			temp = ft_strjoin(temp, buffer);
	}
	extra_buff = ft_strdup(ft_strrchr(temp, 10) + 1);
	result = ft_substr(temp, 0, ft_strchr(temp, 10) - temp);
	free(buffer);
	free(temp);
	return (result);
}
