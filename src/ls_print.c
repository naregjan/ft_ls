/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikourkji <ikourkji@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 17:39:43 by ikourkji          #+#    #+#             */
/*   Updated: 2019/04/18 04:30:10 by ikourkji         ###   ########.fr       */
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

static void print_name(t_lsent *ent, long flags)
{
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
		ent->ftype == 's' ? ft_putchar('=') : 0;
		ent->ftype == 'p' ? ft_putchar('|') : 0;
	}
	ft_putchar('\n');
}

//need to store max hardlinks to get spacing right
//(link padding is one space before len maxlinks)
//actually padding is one space before maxlen of every param
//... you will either need to do something weird or implement wild (*) in pf
//^maybe implement * (takes arg for fw/prec) w/out $ (which is not arg?)
//(to get variable-width padding to format everything properly)
//(AND store maxlen for every param in dir) (maybe a maxlen struct??)
//"ent" for convenience
//print_name does colours/file symbols
static void	lprint(t_lsdir *dir, long flags)
{
	t_list			*run;
	t_lsent			*ent;
	struct passwd	*uid;
	struct group	*gid;

	ft_printf("total %d\n", dir->tot);
	run = dir->entries;
	while (run)
	{
		ent = run->content;
		if (ent->name[0] == '.' && !(flags & LS_LA) && (run = run->next))
			continue ;
		uid = getpwuid(ent->stats->st_uid);
		gid = getgrgid(ent->stats->st_gid);
		ft_printf("%c%s  %2d %s  %s %6ld ", \
				ent->ftype, ent->perms, ent->stats->st_nlink, \
				uid->pw_name, gid->gr_name, ent->stats->st_size);
		print_time(ent->stats->st_mtimespec.tv_sec, flags);
		print_name(ent, flags);
		run = run->next;
	}
}

void		ls_print(t_lsdir *dir, long flags)
{
	t_list	*run;
	t_lsent	*ent;

	if (flags & (LS_UR | LS_MU))
		ft_printf("%s:\n", dir->path);
	if (flags & LS_LL)
		lprint(dir, flags);
	else
	{
		run = dir->entries;
		while (run)
		{
			ent = run->content;
			if (ent->name[0] == '.' && !(flags & LS_LA) && (run = run->next))
				continue ;
			print_name(ent, flags);
			run = run->next;
		}
	}
}