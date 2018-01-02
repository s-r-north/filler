/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 11:47:49 by snorth            #+#    #+#             */
/*   Updated: 2018/01/02 14:38:10 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <limits.h>
# include <stdarg.h>
# include "../libft.h"
# define ISFLAG(a) (a == '+' || a == '-' || a == ' ' || a == '#' || a == '0')
# define ISPREC(a) (a == '.')
# define ISLEN1(a) (a == 'h' || a == 'l' || a == 'L')
# define ISLEN2(a) (a == 'j' || a == 't' || a == 'z')
# define CHKFL(a, b) (a & 1 << b)

enum
{
	hash,
	plus,
	minus,
	zero,
	space,
	precis,
	neg,
	nul
}					t_flag;

enum
{
	false,
	true
}					t_bool;

enum
{
	C,
	H,
	L,
	D,
	J,
	T,
	Z
}					t_len;

typedef struct		s_mods
{
	char			c;
	unsigned short	fl;
	unsigned short	len;
	int				wid;
	int				pre;
	size_t			base;
}					t_mods;

int					ft_asprintf(char **strp, const char *fmt, ...);
int					ft_printf(const char *fmt, ...);
int					ft_vasprintf(char **strp, const char *fmt, va_list ap);
int					eval_data_type(t_mods *sm, va_list ap, char **s, int n);
int					eval_modifiers(char *s, t_mods *sm, va_list ap);

void				zero_mods(t_mods *sm);
char				*ft_multicat(char *dst, int c, size_t size);
void				ft_strupper(char *s);
void				ft_str_clrdel(char **s);
int					ft_memreallocat(char **s1, char *s2, int len1, int len2);

char				*short_char(va_list ap, t_mods *sm);
char				*short_str(va_list ap, t_mods *sm);
char				*long_str(va_list ap, t_mods *sm);
char				*store_n(va_list ap, int n);

size_t				find_base(char c);
void				precision_size(t_mods *sm, size_t len);
char				*int_nums(va_list ap, t_mods *sm);
char				*width_strs(char *num, t_mods *sm, size_t len);
char				*precision_str(t_mods *sm, size_t length, char *num);

char				*ft_umaxtoa_base(uintmax_t n, size_t base);
char				*uswitch(va_list ap, t_mods *sm);
char				*ft_imaxtoa_pf(intmax_t n);
char				*iswitch(va_list ap, t_mods *sm);

#endif
