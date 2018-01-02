/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   placement.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2069/06/09 00:00:00 by snorth            #+#    #+#             */
/*   Updated: 2069/06/09 00:00:00 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../filler.h"

static int	good_point(t_filler *f, t_xy pt, t_xy st, int *touch)
{
	if (f->tok->piece[(st.y + f->tok->st.y) * f->tok->dim.x + \
			st.x + f->tok->st.x] != '*')
		return (0);
	if (pt.x + st.x >= f->map->dim.x || pt.y + st.y >= f->map->dim.y)
		return (-1);
	if (f->map->heat[((pt.y + st.y) * f->map->dim.x) + pt.x + st.x] < 0)
		return (-1);
	if (f->map->board[((pt.y + st.y) * f->map->dim.x) + pt.x + st.x] == f->me
			&& ++(*touch) > 1)
		return (-1);
	return (f->map->heat[((pt.y + st.y) * f->map->dim.x) + pt.x + st.x]);
}

static int	check_piece(t_filler *f, t_xy pt)
{
	t_xy	st;
	int		val;
	int		touch;
	int		d;

	val = 0;
	touch = 0;
	st.y = 0;
	while (st.y < f->tok->psize.y)
	{
		st.x = 0;
		while (st.x < f->tok->psize.x)
		{
			if ((d = good_point(f, pt, st, &touch)) < 0)
				return (-1);
			val += d;
			st.x++;
		}
		st.y++;
	}
	if (touch != 1)
		return (-1);
	return (val);
}

static int	opp_dist(t_map *m, int x, int y)
{
	int max;
	int dist;
	int st;

	dist = 0;
	max = (m->dim.y > m->dim.x) ? m->dim.y : m->dim.x;
	while (dist < max)
	{
		st = x - dist - 1;
		while (++st <= x + dist)
			if (st >= 0 && st < m->dim.x && ((y - dist >= 0 &&
					m->heat[m->dim.x * (y - dist) + st] == -1) || (y + dist <
					m->dim.y && m->heat[m->dim.x * (y + dist) + st] == -1)))
				return (dist == 0 ? -1 : MAXHEAT / (1.1 * dist));
		st = y - dist;
		while (++st < y + dist)
			if (st >= 0 && st < m->dim.y && ((x - dist >= 0 &&
					m->heat[m->dim.x * st + x - dist] == -1) || (x + dist <
					m->dim.x && m->heat[m->dim.x * st + x + dist] == -1)))
				return (dist == 0 ? -1 : MAXHEAT / (1.1 * dist));
		dist++;
	}
	return (-1);
}

static void	populate_heatmap(t_map *map)
{
	int i;

	i = 0;
	while (i < map->final_pt)
	{
		map->heat[i] = opp_dist(map, i % map->dim.x, i / map->dim.x);
		i++;
	}
}

void		placement(t_filler *f)
{
	t_xy	i;
	int		val;

	populate_heatmap(f->map);
	i.y = (f->map->st.y - f->tok->psize.y - 1 < 0) ?
			0 : f->map->st.y - f->tok->psize.y - 1;
	while (i.y < f->map->end.y)
	{
		i.x = (f->map->st.x - f->tok->psize.x - 1 < 0) ?
				0 : f->map->st.x - f->tok->psize.x - 1;
		while (i.x < f->map->end.x)
		{
			if ((val = check_piece(f, i)) > f->map->piece_val)
			{
				f->map->piece_val = val;
				f->map->place.x = i.x - f->tok->st.x;
				f->map->place.y = i.y - f->tok->st.y;
			}
			i.x++;
		}
		i.y++;
	}
	ft_printf("%d %d\n", f->map->place.y, f->map->place.x);
	if (f->map->piece_val < 0)
		f->done = 69;
}
