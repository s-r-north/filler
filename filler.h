#include "./libft/libft.h"
#include <limits.h>

# define MAXHEAT 5000
# define THREAD_CT 1

typedef struct	s_xy
{
	int			x;
	int			y;
}				t_xy;

typedef struct	s_token
{
	int			**piece;
	t_xy		dim;
	t_xy		st;
	t_xy		end;
	t_xy		psize;
}				t_token;

typedef struct	s_map
{
	int			**map;
	int			**heatmap;
	t_xy		dim;
	t_xy		place;
	int			piece_val;
}				t_map;

typedef struct	s_thread
{
	int			**map;
	t_xy		dim;
	int			th_num;
	int			bias;
	pthread_t	tid;
	int			(*rule)(struct s_thread *m, t_xy pt);
}				t_thread;

typedef struct	s_filler
{
	char		me;
	char		opp;
	t_map		board;
	t_token		tok;
	t_thread	heatmap[THREAD_CT];
}				t_filler;

void			set_player(t_filler *data);
void			init_map(t_filler *data);
void			init_heatmaps(t_filler *data);

t_xy			get_dimensions(char *line);

void			init_token(t_filler *data);
void			read_map(t_filler *data);

void			multithread(t_filler *q);
void			*set_thread(void *arg);
void			set_heatmap(t_thread *board);

int				enemy_dist(t_thread *m, t_xy pt);
