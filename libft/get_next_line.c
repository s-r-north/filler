/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/15 02:41:37 by snorth            #+#    #+#             */
/*   Updated: 2017/04/15 02:41:39 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		final_return(int ret, char **block, char ***line, char *tmp)
{
	if (ret < 0)
		return (-1);
	if (ft_strlen(tmp) > 0)
	{
		**line = tmp;
		*block = NULL;
		return (1);
	}
	return (0);
}

int				get_next_line(const int fd, char **line)
{
	int				ret;
	static char		*block[MAX_FD];
	char			*tmp;
	char			*str;
	char			buff[BUFF_SIZE + 1];

	if (fd < 0 || !line || BUFF_SIZE < 1 || fd > MAX_FD)
		return (-1);
	block[fd] = (!block[fd]) ? ft_strnew(BUFF_SIZE) : block[fd];
	tmp = ft_strncpy(ft_memalloc(BUFF_SIZE), block[fd], BUFF_SIZE);
	while (!(ft_strchr(tmp, '\n')))
	{
		if ((ret = read(fd, buff, BUFF_SIZE)) < 1)
			return (final_return(ret, &block[fd], &line, tmp));
		buff[ret] = '\0';
		str = ft_strjoin(tmp, buff);
		free(tmp);
		tmp = str;
	}
	*line = ft_strsub(tmp, 0, ft_strlen(tmp) - ft_strlen(ft_strchr(tmp, '\n')));
	if ((block[fd]) && (ft_strchr(tmp, '\n')))
		block[fd] = ft_strncpy(block[fd], ft_strchr(tmp, '\n') + 1, BUFF_SIZE);
	free(tmp);
	return (1);
}
