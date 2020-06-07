/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 11:09:56 by pcredibl          #+#    #+#             */
/*   Updated: 2020/06/07 23:33:10 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	usage(void)
{
	ft_printf("Usage: ft_ls [OPTION]... [FILE]...\n");
	ft_printf("   -a\tdo not ignore entries starting with .\n");
	ft_printf("   -l\tuse a long listing format\n");
	ft_printf("   -R\tlist subdirectories recursively\n");
	ft_printf("   -r\treverse order while sorting\n");
	ft_printf("   -t\tsort by modification time, newest first\n");
	exit(EXIT_SUCCESS);
}

static int	check_flags(char *flags, long int options)
{
	while (*flags && options)
	{
		if (*flags > 96 && *flags < 123)
		{
			if ((options >> (*flags - 71)) & 1)
				options ^= (1L << (*flags - 71));
		}
		else if (*flags > 64 && *flags < 91)
		{
			if ((options >> (*flags - 65)) & 1)
				options ^= (1L << (*flags - 65));
		}
		flags++;
	}
	return (options ? 0 : 1);
}

static int	check_option(char *str, long int *options)
{
	int i;

	if (str[0] == '-')
	{
		i = 1;
		if (str[i] == 'h')
			return (1);
		while (str[i])
		{
			if (str[i] > 96 && str[i] < 123)
				*options |= (1L << (str[i] - 71));
			else if (str[i] > 64 && str[i] < 91)
			    *options |= (1L << (str[i] - 65));
			else
				return (-1);
			++i;
		}
	}
	return (0);
}

size_t		options(int ac, char **av)
{
	int			i;
	int			check;
	size_t		options;

	options = 0;
	i = 1;
	while (i < ac)
	{
		check = check_option(av[i], &options);
		if (check == 1)
			usage();
		if (check == -1)
		{
			ft_printf("%s: invalid option -- '%s'\n", av[0], av[i]);
			ft_printf("Try '%s -h' for more information.\n", av[0]);
			exit(EXIT_FAILURE);
		}
		++i;
	}
	if (!check_flags("lartR", options))
		usage();
	return (options);
}

void		print_opts(size_t opts)
{
	if (opts & LS_ALL)
		ft_printf("all ");
	if (opts & LS_REC)
		ft_printf("recursive ");
	if (opts & LS_LONG)
		ft_printf("long_format ");
	if (opts & LS_REV_S)
		ft_printf("reverse_sort ");
	if (opts & LS_TIME)
		ft_printf("sort by time");
	ft_printf("\n");
}
