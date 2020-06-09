/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 19:33:08 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/09 11:45:55 by astripeb         ###   ########.fr       */
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
		ft_bzero(&fptr->f_stat, sizeof(struct stat));
		// free(fptr->f_stat);
	}
}

void	ft_del_files(t_darr **files)
{
	ft_da_iter(*files, &free_content);
	ft_da_delete(files);
}

void	ft_error_handle(char *msg)
{
	ft_fprintf(STDERR_FILENO, "%s: ", msg);
	perror(NULL);
}
