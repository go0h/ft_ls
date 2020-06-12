/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   short_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/10 10:07:47 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/12 14:21:48 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_short_print(size_t opts, t_darr *files)
{
	size_t	i;
	t_file	*f_ptr;

	i = 0;
	while (i < files->size)
	{
		f_ptr = (t_file*)ft_da_get_pointer(files, i);
		ft_printf("%s ", f_ptr->filename);
		++i;
	}
	ft_printf("\n%s", opts & LS_REC ? "\n" : "");
}
