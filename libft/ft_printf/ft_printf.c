/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 11:47:49 by snorth            #+#    #+#             */
/*   Updated: 2017/08/11 19:58:06 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_vasprintf(char **strp, const char *fmt, va_list ap)
{
	t_mods	*sm;
	int		i;
	int		n;

	n = 0;
	i = 0;
	sm = (t_mods*)malloc(sizeof(t_mods));
	*strp = ft_strnew(0);
	while (*fmt)
	{
		if (++i && *fmt++ == '%')
		{
			if ((i = ft_memreallocat(strp, (char*)fmt - i, n, i - 1)) < 0)
				break ;
			n += i;
			fmt += eval_modifiers((char*)fmt, sm, ap);
			if (!(i = 0) && sm->c)
				n += eval_data_type(sm, ap, strp, n);
		}
	}
	if (i < 0 || (i = ft_memreallocat(strp, (char*)fmt - i, n, i)) < 0)
		return (-1);
	*((*strp) + n + i) = 0;
	free(sm);
	return (n + i);
}

int		ft_asprintf(char **strp, const char *fmt, ...)
{
	va_list	ap;
	int		n;

	va_start(ap, fmt);
	n = ft_vasprintf(strp, fmt, ap);
	va_end(ap);
	return (n);
}

int		ft_printf(const char *fmt, ...)
{
	va_list	ap;
	char	*s;
	int		n;

	s = NULL;
	va_start(ap, fmt);
	n = ft_vasprintf(&s, fmt, ap);
	write(1, s, n);
	ft_str_clrdel(&s);
	va_end(ap);
	return (n);
}
