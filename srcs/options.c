/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 11:09:56 by pcredibl          #+#    #+#             */
/*   Updated: 2020/06/16 16:16:51 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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
		else if (*flags > 47 && *flags < 58)
		{
			if ((options >> (*flags + 4)) & 1)
				options ^= (1L << (*flags + 4));
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
			else if (str[i] > 47 && str[i] < 58)
				*options |= (1L << (str[i] + 4));
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
			ft_error_handle(NULL, USAGE);
		else if (ret == -1)
			ft_error_handle(av[i], INVALID_OPTION);
		else if (ret == 2)
			files++;
		++i;
	}
	if (!check_flags("lartR1", options))
		ft_error_handle(NULL, USAGE);
	if (files > 1)
		options |= LS_PRPATH;
	else if (files == 0)
		options |= LS_NOFILES;
	return (options);
}

bool		show_all(char *filename)
{
	return (filename != NULL);
}

bool		show_visible(char *filename)
{
	return (*filename != '.');
}
