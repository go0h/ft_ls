/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:21:18 by astripeb          #+#    #+#             */
/*   Updated: 2020/07/05 22:49:42 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

/*
**	<sys/stat.h>, <dirent.h>:	opendir, readdir, closedir
**	<pwd.h>:					getpwuid
**	<grp.h>:					getgrid
**	<sys/xattr.h>:				listxattr
**	<stdio.h>:					perror
**	<string.h>:					strerror
**	<time.h>:					time, ctime
**	<sys/sysmacros.h>			major, minor
*/

# include <sys/stat.h>
# include <dirent.h>
# include <pwd.h>
# include <grp.h>
# include <sys/xattr.h>
# include <stdio.h>
# include <string.h>
# include <time.h>
# include <sys/sysmacros.h>
# include <stdbool.h>

# include "libft.h"

typedef enum	e_error
{
	NONE,
	USAGE,
	INVALID_OPTION,
	SYS,
	MALLOC
}				t_error;

# define LS_REC			0x20000
# define LS_ALL			0x4000000
# define LS_LONG		0x2000000000
# define LS_REV_S		0x80000000000
# define LS_TIME		0x200000000000
# define LS_ONE_LINE	0x20000000000000
# define LS_NOFILES		0x1000000000000000
# define LS_FISRSTPRINT	0x2000000000000000
# define LS_NTOTL		0x4000000000000000
# define LS_PRPATH		0x8000000000000000

# define LS_6M			15778458

typedef struct	s_file
{
	char			*filename;
	char			*link;
	char			*username;
	char			*groupname;
	struct stat		f_stat;
}				t_file;

typedef struct	s_opts
{
	size_t		opts;
	bool		(*isshow)(char *filename);
	int			(*less)(void *p1, void *p2);
	void		(*print)(size_t opts, t_darr *files);
}				t_opts;

size_t			options(int ac, char **av);

t_opts			get_functors(size_t opts);

bool			show_all(char *filename);

bool			show_visible(char *filename);

int				ft_read_root(t_opts *funct, char *path);

int				ft_read_file_stat(size_t opts, t_file *file, char *path);

void			print_files(t_opts *funct, char *path, t_darr *list);

void			ft_short_print(size_t opts, t_darr *list);

void			ft_long_print(size_t opts, t_darr *list);

void			ft_error_handle(const char *msg, t_error err);

void			ft_del_one_file(t_darr *files, size_t index);

void			ft_del_files(t_darr **files);

bool			islink(mode_t mode);

bool			isblk(mode_t mode);

bool			ischr(mode_t mode);

int				strcmp_l(const char *str1, const char *str2);

#endif
