#include "filler.h"

void	final_heatmap(int **in, t_map *final, int thread)
{
	t_xy i;

	i.y = -1;
	while (++i.y < final->dim.y)
	{
		i.x = -1;
		while (++i.x < final->dim.x)
		{
			if (thread == 0)
				final->heatmap[i.y][i.x] = final->heatmap[i.y][i.x] == -1 ? -1 : in[i.y][i.x];
			else
				final->heatmap[i.y][i.x] = final->heatmap[i.y][i.x] == -1 ? -1 : (final->heatmap[i.y][i.x] + in[i.y][i.x]) / 2;
		}
	}
}

void	set_heatmap(t_thread *board)
{
	t_xy pt;

	pt.y = -1;
	while (++pt.y < board->dim.y)
	{
		pt.x = -1;
		while (++pt.x < board->dim.x)
		{
			if (board->map[pt.y][pt.x] >= 0)
				board->map[pt.y][pt.x] = MAXHEAT / (1.1 * board->rule(board, pt));
		}
	}
}

void	*set_thread(void *arg)
{
	t_thread	*board;
	int			i;

	board = (t_thread*)arg;
	i = -1;
	while (++i < THREAD_CT)
	{
		if (pthread_equal(pthread_self(), board->tid))
			set_heatmap(board);
	}
	return (NULL);
}

void	multithread(t_filler *q)
{
	int		th;

	th = -1;
	while (++th < THREAD_CT)
		pthread_create(&q->heatmap[th].tid, NULL, set_thread, &q->heatmap[th]);
	th = -1;
	while (++th < THREAD_CT)
	{
		pthread_join(q->heatmap[th].tid, NULL);
		final_heatmap(q->heatmap[th].map, &q->board, th);
	}
}
