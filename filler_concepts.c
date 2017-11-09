#include "./libft/libft.h"
#include <limits.h>

# define MAXHEAT 5000
# define THREAD_CT 1

typedef struct	s_xy
{
	int			x;
	int			y;
}				t_xy;

typedef struct	s_thread
{
	int			**map;
	int			th_num;
	pthread_t	tid;
	int			(*criterium)(t_filler *data, int x, int y);
}				t_thread;

typedef struct	s_data
{
	char		me;
	char		opp;
	int			map_w;
	int			map_h;
	int			map_bytes;
	int			**map;
	int			**map0;
	int			piece_w;
	int			piece_h;
	t_xy		pstart;
	t_xy		pend;
	t_xy		psize;
	int			**piece;
	t_xy		place;
	int			piece_val;
}				t_filler;

void	piece_trim(t_filler *data, int **piece)
{
	int x;
	int y;

	data->pstart.x = -1;
	data->pstart.y = -1;
	y = -1;
	while (++y < data->piece_h)
	{
		x = -1;
		while (++x < data->piece_w)
		{
			if (piece[y][x] == 1)
			{
				data->pstart.y = data->pstart.y >= 0 ? data->pstart.y : y;
				data->pstart.x = data->pstart.x >= 0 &&
						x >= data->pstart.x ? data->pstart.x : x;
				data->pend.x = x + 1 <= data->pend.x ? data->pend.x : x + 1;
				data->pend.y = y + 1 <= data->pend.y ? data->pend.y : y + 1;
			}
		}
	}
	data->psize.x = data->pend.x - data->pstart.x;
	data->psize.y = data->pend.y - data->pstart.y;
}

void	get_piece(t_filler *data)
{
	char *line;
	int i;
	int j;

	line = NULL;
	get_next_line(STDIN_FILENO, &line);
	while (*line <= '0' || *line >= '9')
		line++;
	data->piece_h = ft_atoi(line);
	while (*line >= '0' && *line <= '9')
		line++;
	data->piece_w = ft_atoi(line);
	data->piece = malloc(sizeof(int*) * data->piece_h);
	i = -1;
	while (++i < data->piece_h)
	{
		get_next_line(STDIN_FILENO, &line);
		data->piece[i] = malloc(sizeof(int) * data->piece_w);
		j = -1;
		while (++j < data->piece_w)
			data->piece[i][j] = *line++ == '*' ? 1 : 0;
	}
	piece_trim(data, data->piece);
}

int		map_value(char c, char me, char opp)
{
	if (c == me)
		return (0);
	if (c == opp)
		return (-1);
	if (c == '.')
		return (1);
	return (INT_MAX);
}

int		heat_val(t_filler *data, int y, int x)
{
	int max;
	int dist;
	int test;
	int done;

	dist = 0;
	done = 0;
	max = (data->map_h > data->map_w) ? data->map_h : data->map_w;
	while (!done && ++dist < max)
	{
		test = x - dist;
		while (test <= x + dist)
		{
			if (test >= 0 && test < data->map_w && ((y - dist >= 0 && data->map[y - dist][test] == -1) || (y + dist < data->map_h && data->map[y + dist][test] == -1)))
			{
				done = 1;
				break ;
			}
			test++;
		}
		test = y - dist;
		while (test <= y + dist)
		{
			if (test >= 0 && test < data->map_h && ((x - dist >= 0 && data->map[test][x - dist] == -1) || (x + dist < data->map_w && data->map[test][x + dist] == -1)))
			{
				done = 1;
				break ;
			}
			test++;
		}
	}
	return (dist);
}

void	set_heatmap(t_filler *data)
{
	int i;
	int j;

	i = -1;
	while (++i < data->map_h)
	{
		j = -1;
		while (++j < data->map_w)
		{
			if (data->map[i][j] >= 0)
				data->map[i][j] = MAXHEAT / (1.1 * heat_val(data, i, j));
		}
	}
}

void	read_map(t_filler *data)
{
	int i;
	int j;
	char *buf;
	int **tmp;

	buf = NULL;
	get_next_line(STDIN_FILENO, &buf);
	tmp = malloc(sizeof(int*) * data->map_h);
	i = -1;
	while (++i < data->map_h)
	{
		get_next_line(STDIN_FILENO, &buf);
		buf += 4;
		tmp[i] = malloc(sizeof(int) * data->map_w);
		j = -1;
		while (++j < data->map_w)
			tmp[i][j] = map_value(ft_toupper(*buf++), data->me, data->opp);
	}
	data->map0 = tmp;
	data->map = malloc(sizeof(int*) * data->map_h);
	i = -1;
	while (++i < data->map_h)
	{
		data->map[i] = malloc(sizeof(int) * data->map_w);
		j = -1;
		while (++j < data->map_w)
			data->map[i][j] = data->map0[i][j];
	}
	// printf("map w: %d, map_h: %d\n", data->map_w, data->map_h);
}

void	init_maps(t_filler *data, char *line)
{
	char	*start;
	int		thread;
	int		h;

	if (!line)
		get_next_line(STDIN_FILENO, &line);
	start = line;
	while (*line <= '0' || *line >= '9')
		line++;
	data->map_h = ft_atoi(line);
	while (*line >= '0' && *line <= '9')
		line++;
	data->map_w = ft_atoi(line);
	thread = -1;
	while (++thread <= THREAD_CT)
	{
		data->map[thread] = malloc(sizeof(int*) * data->map_h);
		h = -1;
		while (++h < data->map_h)
			data->map[thread][h] = malloc(sizeof(int) * data->map_w);
	}
}

void	set_player(t_filler *data, char *line)
{
	while (*line && *line != 'p')
		line++;
	line++;
	data->me = *line == '1' ? 'O' : 'X';
	data->opp = *line == '1' ? 'X' : 'O';
}

int		check_piece(t_filler *data, t_xy *place)
{
	t_xy	st;
	int		val;
	int		touch;

	val = 0;
	touch = 0;
	st.y = -1;
	while (++st.y < data->psize.y)
	{
		st.x = -1;
		while (++st.x < data->psize.x)
		{
			if (data->piece[st.y + data->pstart.y][st.x + data->pstart.x] == 1)
			{
				if (place->x + st.x >= data->map_w ||
						place->y + st.y >= data->map_h ||
						data->map0[place->y + st.y][place->x + st.x] < 0)
					return (-1);
				if (data->map0[place->y + st.y][place->x + st.x] == 0)
					touch++;
				val += data->map[place->y + st.y][place->x + st.x];
			}
		}
	}
	if (touch != 1)
		return (-1);
	return (val);
}

// void	print_placement(t_filler *s)
// {
// 	t_xy beg;
// 	t_xy end;
// 	t_xy pc;
//
// 	pc.y = s->pstart.y;
// 	beg.x = s->place.x;
// 	beg.y = s->place.y;
// 	end.x = s->place.x + s->psize.x;
// 	end.y = s->place.y + s->psize.y;
// 	for (int i = 0; i < s->map_h; i++)
// 	{
// 		pc.x = s->pstart.x;
// 		for (int j = 0; j < s->map_w; j++)
// 		{
// 			if (j >= beg.x && j < end.x && i >= beg.y && i < end.y)
// 			{
// 				if (s->piece[pc.y][pc.x] == 1)
// 					printf("*pc* ");
// 				else
// 					printf("%04d ", s->map[i][j]);
// 				pc.x++;
// 			}
// 			else
// 				printf("%04d ", s->map[i][j]);
// 		}
// 		if (pc.x != s->pstart.x)
// 		{
// 			printf("\t");
// 			for (int k = s->pstart.x; k < s->pend.x; k++)
// 				printf("%d ", s->piece[pc.y][k]);
// 			pc.y++;
// 		}
// 		printf("\tpiece h: %d, size: (%d, %d)\n", pc.y - 1, s->psize.x, s->psize.y);
// 	}
// }

void	place_piece(t_filler *data)
{
	t_xy	pt;
	int		val;

	pt.y = -1;
	while (++pt.y < data->map_h)
	{
		pt.x = -1;
		while (++pt.x < data->map_w)
		{
			if ((val = check_piece(data, &pt)) > data->piece_val)
			{
				data->piece_val = val;
				data->place.x = pt.x;
				data->place.y = pt.y;
			}
		}
	}
	printf("%d %d\n", data->place.y - data->pstart.y, data->place.x - data->pstart.x);
	// print_placement(data);
}

// void	init_filler(t_filler *data);

int		main()
{
	t_filler data;
	char *line;

	ft_bzero(&data, sizeof(data));
	// data = init_filler();
	line = NULL;
	get_next_line(STDIN_FILENO, &line);
	if (!ft_strncmp(line, "$$$", 3))
	{
		set_player(&data, line);
		free(line);
		line = NULL;
	}
	init_map(&data, line);
	read_map(&data);
	get_piece(&data);
	set_heatmap(&data);

	place_piece(&data);
}
