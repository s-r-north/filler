/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_conversions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 11:47:49 by snorth            #+#    #+#             */
/*   Updated: 2017/08/11 19:58:06 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*store_n(va_list ap, int n)
{
	int	*s;

	s = va_arg(ap, int*);
	*s = n;
	return (NULL);
}

char	*short_char(va_list ap, t_mods *sm)
{
	char	*p;
	char	c;

	if (!sm->wid)
		sm->wid = 1;
	p = ft_strnew(sm->wid * (sm->wid < 0 ? -1 : 1));
	if (sm->wid * (sm->wid < 0 ? -1 : 1) > 1)
		ft_memset(p, (CHKFL(sm->fl, zero) && sm->wid >= 0) ?
			'0' : ' ', sm->wid * (sm->wid < 0 ? -1 : 1));
	if (sm->c == 'c' || sm->c == 'C' || sm->c == 'S')
		c = (char)va_arg(ap, int);
	else
		c = sm->c;
	if (sm->wid == 1 || sm->wid < 0)
		p[0] = c;
	else
		p[sm->wid - 1] = c;
	return (p);
}

char	*short_str(va_list ap, t_mods *sm)
{
	char	*s;
	char	*p;
	int		i;

	if ((s = va_arg(ap, char*)))
		i = (!CHKFL(sm->fl, precis) || ft_strlen(s) <= (size_t)sm->pre) ?
			ft_strlen(s) : sm->pre;
	else
		i = (CHKFL(sm->fl, precis) && !sm->pre) ? 0 : 6;
	p = ft_strnew(i);
	ft_memmove(p, s ? s : "(null)", i);
	return (width_strs(p, sm, i));
}

char	*int_nums(va_list ap, t_mods *sm)
{
	size_t	length;
	char	*num;

	sm->base = find_base(sm->c);
	if (ft_strchr("DUOp", sm->c))
		sm->len |= (1 << (sm->c == 'p' ? Z : L));
	if (sm->c == 'p')
		sm->fl |= (1 << hash);
	if (!(num = (ft_strchr("dDi", sm->c) ? iswitch(ap, sm) : uswitch(ap, sm))))
		return (NULL);
	if (sm->c == 'X')
		ft_strupper(num);
	length = ((!(sm->pre) && CHKFL(sm->fl, precis) &&
			CHKFL(sm->fl, nul)) ? 0 : ft_strlen(num));
	precision_size(sm, length);
	if (!CHKFL(sm->fl, precis) || (size_t)sm->pre <= length)
		return (width_strs(num, sm, length));
	num = precision_str(sm, length, num);
	return (width_strs(num, sm, ft_strlen(num)));
}

char	*width_strs(char *s, t_mods *sm, size_t len)
{
	char	*out;
	int		sign;

	sign = sm->wid < 0 ? -1 : 1;
	sm->wid *= sign;
	if ((size_t)sm->wid <= len)
	{
		sm->wid = (int)len;
		return (s);
	}
	if (sign == -1)
	{
		s = ft_realloc(s, sm->wid);
		ft_memset(s + len, ' ', sm->wid - len);
		return (s);
	}
	out = ft_strnew(sm->wid);
	if ((!sm->pre) && CHKFL(sm->fl, zero))
		ft_memset(out, '0', sm->wid - len);
	else
		ft_memset(out, ' ', sm->wid - len);
	ft_memmove(out + (sm->wid - len), s, len);
	ft_str_clrdel(&s);
	return (out);
}
