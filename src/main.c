/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astripeb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 14:22:13 by astripeb          #+#    #+#             */
/*   Updated: 2020/06/08 00:03:33 by astripeb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	free_content(void *content, size_t size)
{
	if (size)
	{
		free(content);
		ft_bzero(content, size);
	}
}

void	print_content(t_list *list_content)
{
	t_file	*f_ptr;

	while (list_content != NULL && list_content->content)
	{
		f_ptr = (t_file*)list_content->content;
		ft_printf("%lu: %s\n", f_ptr->d_ptr->d_ino, f_ptr->d_ptr->d_name);
		list_content = list_content->next;
	}
}

int		rd(size_t opts, char *path)
{
	DIR				*dirp_ptr;
	struct dirent	*d_ptr;
	t_file			file;
	t_list			*list_cont;
	t_list			*temp;

	ft_bzero(&file, sizeof(t_file));
	if (!(dirp_ptr = opendir(path)))
	{
		ft_printf("Can't open dir %s\n", path);
		return (0);
	}
	while ((file.d_ptr = readdir(dirp_ptr)) != NULL)
	{
		if (!(temp = ft_lstnew(&file, sizeof(t_file))))
		{
			ft_lstdel(&list_cont, &free_content);
			return (MALLOC_FAILURE);
		}
		ft_lstadd(&list_cont, temp);
	}
	closedir(dirp_ptr);
	ft_lstdel(&list_cont, &free_content);
	return (1);
}

int		main(int argc, char **argv)
{
	size_t opts;
	int i;

	opts = options(argc, argv);
	if (argc == 1)
		return (EXIT_SUCCESS);
	print_opts(opts);
	rd(opts, argv[1]);
	return (EXIT_SUCCESS);
}
