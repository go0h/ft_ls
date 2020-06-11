/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 19:33:08 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/11 20:18:15 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_exit(int err, char *path)
{
	if (err == E_OPEN)
		ft_printf("Can't open dir %s\n", path);
	exit(err);
}

void	free_content(void *content)
{
	t_file	*fptr;

	fptr = (t_file*)content;
	if (content != NULL)
	{
		ft_strdel(&fptr->filename);
		ft_strdel(&fptr->username);
		ft_strdel(&fptr->groupname);
		ft_strdel(&fptr->link);
	}
}

void	ft_del_files(t_darr **files)
{
	ft_da_iter(*files, &free_content);
	ft_da_delete(files);
}

void	ft_del_one_file(t_darr *files, size_t index)
{
	t_file *f_ptr;

	f_ptr = ft_da_get_pointer(files, index);
	free_content(&f_ptr);
	ft_da_del_index(files, index);
}

void	ft_error_handle(char *msg)
{
	ft_fprintf(STDERR_FILENO, "%s: ", msg);
	perror(NULL);
}
