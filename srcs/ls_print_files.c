/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_print_files.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/05 12:09:02 by astripeb          #+#    #+#             */
/*   Updated: 2020/07/05 13:41:51 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		print_files(t_opts *funct, char *path, t_darr *files)
{
	int	len;

	len = ft_strlen(path) - 1;
	if (funct->opts & LS_PRPATH)
		ft_printf("%.*s:\n", len, path);
	ft_da_sort(files, funct->less);
	funct->print(funct->opts, files);
	if (funct->opts & LS_FISRSTPRINT)
	{
		while (len > 1 && path[len] == '/' && path[len - 1] == '/')
			path[len--] = '\0';
		funct->opts = funct->opts & (~LS_FISRSTPRINT);
	}
}
