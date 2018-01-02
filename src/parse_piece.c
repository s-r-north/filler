/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_piece.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snorth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2069/06/09 00:00:00 by snorth            #+#    #+#             */
/*   Updated: 2069/06/09 00:00:00 by snorth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../filler.h"

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

static void	trim_chpiece(t_token *tok)
{
	t_xy i;

	tok->st.x = tok->dim.x;
	tok->st.y = tok->dim.y;
	tok->end.x = 0;
	tok->end.y = 0;
	i.y = 0;
	while (i.y < tok->dim.y)
	{
		i.x = 0;
		while (i.x < tok->dim.x)
		{
			if (tok->piece[(i.y * tok->dim.x) + i.x] == '*')
			{
				tok->st.y = i.y < tok->st.y ? i.y : tok->st.y;
				tok->st.x = i.x < tok->st.x ? i.x : tok->st.x;
				tok->end.x = i.x + 1 < tok->end.x ? tok->end.x : i.x + 1;
				tok->end.y = i.y + 1 < tok->end.y ? tok->end.y : i.y + 1;
			}
			i.x++;
		}
		i.y++;
	}
	tok->psize.x = tok->end.x - tok->st.x;
	tok->psize.y = tok->end.y - tok->st.y;
}

void		parse_piece(t_filler *f)
{
	t_token	*tok;
	char	*line;
	t_xy	i;

	tok = ft_memalloc(sizeof(t_token));
	get_next_line(STDIN_FILENO, &line);
	tok->dim = get_dimensions(line);
	ft_strdel(&line);
	tok->piece = ft_memalloc((tok->dim.x * tok->dim.y) + 1);
	i.y = 0;
	while (i.y < tok->dim.y)
	{
		get_next_line(STDIN_FILENO, &line);
		i.x = 0;
		while (*line)
			tok->piece[(i.y * tok->dim.x) + i.x++] = *line++;
		line -= i.x;
		ft_strdel(&line);
		i.y++;
	}
	trim_chpiece(tok);
	f->tok = tok;
}
