/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:22:13 by astripeb          #+#    #+#             */
/*   Updated: 2020/07/02 20:44:32 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <limits.h>

const char *g_prog_name;

/*
**	Read args from command line, and read all files with lstat function
**	and add to array of t_file structs
**	On success return t_file array
**	On error return return NULL
*/

static t_darr	*read_args(t_opts funct, int ac, char **argv)
{
	int		i;
	t_file	file;
	t_darr	*files;

	i = 0;
	ft_bzero(&file, sizeof(t_file));
	if (!(files = ft_da_new(ac - 1, sizeof(t_file))))
		return (NULL);
	while (++i < ac)
	{
		if (*argv[i] == '-')
			continue;
		file.filename = ft_strdup(argv[i]);
		if (!file.filename || ft_read_file_stat(funct.opts, &file, argv[i]))
		{
			ft_strdel(&file.filename);
			ft_error_handle(argv[i], SYS);
			continue;
		}
		files = ft_da_add(files, &file);
	}
	ft_da_sort(files, funct.less);
	return (files);
}

/*
**	Check if file is Directory
**	If opts -l is off and file is Symbolic link to directory,
**	read file with stat function.
**
**	On success return 1 if file Directory or 0 if other
**	On error return return -1
*/

static int		selection(size_t opts, t_file *file)
{
	__mode_t	file_type;

	file_type = file->f_stat.st_mode & S_IFMT;
	if (file_type == S_IFDIR)
		return (1);
	else if (file_type == S_IFLNK && !(opts & LS_LONG))
	{
		if (stat(file->filename, &file->f_stat) == -1)
			return (-1);
		file_type = file->f_stat.st_mode & S_IFMT;
		if (file_type == S_IFDIR)
			return (1);
	}
	return (0);
}

/*
**	Check files in array and add regfiles to new array
**	On success return t_file array of regular files
**	On error return return NULL
*/

static t_darr	*separate_files(size_t opts, t_darr *files)
{
	size_t	i;
	int		res;
	t_file	*file;
	t_darr	*regfiles;

	if (!files->size || !(regfiles = ft_da_new(files->size, sizeof(t_file))))
		return (NULL);
	i = 0;
	while (i < files->size)
	{
		file = (t_file*)ft_da_get_pointer(files, i);
		if ((res = selection(opts, file)) == -1)
		{
			ft_error_handle(file->filename, SYS);
			ft_del_one_file(files, i);
		}
		else if (res == 0)
		{
			regfiles = ft_da_add(regfiles, file);
			ft_da_del_index(files, i);
		}
		else
			++i;
	}
	return (regfiles);
}

/*
**	Prints files if it in args, and start to read args Directoties
*/

static void		ft_go_ls(t_opts funct, t_darr *dirs, t_darr *files, char *path)
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
		ft_read_root(&funct, ft_strcpy(path, file->filename));
	}
	ft_del_files(&dirs);
	ft_del_files(&files);
}

int				main(int argc, char **argv)
{
	t_opts	funct;
	char	*pathname;
	t_darr	*files;
	t_darr	*regfiles;

	g_prog_name = argv[0];
	funct = get_functors(options(argc, argv));
	if (!(pathname = ft_strnew(PATH_MAX - 1)))
		ft_error_handle(__FUNCTION__, MALLOC);
	if (funct.opts & LS_NOFILES)
		ft_read_root(&funct, ft_strcpy(pathname, "."));
	else
	{
		if (!(files = read_args(funct, argc, argv)))
		{
			ft_strdel(&pathname);
			ft_error_handle(__FUNCTION__, SYS);
		}
		regfiles = separate_files(funct.opts, files);
		ft_go_ls(funct, files, regfiles, pathname);
	}
	ft_strdel(&pathname);
	return (EXIT_SUCCESS);
}
