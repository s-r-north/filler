/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 11:47:49 by snorth            #+#    #+#             */
/*   Updated: 2017/08/11 19:58:06 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_strupper(char *s)
{
	while (*s)
	{
		*s = ft_toupper(*s);
		s++;
	}
}

void	zero_mods(t_mods *sm)
{
	sm->c = 0;
	sm->fl = 0;
	sm->len = 0;
	sm->wid = 0;
	sm->pre = 0;
	sm->base = 0;
}

int		ft_memreallocat(char **s1, char *s2, int len1, int len2)
{
	if (!len2)
		return (0);
	if (!(*s1 = (char*)ft_realloc(*s1, len1 + len2 + 1)))
		return (-1);
	ft_memmove((*s1) + len1, s2, len2);
	return (len2);
}

void	ft_str_clrdel(char **s)
{
	if (!s)
		return ;
	ft_strclr(*s);
	ft_strdel(s);
}

size_t	find_base(char c)
{
	if (ft_strchr("dDiuUeEfFgG", c))
		return (10);
	if (ft_strchr("pxXaA", c))
		return (16);
	if (c == 'o' || c == 'O')
		return (8);
	if (c == 'b')
		return (2);
	return (0);
}
