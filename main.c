#include "filler.h"

int		(*g_heat[THREAD_CT])(t_thread *m, t_xy pt) =
{
	enemy_dist
};

void	init_heatmaps(t_filler *data)
{
	int th;
	int	i;

	th = -1;
	while (++th < THREAD_CT)
	{
		data->heatmap[th].rule = g_heat[th];
		data->heatmap[th].th_num = th;
		data->heatmap[th].dim.x = data->board.dim.x;
		data->heatmap[th].dim.y = data->board.dim.y;
		data->heatmap[th].map = malloc(sizeof(int*) * data->board.dim.y);
		i = -1;
		while (++i < data->board.dim.y)
			data->heatmap[th].map[i] = malloc(sizeof(int) * data->board.dim.x);
	}
}

void	init_map(t_filler *data)
{
	char	*line;
	t_map	tmp;
	int		i;

	line = NULL;
	get_next_line(STDIN_FILENO, &line);
	tmp.dim = get_dimensions(line);
	ft_strdel(&line);
	tmp.map = malloc(sizeof(int*) * tmp.dim.y);
	tmp.heatmap = malloc(sizeof(int*) * tmp.dim.y);
	i = -1;
	while (++i < tmp.dim.y)
	{
		tmp.map[i] = malloc(sizeof(int) * tmp.dim.x);
		tmp.heatmap[i] = malloc(sizeof(int) * tmp.dim.x);
	}
	tmp.piece_val = -1;
	data->board = tmp;
}

void	set_player(t_filler *data)
{
	char *line;
	char *start;

	line = NULL;
	dprintf(data->fd, "check0\n");
	get_next_line(STDIN_FILENO, &line);
	dprintf(data->fd, "check\n");
	start = line;
	while (*line && *line != 'p')
		line++;
	line++;
	data->me = *line == '1' ? 'O' : 'X';
	data->opp = *line == '1' ? 'X' : 'O';
	ft_strdel(&start);
}

int		check_piece(t_filler *data, t_xy *pt)
{
	t_xy	st;
	int		val;
	int		touch;

	val = 0;
	touch = 0;
	st.y = -1;
	while (++st.y < data->tok.psize.y)
	{
		st.x = -1;
		while (++st.x < data->tok.psize.x)
		{
			if (data->tok.piece[st.y + data->tok.st.y][st.x + data->tok.st.x] == 1)
			{
				if (pt->x + st.x >= data->board.dim.x ||
						pt->y + st.y >= data->board.dim.y ||
						data->board.heatmap[pt->y + st.y][pt->x + st.x] < 0)
					return (-1);
				if (data->board.map[pt->y + st.y][pt->x + st.x] == 0)
					touch++;
				val += data->board.heatmap[pt->y + st.y][pt->x + st.x];
			}
		}
	}
	if (touch != 1)
		return (-1);
	return (val);
}

void	place_piece(t_filler *data)
{
	t_xy	pt;
	int		val;

	pt.y = -1;
	while (++pt.y < data->board.dim.y)
	{
		pt.x = -1;
		while (++pt.x < data->board.dim.x)
		{
			if ((val = check_piece(data, &pt)) > data->board.piece_val)
			{
				data->board.piece_val = val;
				data->board.place.x = pt.x;
				data->board.place.y = pt.y;
			}
		}
	}
	if (data->board.piece_val > -1)
		printf("%d %d\n", data->board.place.y, data->board.place.x);
	dprintf(data->fd, "%d %d with val = %d\n", data->board.place.y, data->board.place.x, data->board.piece_val);
}

void	print_placement(t_filler *s)
{
	t_xy beg;
	t_xy end;
	t_xy pc;

	pc.y = s->tok.st.y;
	beg.x = s->board.place.x;
	beg.y = s->board.place.y;
	end.x = s->board.place.x + s->tok.psize.x;
	end.y = s->board.place.y + s->tok.psize.y;
	for (int i = 0; i < s->board.dim.y; i++)
	{
		// pc.x = s->tok.st.x;
		for (int j = 0; j < s->board.dim.x; j++)
		{
			// if (j >= beg.x && j < end.x && i >= beg.y && i < end.y)
			// {
			// 	if (s->tok.piece[pc.y][pc.x] == 1)
			// 		printf("*pc* ");
			// 	else
			// 		printf("%04d ", s->board.map[i][j]);
			// 	pc.x++;
			// }
			// else
				dprintf(s->fd, "%04d ", s->board.heatmap[i][j]);
		}
		dprintf(s->fd, "\n");
		// if (pc.x != s->tok.st.x)
		// {
		// 	printf("\t");
		// 	for (int k = s->tok.st.x; k < s->tok.end.x; k++)
		// 		printf("%d ", s->tok.piece[pc.y][k]);
		// 	pc.y++;
		// }
		// printf("\tpiece h: %d, size: (%d, %d)\n", pc.y - 1, s->tok.psize.x, s->tok.psize.y);
	}
}

int		main(void)
{
	char *line;
	t_filler data;

	data.fd = open("./error_debug", O_CREAT | O_WRONLY | O_TRUNC);
	// dprintf(data.fd, "my dude\n");
	// ft_bzero(&data, sizeof(data));
	dprintf(data.fd, "boiiii\n");
	set_player(&data);
	dprintf(data.fd, "me: %c, opp: %c\n", data.me, data.opp);
	init_map(&data);
	dprintf(data.fd, "map init: %d by %d\n", data.board.dim.x, data.board.dim.y);
	init_heatmaps(&data);
	dprintf(data.fd, "heatmap init\n");
	// data = init_filler();
	line = NULL;
	while (get_next_line(STDIN_FILENO, &line) > 0)
	{
		if (!ft_strncmp(line, "Plateau", 7))
			continue ;
		ft_strdel(&line);
		read_map(&data);
		dprintf(data.fd, "read map done\n");
		init_token(&data);
		dprintf(data.fd, "read token done\n");
		multithread(&data);
		dprintf(data.fd, "made heatmaps\n");
		place_piece(&data);
		dprintf(data.fd, "piece placed\n");
		print_placement(&data);
	}
}
