/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:24:20 by szhong            #+#    #+#             */
/*   Updated: 2024/01/09 19:04:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

static char	*init_stash(char **extra_buff);
static char	*read_and_extract(int fd, char *stash, char *buff);
static char	*clean_stash(char *stash, char **extra_buff);
static void	free_up(char *stash, char *buff);

/**
 * @brief Read the next line from a file descriptor.
 *
 * This function reads a line from the specificed file descriptor (fd).
 * It uses a buffer to store the read data and a static variable
 * to keep track of any remaining characters from the previous call.
 *
 * buff for buffer for reading data, stash for temporary for parital lines
 * result to be returned , extra_buff for remaining characters
 *
 * @param fd The file descriptor to read from.
 * @return	A dynamically allocated string containing the next line,
 * 		or NULL if an error occurs or the end of the file is reached
 */
char	*get_next_line(int fd)
{
	char		*buff;
	char		*stash;
	char		*result;
	static char	*extra_buff;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buff = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (buff == NULL)
		return (NULL);
	stash = init_stash(&extra_buff);
	while (1)
	{
		if (stash && get_strchr(stash, '\n'))
			break ;
		stash = read_and_extract(fd, stash, buff);
		if (!stash || get_strchr(stash, '\n'))
			break ;
	}
	result = clean_stash(stash, &extra_buff);
	free_up(stash, buff);
	return (result);
}

/**
 * @brief Initialise stash with any remaining characters from the previous call.
 *
 * This functin initialises the stash with any remaining characters stored in 
 * the previous extra_buff, only if the extra_buff is not empty, 
 * it assigns its value to the stash and sets the extra_buff to NULL.
 *
 * @parm extra_buff A pointer to the extra_buff containing remainin characters.
 * 		    This parameter is updated to NULL if the extra_buff is used
 * @return	A pointer to the initialised stash. If no characters are in 
 * 		the extra_buff,	the stash is initialised as NULL.
 */
static char	*init_stash(char **extra_buff)
{
	char	*stash;	

	stash = NULL;
	if (*extra_buff)
	{
		stash = *extra_buff;
		*extra_buff = NULL;
	}
	return (stash);
}

/**
 * @brief Read from the file descriptor into buff and stash
 *
 * This function reads and extracts content from a specified file descriptor;
 * It continues until a newline character (\n) is encountered or
 * when the read function reads the BUFFER_SIZE limit 
 *
 * @param fd A file descriptor from which content will be read.
 * @param stash A pointer to char to store content from the file descriptor
 * @param buff A pointer to temporarily store data from the read function
 * @return A pointer to the read stash. If nothing to read from fd,
 * 	   NULL is returned
 */
static char	*read_and_extract(int fd, char *stash, char *buff)
{
	ssize_t		bytes;
	char		*temp;

	bytes = read(fd, buff, BUFFER_SIZE);
	if (bytes <= 0)
		return (NULL);
	buff[bytes] = '\0';
	if (!stash)
	{
		stash = get_strdup(buff);
	}
	else
	{
		temp = stash;
		stash = get_strjoin(temp, buff);
		free(temp);
	}
	return (stash);
}

/**
 * @brief Remove content after the newline character and store for next call
 *
 * This function extracts characters before the newline characters in stach up to
 * the newline character, storing the remaining characters into static variable
 * extra_buff for the next call.
 *
 * @param	stash A pointer to a char type variable that read into from the loop
 * @param	extra_buff A static pointer variable to store characters after \n
 * @return	A dynamically allocated string for a complete line of content
 * 		with a newline character. If the stash is empty, NULL is returned
 * */
static char	*clean_stash(char *stash, char **extra_buff)
{
	char	*result;
	char	*newline_pos;

	result = NULL;
	newline_pos = get_strchr(stash, '\n');
	if (newline_pos)
	{
		*extra_buff = get_strdup(newline_pos + 1);
		result = get_substr(stash, 0, newline_pos + 1 - stash);
	}
	else if (stash)
	{
		result = get_strdup(stash);
	}
	return (result);
}

/**
 * @brief Free memory allocated for stash and buff.
 *
 * This function releases the memory allocated for the stash and buff parameters.
 *
 * @param stash	A pointer to a memory block allocated for storing content from fd
 * @param buff	A pointer to a memory block allocated for temporary data storage
 */
static void	free_up(char *stash, char *buff)
{
	free(stash);
	free(buff);
}
