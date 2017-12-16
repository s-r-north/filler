#include "filler.h"

t_xy		get_dimensions(char *line)
{
	t_xy i;

	while (*line < '0' || *line > '9')
		line++;
	i.y = ft_atoi(line);
	while (*line >= '0' && *line <= '9')
		line++;
	i.x = ft_atoi(line);
	return (i);
}

void	trim_chpiece(t_token *tok)
{
	t_xy i;

	tok->st.x = -1;
	tok->st.y = -1;
	tok->end.x = 0;
	tok->end.y = 0;
	i.y = 0;
	while (i.y < tok->dim.y)
	{
		i.x = 0;
		while (i.x < tok->dim.x)
		{
			if (tok->piece_c[(i.y * tok->dim.x) + i.x] == '*')
			{
				tok->st.y = tok->st.y >= 0 ? tok->st.y : i.y;
				tok->st.x = tok->st.x >= 0 && i.x > tok->st.x ? tok->st.x : i.x;
				tok->end.x = i.x + 1 < tok->end.x ? tok->end.x : i.x + 1;
				tok->end.y = i.y + 1 < tok->end.y ? tok->end.y : i.y + 1;
			}
			++i.x;
		}
		++i.y;
	}
	tok->psize.x = tok->end.x - tok->st.x;
	tok->psize.y = tok->end.y - tok->st.y;
}

t_token	*parse_piece(void)
{
	t_token *tok;
	char *line;
	t_xy i;

	tok = ft_memalloc(sizeof(t_token));
	get_next_line(STDIN_FILENO, &line);
	tok->dim = get_dimensions(line);
	ft_strdel(&line);
	tok->piece_c = ft_memalloc((tok->dim.x * tok->dim.y) + 1);
	i.y = 0;
	while (i.y < tok->dim.y)
	{
		get_next_line(STDIN_FILENO, &line);
		i.x = 0;
		while (*line)
			tok->piece_c[(i.y * tok->dim.x) + i.x++] = *line++;
		line -= i.x;
		ft_strdel(&line);
		i.y++;
	}
	trim_chpiece(tok);
	return (tok);
}

t_map	*init_board(char *line)
{
	t_map *board;

	board = ft_memalloc(sizeof(t_map));
	board->dim = get_dimensions(line);
	board->final_pt = board->dim.x * board->dim.y;
	board->st.x = board->dim.x;
	board->st.y = board->dim.y;
	board->end.x = -1;
	board->end.y = -1;
	board->map_c = ft_memalloc(board->final_pt + 1);
	board->heat_2 = ft_memalloc(sizeof(int) * board->final_pt);
	return(board);
}

void	reduce_size(t_xy cur, t_map *map, int fd)
{
	map->st.x = (cur.x < map->st.x) ? cur.x : map->st.x;
	dprintf(fd, "st-y:%d cur-y:%d\n", map->st.y, cur.y);
	map->st.y = (cur.y < map->st.y) ? cur.y : map->st.y;
	map->end.x = (cur.x + 1 > map->end.x) ? cur.x + 1 : map->end.x;
	map->end.y = (cur.y + 1 > map->end.y) ? cur.y + 1 : map->end.y;
}

void	parse_board(t_filler *filler)
{
	char *line;
	t_xy i;

	line = NULL;

	get_next_line(STDIN_FILENO, &line);
	if (filler->board == NULL)
		filler->board = init_board(line);
	ft_strdel(&line);
	get_next_line(STDIN_FILENO, &line);
	ft_strdel(&line);
	i.y = 0;
	while (i.y < filler->board->dim.y)
	{
		get_next_line(STDIN_FILENO, &line);
		i.x = 0;
		line += 4;
		while (*line)
		{
			if (ft_toupper(*line) == filler->me)
				reduce_size(i, filler->board, filler->fd);
			filler->board->map_c[(i.y * filler->board->dim.x) + i.x] = *line;
			filler->board->heat_2[(i.y * filler->board->dim.x) + i.x++] =
					ft_toupper(*line++) == filler->opp ? -1 : 0;
		}
		line -= (i.x + 4);
		ft_strdel(&line);
		i.y++;
	}
	filler->board->msize.x = filler->board->end.x - filler->board->st.x;
	filler->board->msize.y = filler->board->end.y - filler->board->st.y;
	dprintf(filler->fd, "st:(%d,%d) end:(%d,%d) size:(%d,%d)\n", \
		filler->board->st.x, filler->board->st.y, filler->board->end.x, \
		filler->board->end.y, filler->board->msize.x, filler->board->msize.y);
}

void	get_player(t_filler *filler)
{
	int i;
	char *line;

	line = NULL;
	get_next_line(STDIN_FILENO, &line);
	i = 0;
	while (line[i] != 'p')
		i++;
	filler->me = line[++i] == '1' ? 'O' : 'X';
	filler->opp = line[i] == '1' ? 'X' : 'O';
	ft_strdel(&line);
}

int		enemy_dist(t_map *m, int x, int y)
{
	int max;
	int dist;
	int st;

	dist = -1;
	max = (m->dim.y > m->dim.x) ? m->dim.y : m->dim.x;
	while (++dist < max)
	{
		st = x - dist - 1;
		while (++st <= x + dist)
		{
			if (st >= 0 && st < m->dim.x &&
					((y - dist >= 0 && m->heat_2[m->dim.x * (y - dist) + st] == -1) ||
					(y + dist < m->dim.y && m->heat_2[m->dim.x * (y + dist) + st] == -1)))
				return (dist == 0 ? -1 : MAXHEAT / (1.1 * dist));
		}
		st = y - dist;
		while (++st < y + dist)
		{
			if (st >= 0 && st < m->dim.y &&
					((x - dist >= 0 && m->heat_2[m->dim.x * st + x - dist] == -1) ||
					(x + dist < m->dim.x && m->heat_2[m->dim.x * st + x + dist] == -1)))
				return (dist == 0 ? -1 : MAXHEAT / (1.1 * dist));
		}
	}
	return (-1);
}

int		check_piece(t_filler *filler, t_xy pt)
{
	t_xy st;
	int val;
	int touch;

	val = 0;
	touch = 0;
	st.y = 0;
	while (st.y < filler->tok->psize.y)
	{
		st.x = 0;
		while (st.x < filler->tok->psize.x)
		{
			if (filler->board->heat_2[(pt.y + st.y) * filler->board->dim.x + pt.x + st.x] == 0)
				filler->board->heat_2[(pt.y + st.y) * filler->board->dim.x + pt.x + st.x] = enemy_dist(filler->board, pt.x + st.x, pt.y + st.y);
			if (filler->tok->piece_c[(st.y + filler->tok->st.y) * filler->tok->dim.x + st.x + filler->tok->st.x] == '*')
			{
				if (pt.x + st.x >= filler->board->dim.x ||
						pt.y + st.y >= filler->board->dim.y)
					return (-1);
				if (filler->board->heat_2[(pt.y + st.y) * filler->board->dim.x + pt.x + st.x] < 0)
					return (-1);
				if (filler->board->map_c[(pt.y + st.y) * filler->board->dim.x + pt.x + st.x] == filler->me)
				{
					if (++touch > 1)
						return (-1);
				}
				val += filler->board->heat_2[(pt.y + st.y) * filler->board->dim.x + pt.x + st.x];
			}
			st.x++;
		}
		st.y++;
	}
	if (touch != 1)
		return (-1);
	return (val);
}

void	placement(t_filler *filler)
{
	t_xy i;
	int val;

	filler->board->piece_val = -1;
	filler->board->place.x = 0;
	filler->board->place.y = 0;
	i.y = (filler->board->st.y - filler->tok->psize.y - 1 < 0) ? 0 : filler->board->st.y - filler->tok->psize.y - 1;
	while (i.y < filler->board->end.y)
	{
		i.x = (filler->board->st.x - filler->tok->psize.x - 1 < 0) ? 0 : filler->board->st.x - filler->tok->psize.x - 1;
		while (i.x < filler->board->end.x)
		{
			if ((val = check_piece(filler, i)) > filler->board->piece_val)
			{
				filler->board->piece_val = val;
				filler->board->place.x = i.x - filler->tok->st.x;
				filler->board->place.y = i.y - filler->tok->st.y;
			}
			i.x++;
		}
		i.y++;
	}
	if (filler->board->piece_val > 0)
	{
		ft_putnbr(filler->board->place.y);
		ft_putchar(' ');
		ft_putnbr(filler->board->place.x);
		ft_putchar('\n');
	}
	else
	{
		ft_putnbr(0);
		ft_putchar(' ');
		ft_putnbr(0);
		ft_putchar('\n');
		filler->done = 42;
	}
}

void	clean_everything(t_filler *f)
{
	ft_bzero(f->board->heat_2, f->board->final_pt * sizeof(int));
	ft_bzero(f->tok->piece_c, f->tok->dim.x * f->tok->dim.y);
	ft_memdel((void**)&f->tok->piece_c);
	ft_bzero(f->tok, sizeof(t_token));
	ft_memdel((void**)&f->tok);
}

int		main(void)
{
	t_filler filler;

	ft_bzero(&filler, sizeof(t_filler));
	filler.fd = open("./dumb.log", O_CREAT | O_WRONLY | O_TRUNC);
	get_player(&filler);
	while (!filler.done)
	{
		parse_board(&filler);
		filler.tok = parse_piece();
		placement(&filler);
		clean_everything(&filler);
	}
}
