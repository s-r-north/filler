/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_ints.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 11:47:49 by snorth            #+#    #+#             */
/*   Updated: 2017/08/11 19:58:06 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*uswitch(va_list ap, t_mods *sm)
{
	uintmax_t	n;

	n = 0;
	if (!(sm->len))
		n = va_arg(ap, unsigned int);
	else if (sm->len & 1 << Z)
		n = va_arg(ap, size_t);
	else if (sm->len & 1 << J)
		n = va_arg(ap, uintmax_t);
	else if (sm->len & 1 << D)
		n = va_arg(ap, unsigned long long);
	else if (sm->len & 1 << L)
		n = va_arg(ap, unsigned long);
	else if (sm->len & 1 << H)
		n = (unsigned short)va_arg(ap, int);
	else if (sm->len & 1 << C)
		n = (unsigned char)va_arg(ap, int);
	if (n == 0)
		sm->fl |= (1 << nul);
	return (ft_umaxtoa_base(n, sm->base));
}

char	*ft_imaxtoa_pf(intmax_t n)
{
	intmax_t	nb;
	size_t		len;
	int			sign;
	char		*ret;

	len = 1;
	nb = n;
	while ((nb /= 10))
		len++;
	if (!(ret = ft_strnew(len)))
		return (NULL);
	if (n == 0)
		ret[0] = '0';
	sign = n < 0 ? -1 : 1;
	while (n)
	{
		ret[--len] = '0' + ((n % 10) * sign);
		n /= 10;
	}
	return (ret);
}

char	*iswitch(va_list ap, t_mods *sm)
{
	intmax_t	n;

	n = 0;
	if (!(sm->len))
		n = va_arg(ap, int);
	else if (sm->len & 1 << Z)
		n = va_arg(ap, ssize_t);
	else if (sm->len & 1 << J)
		n = va_arg(ap, intmax_t);
	else if (sm->len & 1 << D)
		n = va_arg(ap, long long);
	else if (sm->len & 1 << L)
		n = va_arg(ap, long);
	else if (sm->len & 1 << H)
		n = (short)va_arg(ap, int);
	else if (sm->len & 1 << C)
		n = (char)va_arg(ap, int);
	if (n < 0)
		sm->fl |= (1 << neg);
	if (n == 0)
		sm->fl |= (1 << nul);
	return (ft_imaxtoa_pf(n));
}

void	precision_size(t_mods *sm, size_t len)
{
	size_t	base;
	size_t	prec;

	base = sm->base;
	prec = (sm->pre < 0 ? len : (size_t)sm->pre);
	if (base == 16 && CHKFL(sm->fl, hash) &&
			(!CHKFL(sm->fl, nul) || sm->c == 'p'))
		prec = prec < len ? len + 2 : prec + 2;
	if (base == 8 && CHKFL(sm->fl, hash) && prec < len + 1)
		prec = (CHKFL(sm->fl, nul) && !CHKFL(sm->fl, precis) ? len : len + 1);
	if (base == 2 && len % 8 && len)
		prec = len + 8 - (len % 8);
	if (ft_strchr("dDi", sm->c) && len &&
			(sm->fl & ((1 << neg) + (1 << plus) + (1 << space))))
		prec = prec < len ? len + 1 : prec + 1;
	if ((sm->pre < 0 || !CHKFL(sm->fl, precis)) && sm->wid >= 0 &&
			CHKFL(sm->fl, zero))
		prec = ((size_t)sm->wid > len && (size_t)sm->wid > prec) ?
			(size_t)sm->wid : prec;
	sm->pre = (int)prec;
	sm->fl |= ((prec > 0) << precis);
}

char	*precision_str(t_mods *sm, size_t length, char *num)
{
	char	*ret;

	ret = ft_strnew(sm->pre);
	ft_memset(ret, '0', sm->pre - length);
	if (sm->base == 16 && (sm->fl & (1 << hash)))
		ret[1] = sm->c == 'X' ? 'X' : 'x';
	if (sm->base == 10 && CHKFL(sm->fl, neg))
		ret[0] = '-';
	else if (sm->base == 10 && (CHKFL(sm->fl, space) || CHKFL(sm->fl, plus)))
		ret[0] = CHKFL(sm->fl, plus) ? '+' : ' ';
	if (length)
		ft_memmove(ret + (sm->pre - length), num, length);
	ft_str_clrdel(&num);
	return (ret);
}
