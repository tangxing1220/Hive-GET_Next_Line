/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xtang <xtang@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/30 18:34:21 by xtang             #+#    #+#             */
/*   Updated: 2020/08/05 14:27:22 by xtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

static int	check_line(char **bufstack, char **line)
{
	int		i;
	int		len;
	char	*tmpstack;

	i = 0;
	len = ft_strlen(*bufstack);
	while (bufstack[0][i] != '\0')
	{
		if (bufstack[0][i] == '\n')
		{
			if (i == (len - 1))
				tmpstack = NULL;
			else
				tmpstack = ft_strdup(*bufstack + i + 1);
			*line = ft_strsub(*bufstack, 0, i);
			ft_strdel(bufstack);
			*bufstack = tmpstack;
			return (1);
		}
		i++;
	}
	return (0);
}

static int	read_to_end(int rd_result, char **line, char **bufstack)
{
	int result;

	if (rd_result == -1)
		result = -1;
	else if (rd_result == 0)
	{
		*line = *bufstack;
		result = 0;
	}
	else if (rd_result > 0 && rd_result < BUFF_SIZE)
	{
		if (*bufstack == NULL)
			result = 0;
		else
			result = 1;
	}
	else
		result = 1;
	return (result);
}

static int	read_buff(int fd, char *bufheap, char **bufstack, char **line)
{
	int		rd_result;
	char	*temp_stack;
	int		status;

	while ((rd_result = read(fd, bufheap, BUFF_SIZE)) > 0)
	{
		bufheap[rd_result] = '\0';
		if (*bufstack)
		{
			temp_stack = *bufstack;
			*bufstack = ft_strjoin(temp_stack, bufheap);
			free(temp_stack);
			temp_stack = NULL;
		}
		else
		{
			*bufstack = ft_strdup(bufheap);
		}
		if (check_line(bufstack, line))
		{
			break ;
		}
	}
	status = read_to_end(rd_result, line, bufstack);
	return (status);
}

int			get_next_line(const int fd, char **line)
{
	int			result;
	static char	*buf_stack;
	char		*buf_heap;

	if (!(buf_heap = (char *)malloc(BUFF_SIZE + 1)))
		return (-1);
	if (buf_stack)
	{
		if (check_line(&buf_stack, line))
		{
			free(buf_heap);
			return (1);
		}
	}
	result = read_buff(fd, buf_heap, &buf_stack, line);
	free(buf_heap);
	buf_heap = NULL;
	return (result);
}
