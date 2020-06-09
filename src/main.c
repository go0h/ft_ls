/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:22:13 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/09 20:55:30 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <limits.h>

int		main(int argc, char **argv)
{
	int 	i;
	t_opts	funct;
	char	*pathname;

	funct = get_functors(options(argc, argv));
	if (argc == 1)
		return (EXIT_SUCCESS);
	if (!(pathname = ft_strnew(PATH_MAX - 1)))
		ft_exit(E_MALLOC, NULL);
	i = 1;
	while (i < argc)
	{
		if (*argv[i] == '-')
		{
			++i;
			continue;
		}
		ft_strcpy(pathname, argv[i]);
		ft_read_root(&funct, pathname);
		++i;
	}
	ft_strdel(&pathname);
	return (EXIT_SUCCESS);
}
