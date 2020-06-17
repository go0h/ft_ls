/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_long_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/09 08:32:58 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/17 18:23:27 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
**	S_IFMT	   00170000		MASK
**	S_IFSOCK	0140000		socket
**	S_IFLNK		0120000		symbolic (soft) link
**	S_IFREG		0100000		regular file
**	S_IFBLK		0060000		block device
**	S_IFDIR		0040000		directory
**	S_IFCHR		0020000		character device
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

static void	get_line_params(t_darr *files, size_t *params)
{
	size_t	i;
	t_file	*f_ptr;

	i = files->size;
	f_ptr = (t_file*)files->array;
	ft_bzero(params, sizeof(size_t) * 5);
	while (i-- != 0)
	{
		params[0] += f_ptr->f_stat.st_blocks;
		params[1] = ft_maxs(params[1], f_ptr->f_stat.st_nlink);
		params[2] = ft_maxs(params[2], ft_strlen(f_ptr->username));
		params[3] = ft_maxs(params[3], ft_strlen(f_ptr->groupname));
		params[4] = ft_maxs(params[4], f_ptr->f_stat.st_size);
		++f_ptr;
	}
	params[0] = params[0] / 2;
	params[1] = ft_size_t_len(params[1]);
	params[4] = ft_size_t_len(params[4]);
}

/*
**	Compare year in last status change in file and now time
**	swap time and year if not the same
**				____________
**			   |    |   |  |
**	Thu Jun 11 10:21:31 2020
**	   |____________|
*/

void		ft_print_time(struct stat *s)
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

void		print_files(t_opts *funct, char *path, t_darr *files)
{
	int	len;

	len = ft_strlen(path) - 1;
	if (funct->opts & LS_PRPATH)
		ft_printf("%.*s:\n", len, path);
	ft_da_sort(files, funct->less);
	funct->print(funct->opts, files);
	if (funct->opts & LS_FISRSTPRINT)
	{
		while (len > 1 && path[len] == '/' && path[len - 1] == '/')
			path[len--] = '\0';
		funct->opts = funct->opts & (~LS_FISRSTPRINT);
	}
}

void		ft_long_print(size_t opts, t_darr *files)
{
	size_t	i;
	t_file	*f_ptr;
	size_t	params[5];

	i = files->size;
	f_ptr = (t_file*)files->array;
	get_line_params(files, (size_t*)&params);
	if (!(opts & LS_NTOTL))
		ft_printf("total %lu\n", params[0]);
	while (i-- != 0)
	{
		ft_printf("%s %*lu %-*s %-*s %*lu",\
			get_roots(f_ptr->f_stat.st_mode),\
			params[1], f_ptr->f_stat.st_nlink,\
			params[2], f_ptr->username,\
			params[3], f_ptr->groupname,\
			params[4], f_ptr->f_stat.st_size);
		ft_print_time(&f_ptr->f_stat);
		if ((f_ptr->f_stat.st_mode & S_IFMT) == S_IFLNK)
			ft_printf("%s -> %s\n", f_ptr->filename, f_ptr->link);
		else
			ft_printf("%s\n", f_ptr->filename);
		++f_ptr;
	}
}
