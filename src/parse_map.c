/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2069/06/09 00:00:00 by snorth            #+#    #+#             */
/*   Updated: 2069/06/09 00:00:00 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../filler.h"

static t_map	*init_map(char *line)
{
	t_map	*map;

	map = ft_memalloc(sizeof(t_map));
	map->dim = get_dimensions(line);
	map->final_pt = map->dim.x * map->dim.y;
	map->st.x = map->dim.x;
	map->st.y = map->dim.y;
	map->end.x = -1;
	map->end.y = -1;
	map->board = ft_memalloc(map->final_pt + 1);
	map->heat = ft_memalloc(sizeof(int) * map->final_pt);
	map->place.x = 0;
	map->place.y = 0;
	map->piece_val = -1;
	return (map);
}

static void		set_line(t_filler *f, char *line, int y)
{
	int		x;

	x = 0;
	line += 4;
	while (*line)
	{
		if (ft_toupper(*line) == f->me)
		{
			f->map->st.x = x < f->map->st.x ? x : f->map->st.x;
			f->map->st.y = y < f->map->st.y ? y : f->map->st.y;
			f->map->end.x = x + 1 > f->map->end.x ? x + 1 : f->map->end.x;
			f->map->end.y = y + 1 > f->map->end.y ? y + 1 : f->map->end.y;
		}
		f->map->board[(y * f->map->dim.x) + x] = *line;
		f->map->heat[(y * f->map->dim.x) + x] =
				ft_toupper(*line) == f->opp ? -1 : 0;
		line++;
		x++;
	}
}

void			parse_map(t_filler *f)
{
	char	*line;
	int		y;

	line = NULL;
	get_next_line(STDIN_FILENO, &line);
	if (f->map == NULL)
		f->map = init_map(line);
	ft_strdel(&line);
	get_next_line(STDIN_FILENO, &line);
	ft_strdel(&line);
	y = 0;
	while (y < f->map->dim.y)
	{
		get_next_line(STDIN_FILENO, &line);
		set_line(f, line, y);
		ft_strdel(&line);
		y++;
	}
	f->map->msize.x = f->map->end.x - f->map->st.x;
	f->map->msize.y = f->map->end.y - f->map->st.y;
}
