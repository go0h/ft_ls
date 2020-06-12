/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:22:13 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/12 22:07:08 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <limits.h>

t_darr	*read_args(t_opts funct, int ac, char **argv)
{
	int		i;
	t_file	file;
	t_darr	*files;

	if (!(files = ft_da_new(ac - 1, sizeof(t_file))))
	{
		ft_error_handle(__FUNCTION__);
		return (NULL);
	}
	ft_bzero(&file, sizeof(t_file));
	i = 0;
	while (++i < ac)
	{
		if (*argv[i] == '-')
			continue;
		file.filename = ft_strdup(argv[i]);
		if (!file.filename || ft_read_file_stat(funct.opts, &file, argv[i]))
		{
			ft_error_handle(__FUNCTION__);
			continue;
		}
		files = ft_da_add(files, &file);
	}
	ft_da_sort(files, funct.less);
	return (files);
}

t_darr	*separate_files(t_darr *files)
{
	size_t		i;
	t_file		*file;
	t_darr		*regfiles;
	__mode_t	file_type;

	if (!files->size || !(regfiles = ft_da_new(files->size, sizeof(t_file))))
		return (NULL);
	i = 0;
	while (i < files->size)
	{
		file = (t_file*)ft_da_get_pointer(files, i);
		file_type = file->f_stat.st_mode & S_IFMT;
		if (file_type == S_IFDIR)
		{
			++i;
			continue;
		}
		regfiles = ft_da_add(regfiles, file);
		ft_da_del_index(files, i);
	}
	return (regfiles);
}

void	ft_go_ls(t_opts funct, t_darr *dirs, t_darr *files, char *path)
{
	size_t	i;
	t_file	*file;

	if (files && files->size)
	{
		funct.print(funct.opts | LS_NTOTL, files);
		dirs->size ? ft_printf("\n") : 0;
	}
	i = 0;
	while (i < dirs->size)
	{
		i != 0 ? ft_printf("\n") : 0;
		file = ft_da_get_pointer(dirs, i++);
		ft_strcpy(path, file->filename);
		ft_read_root(&funct, path);
	}
	i == 0 ? ft_read_root(&funct, path) : 0;
	ft_del_files(&dirs);
	ft_del_files(&files);
}

int		main(int argc, char **argv)
{
	t_opts	funct;
	char	*pathname;
	t_darr	*files;
	t_darr	*regfiles;

	funct = get_functors(options(argc, argv));
	if (!(pathname = ft_strnew(PATH_MAX - 1)))
		ft_exit(E_MALLOC, __FUNCTION__);
	ft_strcpy(pathname, ".");
	if (!(files = read_args(funct, argc, argv)))
	{
		ft_strdel(&pathname);
		ft_exit(E_MALLOC, __FUNCTION__);
	}
	if (files->size > 1)
		funct.opts |= LS_PRPATH;
	regfiles = separate_files(files);
	ft_go_ls(funct, files, regfiles, pathname);
	ft_strdel(&pathname);
	return (EXIT_SUCCESS);
}
