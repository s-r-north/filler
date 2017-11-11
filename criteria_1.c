#include "filler.h"

int		wall_dist(t_thread *m, t_xy pt)
{
	
}

int		enemy_dist(t_thread *m, t_xy pt)
{
	int max;
	int dist;
	int st;

	dist = 0;
	max = (m->dim.y > m->dim.x) ? m->dim.y : m->dim.x;
	while (++dist < max)
	{
		st = pt.x - dist - 1;
		while (++st <= pt.x + dist)
		// {
			if (st >= 0 && st < m->dim.x &&
					((pt.y - dist >= 0 && m->map[pt.y - dist][st] == -1) ||
					(pt.y + dist < m->dim.y && m->map[pt.y + dist][st] == -1)))
				return (dist);
		// }
		st = pt.y - dist;
		while (++st < pt.y + dist)
		// {
			if (st >= 0 && st < m->dim.y &&
					((pt.x - dist >= 0 && m->map[st][pt.x - dist] == -1) ||
					(pt.x + dist < m->dim.x && m->map[st][pt.x + dist] == -1)))
				return (dist);
		// }
	}
	return (-1);
}
