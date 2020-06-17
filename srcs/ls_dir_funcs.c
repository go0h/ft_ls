/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_dir_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/08 20:26:59 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/17 18:23:19 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <limits.h>

static bool	not_ref(char *filename)
{
	return (ft_strcmp(filename, ".") && ft_strcmp(filename, ".."));
}

int			ft_read_dir(t_opts *funct, char *path, t_darr *files)
{
	DIR				*dirp_ptr;
	struct dirent	*dir;
	t_file			file;
	int				ret;

	ret = 0;
	ft_bzero(&file, sizeof(t_file));
	if (!(dirp_ptr = opendir(path)))
		ret = SYS;
	while (!ret && (dir = readdir(dirp_ptr)) != NULL)
	{
		if (funct->isshow(dir->d_name))
		{
			if (!(file.filename = ft_strdup(dir->d_name)))
				ret = MALLOC;
			if (ret || !(files = ft_da_add(files, &file)))
				ret = MALLOC;
		}
	}
	if (ret)
		ft_error_handle(path, ret);
	closedir(dirp_ptr);
	return (ret);
}

void		ft_read_stats(size_t opts, t_darr *files, char *path)
{
	size_t	i;
	size_t	len;
	t_file	*file;

	i = 0;
	len = ft_strlen(path);
	ft_strcpy(&path[len++], "/");
	while (i < files->size)
	{
		file = (t_file*)ft_da_get_pointer(files, i);
		ft_strcpy(&path[len], file->filename);
		if (ft_read_file_stat(opts, file, path))
		{
			ft_del_one_file(files, i--);
			ft_error_handle(path, SYS);
		}
		++i;
	}
	ft_bzero(&path[len], 256);
}

void		ft_process_next(t_opts *funct, t_darr *files, char *path)
{
	size_t	i;
	size_t	len;
	t_file	*file;

	i = 0;
	len = ft_strlen(path);
	while (i < files->size)
	{
		file = ft_da_get_pointer(files, i);
		if ((file->f_stat.st_mode & S_IFDIR) && not_ref(file->filename))
		{
			ft_strcpy(&path[len], file->filename);
			ft_printf("\n");
			ft_read_root(funct, path);
		}
		++i;
	}
}

int			ft_read_root(t_opts *funct, char *path)
{
	t_darr			*files;

	if (!(files = ft_da_new(2, sizeof(t_file))))
		return (MALLOC);
	if ((ft_read_dir(funct, path, files)) != 0)
	{
		ft_del_files(&files);
		return (EXIT_FAILURE);
	}
	ft_read_stats(funct->opts, files, path);
	print_files(funct, path, files);
	if (funct->opts & LS_REC)
		ft_process_next(funct, files, path);
	ft_del_files(&files);
	return (EXIT_SUCCESS);
}
