/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_short_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/10 10:07:47 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/17 18:23:41 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <sys/ioctl.h>

static void		ft_one_line_print(t_darr *files)
{
	size_t	i;
	t_file	*f_ptr;

	i = files->size;
	f_ptr = (t_file*)files->array;
	while (i-- != 0)
	{
		ft_printf("%s\n", f_ptr->filename);
		f_ptr++;
	}
}

static	int		get_term_width(void)
{
	struct winsize	win;

	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &win))
		return (128);
	return (win.ws_col);
}

static size_t	get_max_len(t_darr *files, int start, int step)
{
	size_t			i;
	size_t			max_len;
	t_file			*f_ptr;

	i = start;
	max_len = 0;
	while (i < files->size)
	{
		f_ptr = (t_file*)ft_da_get_pointer(files, i);
		max_len = ft_maxs(max_len, ft_strlen(f_ptr->filename));
		i += step;
	}
	return (max_len);
}

void			calc_rows_cols(size_t *params, t_darr *files)
{
	static int	width = 0;
	size_t		full_len;

	if (!files->size)
		return ;
	if (!width)
		width = get_term_width();
	params[0] = get_max_len(files, 0, 1) + 2;
	full_len = files->size * params[0];
	params[1] = width / params[0] + (width / params[0] == 0);
	params[2] = full_len / (params[0] * params[1]) + (params[1] != 0);
}

/*
**	params[0] = max length name
**	params[1] = columns
**	params[2] = rows
*/

void			ft_short_print(size_t opts, t_darr *files)
{
	size_t		i;
	size_t		j;
	size_t		params[3];
	t_file		*f_ptr;

	if (opts & LS_ONE_LINE)
	{
		ft_one_line_print(files);
		return ;
	}
	i = 0;
	calc_rows_cols(params, files);
	while (files->size && i < params[2])
	{
		j = i;
		while (j < files->size)
		{
			f_ptr = ft_da_get_pointer(files, j);
			ft_printf("%-*s", (j + params[2]) < files->size ? params[0] : 0,\
			f_ptr->filename);
			j += params[2];
		}
		++i;
		ft_printf("\n");
	}
}
