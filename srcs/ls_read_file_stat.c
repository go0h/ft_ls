/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_read_file_stat.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 10:00:25 by astripeb          #+#    #+#             */
/*   Updated: 2020/07/05 14:57:35 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int			ft_read_file_stat(size_t opts, t_file *file, char *path)
{
	size_t			size;
	struct passwd	*user;
	struct group	*group;

	if (lstat(path, &file->f_stat) == -1)
		return (EXIT_FAILURE);
	if (!(opts & LS_LONG))
		return (EXIT_SUCCESS);
	user = getpwuid(file->f_stat.st_uid);
	group = getgrgid(file->f_stat.st_gid);
	if (!user || !group)
		return (EXIT_FAILURE);
	file->username = ft_strdup(user->pw_name);
	file->groupname = ft_strdup(group->gr_name);
	if (!file->username || !file->groupname)
		return (EXIT_FAILURE);
	if (!islink(file->f_stat.st_mode))
		return (EXIT_SUCCESS);
	size = !file->f_stat.st_size ? PATH_MAX : file->f_stat.st_size;
	if (!(file->link = ft_strnew(size)))
		return (EXIT_FAILURE);
	return (readlink(path, file->link, size) == -1);
}

bool		islink(mode_t mode)
{
	return ((mode & S_IFMT) == S_IFLNK);
}

bool		isblk(mode_t mode)
{
	return ((mode & S_IFMT) == S_IFBLK);
}

bool		ischr(mode_t mode)
{
	return ((mode & S_IFMT) == S_IFCHR);
}
