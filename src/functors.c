/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functors.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/08 20:33:03 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/09 20:57:09 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

bool	show_all(char *filename)
{
	return (filename != NULL);
}

bool	show_visible(char *filename)
{
	return (*filename != '.');
}

int		name_less(void *p1, void *p2)
{
	int		i;
	char	*s1;
	char	*s2;

	s1 = (char*)&((t_file*)p1)->d_ptr.d_name;
	s2 = (char*)&((t_file*)p2)->d_ptr.d_name;
	i = 0;
	while (s1[i] || s2[i])
	{
		if (ft_tolower(s1[i]) != ft_tolower(s2[i]))
			break ;
		++i;
	}
	return ((ft_tolower(s1[i]) - ft_tolower(s2[i])) < 0);
}

int		rev_name(void *p1, void *p2)
{
	return (name_less(p2, p1));
}

int		time_less(void *p1, void *p2)
{
	struct timespec *t1;
	struct timespec *t2;

	t1 = &((t_file*)p1)->f_stat.st_mtim;
	t2 = &((t_file*)p2)->f_stat.st_mtim;
	if (t1->tv_sec > t2->tv_sec)
		return (1);
	else if (t1->tv_sec == t2->tv_sec)
		return (t1->tv_nsec > t2->tv_nsec);
	return (0);
}

int		rev_time(void *p1, void *p2)
{
	return (time_less(p2, p1));
}

t_opts	get_functors(size_t opts)
{
	t_opts	funct;

	ft_bzero(&funct, sizeof(t_opts));
	funct.opts = opts;
	funct.print = &print_content;
	funct.isshow = opts & LS_ALL ? &show_all : &show_visible;
	if (opts & LS_TIME)
		funct.less = opts & LS_REV_S ? &rev_time : &time_less;
	else
		funct.less = opts & LS_REV_S ? &rev_name : &name_less;

	return (funct);
}