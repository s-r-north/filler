/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filler.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2069/06/09 00:00:00 by snorth            #+#    #+#             */
/*   Updated: 2069/06/09 00:00:00 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILLER_H

# define FILLER_H

# include "./libft/libft.h"
# include <limits.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

# define MAXHEAT 5000

typedef struct	s_token
{
	char		*piece;
	t_xy		dim;
	t_xy		st;
	t_xy		end;
	t_xy		psize;
}				t_token;

typedef struct	s_map
{
	int			*heat;
	char		*board;
	int			final_pt;
	t_xy		st;
	t_xy		end;
	t_xy		msize;
	t_xy		dim;
	t_xy		place;
	int			piece_val;
}				t_map;

typedef struct	s_filler
{
	int			done;
	char		me;
	char		opp;
	t_map		*map;
	t_token		*tok;
}				t_filler;

void			get_player(t_filler *f);

t_xy			get_dimensions(char *line);

void			parse_map(t_filler *f);

void			parse_piece(t_filler *f);

void			placement(t_filler *f);

void			clean_everything(t_filler *f);

#endif
