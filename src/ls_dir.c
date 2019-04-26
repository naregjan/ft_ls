/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_dir.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikourkji <ikourkji@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 16:47:22 by ikourkji          #+#    #+#             */
/*   Updated: 2019/04/26 02:14:24 by ikourkji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	make_entries(t_lsdir *dir, long flags)
{
	struct dirent	*entry;
	t_lsent			*ls_entry;

	while ((entry = readdir(dir->dir)))
	{
		ls_entry = malloc(sizeof(*ls_entry));
		ls_entry->name = entry->d_name;
		ft_asprintf(&(ls_entry->full_name), "%s/%s", dir->path, entry->d_name);
		ls_entry->stats = malloc(sizeof(*ls_entry->stats));
		lstat(ls_entry->full_name, ls_entry->stats);
		ls_get_ftype(ls_entry);
		ls_get_perms(ls_entry);
		dir->tot += entry->d_name[0] != '.' || flags & (LS_LA | LS_UA) ? \
					ls_entry->stats->st_blocks : 0;
		dir->tot -= flags & LS_UA && (!(ft_strcmp(entry->d_name, ".")) || \
					!(ft_strcmp(entry->d_name, ".."))) ? \
					ls_entry->stats->st_blocks : 0;
		ft_lstaddend(&(dir->entries), ft_lstnew(ls_entry, sizeof(*ls_entry)));
	}
}

/*
** using a dispatch table is actually *less descriptive* than this
** I would have had to pass them manually anyway, only without helpful names
*/

static void	sort_ents(t_list **ents, long fl)
{
	ft_lstmsort(ents, fl & LS_LR ? &ls_revalpha : &ls_alphacomp);
	if (fl & LS_US)
		ft_lstmsort(ents, fl & LS_LR ? &ls_revsize : &ls_sizecomp);
	else if (fl & (LS_LT | LS_LU))
		ft_lstmsort(ents, fl & LS_LR ? &ls_revacctime : &ls_acctimecomp);
	else if (fl & LS_LT)
		ft_lstmsort(ents, fl & LS_LR ? &ls_revmodtime : &ls_modtimecomp);
}

t_lsdir		*ls_mkdir(char *name, long flags, char *parent)
{
	t_lsdir	*dir;

	dir = ft_memalloc(sizeof(*dir));
	parent ? ft_asprintf(&(dir->path), "%s/%s", parent, name) : \
		ft_asprintf(&(dir->path), "%s", name);
	dir->dir = opendir(dir->path);
	dir->entries = NULL;
	if (!dir->dir)
		return (dir);//is this a good idea?
	make_entries(dir, flags);
	sort_ents(&(dir->entries), flags);
	return (dir);
}

static void	rmentries(t_list *entries)
{
	t_list	*run;
	t_lsent	*entry;

	if (!entries)
		return ;
	run = entries->next;
	entry = entries->content;
	//free(entry->name); ??
	free(entry->full_name);
	free(entry->stats);
	free(entry->perms);
	free(entries);
	//free(entry->owner);
	if (run)
		rmentries(run);
}

void		ls_rmdir(t_lsdir *dir)
{
	rmentries(dir->entries);
	free(dir->path);
	closedir(dir->dir);
	free(dir);
	dir = NULL;
}
