/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/08 20:26:59 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/09 20:36:55 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static bool	not_ref(char *filename)
{
	return (ft_strcmp(filename, ".") && ft_strcmp(filename, ".."));
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
		if (not_ref(file->d_ptr.d_name) && file->d_ptr.d_type & DT_DIR)
		{
			ft_strcpy(&path[len], file->d_ptr.d_name);
			ft_read_root(funct, path);
		}
		++i;
	}
}

void		ft_read_stats(t_darr *files, char *path)
{
	size_t	i;
	int		ret;
	size_t	len;
	t_file	*file;

	i = 0;
	len = ft_strlen(path);
	if (path[len - 1] != '/')
		ft_strcpy(&path[len++], "/");
	while (i < files->size)
	{
		file = (t_file*)ft_da_get_pointer(files, i);
		ft_strcpy(&path[len], file->d_ptr.d_name);
		if (file->d_ptr.d_type & DT_LNK)
			ret = lstat(path, &file->f_stat);
		else
			ret = stat(path, &file->f_stat);
		if (ret == -1)
		{
			ft_da_del_index(files, i--);	//remove bad file from files
			ft_error_handle(path);
		}
		++i;
	}
	ft_bzero(&path[len], NAME_MAX);
}

int			ft_read_dir(t_opts *funct, char *path, t_darr *files)
{
	DIR				*dirp_ptr;
	struct dirent	*dir;
	t_file			file;

	if (!(dirp_ptr = opendir(path)))
	{
		ft_error_handle(path);
		return (EXIT_FAILURE);
	}
	while ((dir = readdir(dirp_ptr)) != NULL)
	{
		if (funct->isshow(dir->d_name))
		{
			ft_bzero(&file, sizeof(t_file));
			ft_memcpy(&file.d_ptr, dir, dir->d_reclen);
			if (!(files = ft_da_add(files, &file)))
			{
				closedir(dirp_ptr);
				ft_error_handle(path);
				return (E_MALLOC);
			}
		}
	}
	closedir(dirp_ptr);
	return (EXIT_SUCCESS);
}

int			ft_read_root(t_opts *funct, char *path)
{
	t_darr			*files;

	if (!(files = ft_da_new(1, sizeof(t_file))))
		return (E_MALLOC);
	if ((ft_read_dir(funct, path, files)) != 0)
	{
		ft_del_files(&files);
		return (EXIT_FAILURE);
	}
	ft_read_stats(files, path);
	ft_da_sort(files, funct->less);
	funct->print(path, files);
	if (funct->opts & LS_REC)
		ft_process_next(funct, files, path);
	ft_del_files(&files);
	return (EXIT_SUCCESS);
}
