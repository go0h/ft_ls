/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 11:09:56 by pcredibl          #+#    #+#             */
/*   Updated: 2020/06/14 21:14:46 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	usage(char *prog, char *opt, int err)
{
	if (err == USAGE)
	{
		ft_printf("Usage: ft_ls [OPTION]... [FILE]...\n");
		ft_printf("   -a\tdo not ignore entries starting with .\n");
		ft_printf("   -l\tuse a long listing format\n");
		ft_printf("   -R\tlist subdirectories recursively\n");
		ft_printf("   -r\treverse order while sorting\n");
		ft_printf("   -t\tsort by modification time, newest first\n");
		exit(EXIT_SUCCESS);
	}
	else if (err == INVALID_OPTION)
	{
		ft_printf("%s: invalid option -- '%s'\n", prog, opt);
		ft_printf("Try '%s -h' for more information.\n", prog);
		exit(INVALID_OPTION);
	}
}

static int	check_flags(char *flags, size_t options)
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

static int	check_option(char *str, size_t *options)
{
	int			i;

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
		return (0);
	}
	return (2);
}

size_t		options(int ac, char **av)
{
	int			i;
	int			ret;
	int			files;
	size_t		options;

	i = 1;
	files = 0;
	options = 0;
	while (i < ac)
	{
		if ((ret = check_option(av[i], &options)) == 1)
			usage(av[0], av[i], USAGE);
		else if (ret == -1)
			usage(av[0], av[i], INVALID_OPTION);
		else if (ret == 2)
			files++;
		++i;
	}
	if (!check_flags("lartR", options))
		usage(av[0], av[i], USAGE);
	if (files > 1)
		options |= LS_PRPATH;
	else if (files == 0)
		options |= LS_NOFILES;
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
