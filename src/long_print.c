/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   long_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/09 08:32:58 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/10 22:48:23 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
	#define S_IFMT	00170000 MASK
	#define S_IFSOCK	0140000 socket
	#define S_IFLNK	0120000	symbolic (soft) link
	#define S_IFREG  0100000 regular file
	#define S_IFBLK  0060000 block device
	#define S_IFDIR  0040000	directory
	#define S_IFCHR  0020000	character device
	#define S_IFIFO  0010000	FIFO or pipe

	#define S_ISUID  0004000
	#define S_ISGID  0002000
	#define S_ISVTX  0001000
*/

static char	*get_roots(mode_t st_mode)
{
	static char	types[] = "fcdb-ls";
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
	return ((char*)&roots);
}

static void	get_line_params(t_darr *files, size_t *params)
{
	size_t	i;
	t_file	*f_ptr;

	i = 0;
	f_ptr = (t_file*)files->array;
	ft_bzero(params, sizeof(size_t) * 5);
	while (i < files->size)
	{
		params[0] += f_ptr->f_stat.st_blocks;
		params[1] = ft_max(params[1], f_ptr->f_stat.st_nlink);
		params[2] = ft_max(params[2], ft_strlen(f_ptr->user->pw_name));
		params[3] = ft_max(params[3], ft_strlen(f_ptr->group->gr_name));
		params[4] = ft_max(params[4], f_ptr->f_stat.st_size);
		++i;
		++f_ptr;
	}
	params[0] = params[0] / 2;
	params[1] = ft_int_len(params[1]);
	params[4] = ft_int_len(params[4]);
}

static char	*ft_ls_time(time_t c_time)
{
	char		now_time[256];
	char		*file_time;
	time_t		now;

	now = time(NULL);
	ft_strcpy(now_time, ctime(&now));
	file_time = ft_strchr(ctime(&c_time), ' ');
	// if (!(ft_strcmp(ft_strrchr(file_time, ' '), ft_strrchr(now_time, ' '))))
		*ft_strrchr(file_time, ':') = 0;
	// else
	// {
	// 	char *temp = ft_strrchr(file_time, ':');
	// 	ft_strcpy(temp, ft_strrchr(temp, ' '));
	// 	*ft_strrchr(file_time, '\n') = '\0';
	// }
	return (file_time);
}

void		print_path(char *path)
{
	static bool first = true;
	size_t		len;

	if (first)
	{
		ft_printf("%s:\n", path);
		while (*path++)
		{
			if (*path == '/' && *(path + 1) == '/')
				*(path + 1) = '\0';
		}
		first = false;
	}
	else	// BAD!!
	{
		len = ft_strlen(path);
		path[len - 1] = '\0';
		ft_printf("\n%s:\n", path);
		path[len - 1] = '/';
	}
}

void		ft_long_print(size_t opts, char *path, t_darr *files)
{
	size_t	i;
	t_file	*f_ptr;
	size_t	params[5];

	i = 0;
	f_ptr = (t_file*)files->array;
	get_line_params(files, (size_t*)&params);
	if (opts & LS_REC)
		print_path(path);
	ft_printf("total %lu", params[0]);
	while (i < files->size)
	{
		ft_printf("\n%s %*lu %*s %*s %*lu%s %s",\
			get_roots(f_ptr->f_stat.st_mode),\
			params[1], f_ptr->f_stat.st_nlink,\
			params[2], f_ptr->user->pw_name,\
			params[3], f_ptr->group->gr_name,\
			params[4], f_ptr->f_stat.st_size,\
			ft_ls_time(f_ptr->f_stat.st_ctime), f_ptr->filename);
		if ((f_ptr->f_stat.st_mode & S_IFMT) == S_IFLNK)
			ft_printf(" -> %s", f_ptr->link);
		++i;
		++f_ptr;
	}
	ft_printf("\n");
}
