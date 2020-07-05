/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_long_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/09 08:32:58 by astripeb          #+#    #+#             */
/*   Updated: 2020/07/05 14:51:22 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
**	S_IFMT	   00170000		MASK
**	S_IFSOCK	0140000		socket					/tmp/.X11-unix/
**	S_IFLNK		0120000		symbolic (soft) link
**	S_IFREG		0100000		regular file
**	S_IFBLK		0060000		block device			/dev/sda
**	S_IFDIR		0040000		directory
**	S_IFCHR		0020000		character device		/dev/null
**	S_IFIFO		0010000		FIFO or pipe
**
**	S_ISUID		0004000		set-user-ID
**	S_ISGID		0002000		set-group-ID
**	S_ISVTX		0001000		sticky bit
**	"?pcdb-lswd";
*/

static char	*get_roots(mode_t st_mode)
{
	static char	types[] = "pcdb-ls";
	static char	template[] = "-rwxrwxrwx";
	static char	roots[11];

	roots[0] = types[((st_mode & S_IFMT) >> 13)];
	roots[1] = st_mode & S_IRUSR ? template[1] : '-';
	roots[2] = st_mode & S_IWUSR ? template[2] : '-';
	roots[3] = st_mode & S_IXUSR ? template[3] : '-';
	roots[4] = st_mode & S_IRGRP ? template[4] : '-';
	roots[5] = st_mode & S_IWGRP ? template[5] : '-';
	roots[6] = st_mode & S_IXGRP ? template[6] : '-';
	roots[7] = st_mode & S_IROTH ? template[7] : '-';
	roots[8] = st_mode & S_IWOTH ? template[8] : '-';
	roots[9] = st_mode & S_IXOTH ? template[9] : '-';
	if (st_mode & S_ISUID)
		roots[3] = st_mode & S_IXUSR ? 's' : 'S';
	if (st_mode & S_ISGID)
		roots[6] = st_mode & S_IXGRP ? 's' : 'S';
	if (st_mode & S_ISVTX)
		roots[9] = st_mode & S_IXOTH ? 't' : 'T';
	return ((char*)&roots);
}

/*
**	PARAMS ARRAY indexes:
**	0 - sum of blocks of all files in dir
**	1 - max links in one file of all files in dir
**	2 - max length of usernames
**	3 - max length of groupnames
**	4 - max device major number (only for char and block devices)
**	5 - max device minor number (only for char and block devices)
**	6 - max size of file size
*/

static void	get_line_param(t_darr *files, size_t *param)
{
	size_t	i;
	t_file	*f_ptr;

	i = files->size;
	f_ptr = (t_file*)files->array;
	ft_bzero(param, sizeof(size_t) * 7);
	while (i-- != 0)
	{
		param[0] += f_ptr->f_stat.st_blocks;
		param[1] = ft_maxs(param[1], f_ptr->f_stat.st_nlink);
		param[2] = ft_maxs(param[2], ft_strlen(f_ptr->username));
		param[3] = ft_maxs(param[3], ft_strlen(f_ptr->groupname));
		if (isblk(f_ptr->f_stat.st_mode) || ischr(f_ptr->f_stat.st_mode))
		{
			param[4] = ft_maxs(param[4], major(f_ptr->f_stat.st_rdev));
			param[5] = ft_maxs(param[5], minor(f_ptr->f_stat.st_rdev));
		}
		param[6] = ft_maxs(param[6], f_ptr->f_stat.st_size);
		++f_ptr;
	}
	param[0] = param[0] / 2;
	param[1] = ft_size_t_len(param[1]);
	param[4] = param[4] ? ft_size_t_len(param[4]) : 0;
	param[5] = param[5] ? ft_size_t_len(param[5]) : 0;
	param[6] = param[4] ? param[4] + param[5] + 1 : ft_size_t_len(param[6]);
}

/*
**	Compare year in last status change in file and now time
**	swap time and year if not the same
**				____________
**			   |    |   |  |
**	Thu Jun 11 10:21:31 2020
**	   |____________|
*/

static void	ft_print_time(struct stat *s)
{
	time_t		n_time;
	time_t		f_time;
	char		*file_time;

	f_time = s->st_mtime;
	n_time = time(NULL);
	file_time = ctime(&f_time);
	if (f_time - n_time > LS_6M || n_time - f_time > LS_6M)
		ft_printf("%.7s %5.4s ", (file_time + 3), (file_time + 20));
	else
		ft_printf("%.13s ", (file_time + 3));
}

static void	ft_print_size(struct stat *s, size_t *param)
{
	if (isblk(s->st_mode) || ischr(s->st_mode))
	{
		ft_printf("%*lu,%*lu",\
		param[4], major(s->st_rdev),\
		param[5], minor(s->st_rdev));
	}
	else
		ft_printf("%*lu", param[6], s->st_size);
}

void		ft_long_print(size_t opts, t_darr *files)
{
	size_t	i;
	t_file	*f_ptr;
	size_t	param[7];

	i = files->size;
	f_ptr = (t_file*)files->array;
	get_line_param(files, (size_t*)&param);
	if (!(opts & LS_NTOTL))
		ft_printf("total %lu\n", param[0]);
	while (i-- != 0)
	{
		ft_printf("%s %*lu %-*s %-*s ",\
			get_roots(f_ptr->f_stat.st_mode),\
			param[1], f_ptr->f_stat.st_nlink,\
			param[2], f_ptr->username,\
			param[3], f_ptr->groupname);
		ft_print_size(&f_ptr->f_stat, param);
		ft_print_time(&f_ptr->f_stat);
		if (islink(f_ptr->f_stat.st_mode))
			ft_printf("%s -> %s\n", f_ptr->filename, f_ptr->link);
		else
			ft_printf("%s\n", f_ptr->filename);
		++f_ptr;
	}
}
