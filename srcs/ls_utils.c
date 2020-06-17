/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 19:33:08 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/17 18:23:43 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

extern const char *g_prog_name;

static void	usage(const char *opt, t_error err)
{
	if (err == USAGE)
	{
		ft_printf("Usage: ft_ls [OPTION]... [FILE]...\n");
		ft_printf("   -a\tdo not ignore entries starting with .\n");
		ft_printf("   -l\tuse a long listing format\n");
		ft_printf("   -R\tlist subdirectories recursively\n");
		ft_printf("   -r\treverse order while sorting\n");
		ft_printf("   -t\tsort by modification time, newest first\n");
		exit(EXIT_SUCCESS);
	}
	else if (err == INVALID_OPTION)
	{
		ft_printf("%s: invalid option -- '%s'\n", g_prog_name, opt);
		ft_printf("Try '%s -h' for more information.\n", g_prog_name);
		exit(INVALID_OPTION);
	}
}

void		ft_error_handle(const char *msg, t_error err)
{
	if (err == USAGE || err == INVALID_OPTION)
		usage(msg, err);
	else
	{
		ft_fprintf(STDERR_FILENO, "%s: ", msg);
		perror(NULL);
	}
}

void		free_content(void *content)
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

void		ft_del_files(t_darr **files)
{
	ft_da_iter(*files, &free_content);
	ft_da_delete(files);
}

void		ft_del_one_file(t_darr *files, size_t index)
{
	t_file *f_ptr;

	f_ptr = ft_da_get_pointer(files, index);
	free_content(f_ptr);
	ft_da_del_index(files, index);
}
