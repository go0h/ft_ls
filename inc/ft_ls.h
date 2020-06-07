/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:21:18 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/07 23:45:14 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

/*
**	<sys/types.h>, <dirent.h>:	opendir, readdir, closedir
**	<pwd.h>:					getpwuid
**	<grp.h>:					getgrid
**	<sys/xattr.h>:				listxattr
**	<stdio.h>:					perror
**	<string.h>:					strerror
**	<time.h>:					time, ctime
*/

# include <sys/types.h>
# include <dirent.h>
# include <pwd.h>
# include <grp.h>
# include <sys/xattr.h>
# include <stdio.h>
# include <string.h>
# include <time.h>

# include "libft.h"

enum
{
	USAGE,
	INVALID_OPTION
};

# define LS_REC		0x20000
# define LS_ALL		0x4000000
# define LS_LONG	0x2000000000
# define LS_REV_S	0x80000000000
# define LS_TIME	0x200000000000

typedef struct s_file
{
	struct dirent	*d_ptr;
	struct stat		*st_ptr;
}				t_file;

size_t		options(int ac, char **av);

void		print_opts(size_t opts);

#endif
