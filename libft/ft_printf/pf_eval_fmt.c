/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_eval_fmt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 11:47:49 by snorth            #+#    #+#             */
/*   Updated: 2017/08/11 19:58:06 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		eval_data_type(t_mods *sm, va_list ap, char **s, int n)
{
	char	*end;

	if (ft_strchr("idDuUboOxXp", sm->c))
		end = int_nums(ap, sm);
	else if (sm->c == 's' && !(sm->len & L))
		end = short_str(ap, sm);
	else if (sm->c == 'n')
		end = store_n(ap, n);
	else
		end = short_char(ap, sm);
	if (!end)
		return (0);
	sm->wid *= sm->wid < 0 ? -1 : 1;
	*s = (char*)ft_realloc(*s, n + sm->wid);
	ft_memmove((*s) + n, end, sm->wid);
	ft_str_clrdel(&end);
	return (sm->wid);
}

void	set_flag(char *s, t_mods *sm, va_list ap)
{
	char	*p;
	int		n;

	p = "#+-0 .";
	if ((n = ft_strchr(p, *s) - p) < 0)
		return ;
	if (n == zero)
	{
		if ((*(s - 1) >= '0' && *(s - 1) <= '9') || *(s - 1) == '.')
			return ;
	}
	if (*s == '.')
		sm->pre = *(s + 1) == '*' ? va_arg(ap, int) : ft_atoi(s + 1);
	sm->fl |= 1 << n;
}

void	set_len(char *s, t_mods *sm)
{
	sm->len |= (*s == 'z') << Z;
	sm->len |= (*s == 't') << T;
	sm->len |= (*s == 'j') << J;
	sm->len |= (*s == 'L' || (*s == 'l' && *(s - 1) == 'l')) << D;
	sm->len |= (*s == 'l' && *(s + 1) != 'l') << L;
	sm->len |= (*s == 'h' && *(s + 1) != 'h' && *(s - 1) != 'h') << H;
	sm->len |= (*s == 'h' && *(s + 1) == 'h') << C;
}

int		eval_modifiers(char *s, t_mods *sm, va_list ap)
{
	int		i;

	i = 0;
	zero_mods(sm);
	while (*s)
	{
		++i;
		if (!ft_strchr("0123456789+-# ztjLlh.*", *s))
			break ;
		if (ISFLAG(*s) || ISPREC(*s))
			set_flag(s, sm, ap);
		else if (ISLEN1(*s) || ISLEN2(*s))
			set_len(s, sm);
		if (((*s >= '1' && *s <= '9' && (*(s - 1) < '1' ||
				*(s - 1) > '9')) || *s == '*') && *(s - 1) != '.')
			sm->wid = *s == '*' ? va_arg(ap, int) : ft_atoi(s);
		s++;
	}
	sm->wid *= ((sm->fl & 1 << minus) && sm->wid > 0) ? -1 : 1;
	sm->fl |= (sm->wid < 0) ? 1 << minus : 0;
	sm->c = *s;
	sm->pre *= (sm->pre < 0 && sm->c == 's') ? -1 : 1;
	return (i);
}
