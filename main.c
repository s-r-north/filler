/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2069/06/09 00:00:00 by snorth            #+#    #+#             */
/*   Updated: 2069/06/09 00:00:00 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./filler.h"

void	get_player(t_filler *f)
{
	int		i;
	char	*line;

	line = NULL;
	get_next_line(STDIN_FILENO, &line);
	i = 0;
	while (line[i] != 'p')
		i++;
	f->me = line[++i] == '1' ? 'O' : 'X';
	f->opp = line[i] == '1' ? 'X' : 'O';
	ft_strdel(&line);
}

void	clean_everything(t_filler *f)
{
	f->map->place.x = 0;
	f->map->place.y = 0;
	f->map->piece_val = -1;
	ft_bzero(f->tok->piece, f->tok->dim.x * f->tok->dim.y);
	ft_memdel((void**)&f->tok->piece);
	ft_bzero(f->tok, sizeof(t_token));
	ft_memdel((void**)&f->tok);
}

int		main(void)
{
	t_filler filler;

	ft_bzero(&filler, sizeof(t_filler));
	get_player(&filler);
	while (!filler.done)
	{
		parse_map(&filler);
		parse_piece(&filler);
		placement(&filler);
		clean_everything(&filler);
	}
}
