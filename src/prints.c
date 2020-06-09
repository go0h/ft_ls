/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/09 08:32:58 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/09 20:36:24 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_content(char *path, t_darr *list)
{
	size_t	i;
	t_file	*f_ptr;

	i = 0;
	ft_printf("%s:\n", path);
	while (i < list->size)
	{
		f_ptr = (t_file*)ft_da_get_pointer(list, i);
		ft_printf("%s ", f_ptr->d_ptr.d_name);
		++i;
	}
	ft_printf("\n\n");
}
