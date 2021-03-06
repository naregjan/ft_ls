/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikourkji <ikourkji@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 17:39:43 by ikourkji          #+#    #+#             */
/*   Updated: 2019/04/26 22:07:11 by ikourkji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** that number is 6 mo in seconds
*/

static void	print_time(time_t sec, long flags)
{
	time_t	cur;
	char	*timestr;
	int		i;

	timestr = ctime(&sec);
	i = 4;
	while (i < 11)
		ft_putchar(timestr[i++]);
	if (flags & LS_UT)
		while (i < 24)
			ft_putchar(timestr[i++]);
	else
	{
		cur = time(0);
		if (cur - sec < 15552000)
			while (i < 16)
				ft_putchar(timestr[i++]);
		else
		{
			i = 19;
			while (i < 24)
				ft_putchar(timestr[i++]);
		}
	}
}

void		ls_print_name(t_lsent *ent, long flags)
{
	ssize_t	slen;

	if (flags & (LS_LL | LS_LS | LS_LI))
		ft_putchar(' ');
	if (flags & LS_UG)
		ls_set_color(ent);
	ft_printf("%s%{}", ent->name);
	if (flags & LS_LP && ent->ftype == 'd' && !(flags & LS_UF))
		ft_putchar('/');
	if (flags & LS_UF)
	{
		ent->ftype == '-' && ft_charat(ent->perms, 'x') > -1 ? \
					ft_putchar('*') : 0;
		ent->ftype == 'd' ? ft_putchar('/') : 0;
		ent->ftype == 'l' ? ft_putchar('@') : 0;
		ent->ftype == 's' ? ft_putchar('=') : 0;
		ent->ftype == 'p' ? ft_putchar('|') : 0;
	}
	if (ent->ftype == 'l')
	{
		slen = readlink(ent->full_name, ent->symbuf, sizeof(ent->symbuf) - 1);
		ent->symbuf[slen == -1 ? 0 : slen] = '\0';
		flags & LS_LL ? ft_printf(" -> %s", ent->symbuf) : 0;
	}
	ft_putchar('\n');
}

static void	print_uid_gid(t_lsent *ent, long fl)
{
	struct passwd	*uid;
	struct group	*gid;

	uid = getpwuid(ent->stats->st_uid);
	gid = getgrgid(ent->stats->st_gid);
	if (!(fl & LS_LG))
		fl & LS_LN ? ft_printf(" %d ", uid->pw_uid) : \
			ft_printf(" %s ", uid->pw_name);
	fl & LS_LN ? ft_printf(" %d ", gid->gr_gid) : \
		ft_printf(" %s ", gid->gr_name);
}

/*
** padding is one space before and after maxlen of every param
** requires storing of maximum length; can cheat though
** ("good enough" spacing)
** "ent" var is for convenience (not derefing run->content 9000 times)
** ls_print_name does colours/file symbols if flagged
*/

void		ls_lprint(t_list *run, long flags)
{
	t_lsent			*ent;

	while (run)
	{
		ent = run->content;
		if (ls_inode_block_skip(ent, flags) && (run = run->next))
			continue ;
		if (!run)
			break ;
		ft_printf("%c%s  %2d", \
				ent->ftype, ent->perms, ent->stats->st_nlink);
		print_uid_gid(ent, flags);
		ft_printf("%6ld ", ent->stats->st_size);
		flags & LS_LU ? print_time(ent->stats->st_atimespec.tv_sec, flags) :\
			print_time(ent->stats->st_mtimespec.tv_sec, flags);
		ls_print_name(ent, flags);
		run = run->next;
	}
}

void		ls_print(t_lsdir *dir, long flags)
{
	t_list	*run;
	t_lsent	*ent;

	if (!dir)
		return ;
	if (flags & LS_MU || (flags & LS_UR && dir->r_notfirst))
		ft_printf("%s:\n", dir->path);
	if (flags & (LS_LL | LS_LS))
		ft_printf("total %d\n", dir->tot);
	if (flags & LS_LL)
		ls_lprint(dir->entries, flags);
	else
	{
		run = dir->entries;
		while (run)
		{
			ent = run->content;
			if (ls_inode_block_skip(ent, flags) && (run = run->next))
				continue ;
			if (!run)
				break ;
			ls_print_name(ent, flags);
			run = run->next;
		}
	}
}
