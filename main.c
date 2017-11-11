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
	data->board = tmp;
}

void	set_player(t_filler *data)
{
	char *line;
	char *start;

	line = NULL;
	get_next_line(STDIN_FILENO, &line);
	start = line;
	while (*line && *line != 'p')
		line++;
	line++;
	data->me = *line == '1' ? 'O' : 'X';
	data->opp = *line == '1' ? 'X' : 'O';
	ft_strdel(&start);
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
				printf("%04d ", s->board.heatmap[i][j]);
		}
		printf("\n");
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

	ft_bzero(&data, sizeof(data));
	set_player(&data);
	init_map(&data);
	init_heatmaps(&data);
	// data = init_filler();
	line = NULL;
	while (get_next_line(STDIN_FILENO, &line) > 0)
	{
		if (!ft_strncmp(line, "Plateau", 7))
			continue ;
		ft_strdel(&line);
		read_map(&data);
		init_token(&data);
		multithread(&data);
		// place_piece(&data);
		print_placement(&data);
	}
	// if (!ft_strncmp(line, "$$$", 3))
	// {
	// 	set_player(&data, line);
	// 	free(line);
	// 	line = NULL;
	// }
	// init_map(&data, line);
	// read_map(&data);
	// get_piece(&data);
	// set_heatmap(&data);

	// place_piece(&data);
}
