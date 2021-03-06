/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_comps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikourkji <ikourkji@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 13:40:11 by ikourkji          #+#    #+#             */
/*   Updated: 2019/04/26 02:16:36 by ikourkji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** alphacomp for fname and dname t_lists
*/

int		ls_basic_alphacomp(t_list *n1, t_list *n2)
{
	return (ft_strcmp((char*)n1->content, (char*)n2->content));
}

int		ls_alphacomp(t_list *n1, t_list *n2)
{
	return (ft_strcmp(((t_lsent*)(n1->content))->name, \
				((t_lsent*)(n2->content))->name));
}

/*
** since mergesort is stablesort, run alphacomp first, and then the following;
** ls asks for all timesorts to do time and then alpha
** run alpha and then time to satisfy
** ALSO: the comparator needs to return an int and not a long/off_t/whatever
** and size among those can vary
** hence the solution below, instead of a one-line subtraction return
** and we *should* check for 0 in the (unlikely) event that they're the same
*/

int		ls_modtimecomp(t_list *n1, t_list *n2)
{
	if (((t_lsent*)(n1->content))->stats->st_mtimespec.tv_sec == \
		((t_lsent*)(n2->content))->stats->st_mtimespec.tv_sec)
		return (0);
	return (\
	((t_lsent*)(n1->content))->stats->st_mtimespec.tv_sec <\
		((t_lsent*)(n2->content))->stats->st_mtimespec.tv_sec ? 1 : -1);
}

int		ls_acctimecomp(t_list *n1, t_list *n2)
{
	if (((t_lsent*)(n1->content))->stats->st_atimespec.tv_sec == \
		((t_lsent*)(n2->content))->stats->st_atimespec.tv_sec)
		return (0);
	return (\
	((t_lsent*)(n1->content))->stats->st_atimespec.tv_sec <\
		((t_lsent*)(n2->content))->stats->st_atimespec.tv_sec ? 1 : -1);
}

int		ls_sizecomp(t_list *n1, t_list *n2)
{
	off_t	ret;

	ret = ((t_lsent*)(n1->content))->stats->st_size - \
		((t_lsent*)(n2->content))->stats->st_size;
	if (ret == 0)
		return (0);
	return (ret > 0 ? -1 : 1);
}
