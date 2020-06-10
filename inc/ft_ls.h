/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:21:18 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/10 18:44:19 by astripeb         ###   ########.fr       */
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
*/

# include <sys/stat.h>
# include <dirent.h>
# include <pwd.h>
# include <grp.h>
# include <sys/xattr.h>
# include <stdio.h>
# include <string.h>
# include <time.h>

# include "stdbool.h"
# include "libft.h"

enum
{
	USAGE,
	INVALID_OPTION,
	E_MALLOC,
	E_OPEN,
	E_SYS
};

# define LS_REC		0x20000
# define LS_ALL		0x4000000
# define LS_LONG	0x2000000000
# define LS_REV_S	0x80000000000
# define LS_TIME	0x200000000000

typedef struct	s_file
{
	char			*filename;
	char			*link;
	struct passwd	*user;
	struct group	*group;
	struct stat		f_stat;
}				t_file;


typedef struct	s_opts
{
	size_t		opts;
	bool		(*isshow)(char *filename);
	int			(*less)(void *p1, void *p2);
	void		(*print)(size_t opts, char *path, t_darr *files);
}				t_opts;


size_t		options(int ac, char **av);

void		print_opts(size_t opts);

t_opts		get_functors(size_t opts);


int			ft_read_root(t_opts *funct, char *path);

void		print_path(char *path);

void		ft_short_print(size_t opts, char *path, t_darr *list);

void		ft_long_print(size_t opts, char *path, t_darr *list);


void		ft_exit(int err, char *path);

void		ft_del_files(t_darr **files);

void		ft_error_handle(char *msg);
#endif
