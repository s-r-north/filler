#include "filler.h"

static void	piece_trim(t_token *tok)
{
	int x;
	int y;

	tok->st.x = -1;
	tok->st.y = -1;
	y = -1;
	while (++y < tok->dim.y)
	{
		x = -1;
		while (++x < tok->dim.x)
		{
			if (tok->piece[y][x] == 1)
			{
				tok->st.y = tok->st.y >= 0 ? tok->st.y : y;
				tok->st.x = tok->st.x >= 0 && x >= tok->st.x ? tok->st.x : x;
				tok->end.x = x + 1 <= tok->end.x ? tok->end.x : x + 1;
				tok->end.y = y + 1 <= tok->end.y ? tok->end.y : y + 1;
			}
		}
	}
	tok->psize.x = tok->end.x - tok->st.x;
	tok->psize.y = tok->end.y - tok->st.y;
}

t_xy		get_dimensions(char *line)
{
	t_xy xy;

	while (*line <= '0' || *line >= '9')
		line++;
	xy.y = ft_atoi(line);
	while (*line >= '0' && *line <= '9')
		line++;
	xy.x = ft_atoi(line);
	return (xy);
}

void		init_token(t_filler *data)
{
	char *line;
	int i;
	int j;

	line = NULL;
	get_next_line(STDIN_FILENO, &line);
	data->tok.dim = get_dimensions(line);
	ft_strdel(&line);
	data->tok.piece = malloc(sizeof(int*) * data->tok.dim.y);
	i = -1;
	while (++i < data->tok.dim.y)
	{
		get_next_line(STDIN_FILENO, &line);
		data->tok.piece[i] = malloc(sizeof(int) * data->tok.dim.x);
		j = -1;
		while (++j < data->tok.dim.x)
			data->tok.piece[i][j] = *line++ == '*' ? 1 : 0;
		line -= data->tok.dim.x;
		ft_strdel(&line);
	}
	piece_trim(&data->tok);
}

static int	map_value(char c, char me, char opp)
{
	if (c == me)
		return (0);
	if (c == opp)
		return (-1);
	if (c == '.')
		return (1);
	return (INT_MAX);
}

void		read_map(t_filler *data)
{
	t_xy i;
	int val;
	int th;
	char *buf;

	buf = NULL;
	i.y = -1;
	while (++i.y < data->board.dim.y)
	{
		get_next_line(STDIN_FILENO, &buf);
		buf += 4;
		i.x = -1;
		while (++i.x < data->board.dim.x)
		{
			val = map_value(ft_toupper(*buf), data->me, data->opp);
			data->board.map[i.y][i.x] = val;
			data->board.heatmap[i.y][i.x] = ((*buf++ == data->opp) ? -1 : 1);
			th = -1;
			while (++th < THREAD_CT)
				data->heatmap[th].map[i.y][i.x] = val;
		}
		buf -= (4 + data->board.dim.x);
		ft_strdel(&buf);
	}
}
