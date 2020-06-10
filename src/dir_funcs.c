/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/08 20:26:59 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/10 20:14:26 by astripeb         ###   ########.fr       */
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
		ret = E_SYS;
	while (!ret && (dir = readdir(dirp_ptr)) != NULL)
	{
		if (funct->isshow(dir->d_name))
		{
			if (!(file.filename = ft_strdup(dir->d_name)))
				ret = E_MALLOC;
			if (ret || !(files = ft_da_add(files, &file)))
				ret = E_MALLOC;
		}
	}
	if (ret)
		ft_error_handle(path);
	closedir(dirp_ptr);
	return (ret);
}

int			ft_read_link(char *path, t_file *file)
{
	int		size;

	if ((file->f_stat.st_mode & S_IFMT) != S_IFLNK)
		return (EXIT_SUCCESS);
	size = !file->f_stat.st_size ? PATH_MAX : file->f_stat.st_size;
	if (!(file->link = ft_strnew(size)))
			return (EXIT_FAILURE);
	return (readlink(path, file->link, size) == -1);
}

void		ft_read_stats(size_t opts, t_darr *files, char *path)
{
	size_t	i;
	size_t	len;
	int		ret;
	t_file	*file;

	i = 0;
	len = ft_strlen(path);
	ft_strcpy(&path[len++], "/");
	while (i < files->size)
	{
		file = (t_file*)ft_da_get_pointer(files, i);
		ft_strcpy(&path[len], file->filename);
		ret = lstat(path, &file->f_stat);
		if (!ret && (opts & LS_LONG))
			ret = ft_read_link(path, file);
		if (ret)
		{
			ft_da_del_index(files, i--);	// !!! remove bad file from files
			ft_error_handle(path);
		}
		++i;
	}
	ft_bzero(&path[len], ft_strlen(file->filename));
}

void		ft_read_uid_gid(t_darr *files)
{
	size_t	i;
	t_file	*file;

	i = 0;
	while (i < files->size)
	{
		file = (t_file*)ft_da_get_pointer(files, i);
		file->user = getpwuid(file->f_stat.st_uid);
		file->group = getgrgid(file->f_stat.st_uid);
		if (!file->user || !file->group)
		{
			ft_da_del_index(files, i--);	//remove bad file from files
			ft_error_handle(NULL);
		}
		++i;
	}
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
			ft_read_root(funct, path);
		}
		++i;
	}
}

int			ft_read_root(t_opts *funct, char *path)
{
	t_darr			*files;

	if (!(files = ft_da_new(2, sizeof(t_file))))
		return (E_MALLOC);
	if ((ft_read_dir(funct, path, files)) != 0)
	{
		ft_del_files(&files);
		return (EXIT_FAILURE);
	}
	ft_read_stats(funct->opts, files, path);
	if (funct->opts & LS_LONG)
		ft_read_uid_gid(files);
	ft_da_sort(files, funct->less);
	funct->print(funct->opts, path, files);
	if (funct->opts & LS_REC)
		ft_process_next(funct, files, path);
	ft_del_files(&files);
	return (EXIT_SUCCESS);
}
