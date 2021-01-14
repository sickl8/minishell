/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_mask_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:27:37 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/14 19:35:32 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <signal.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include "libft/libft.h"
#include "header_proto.h"
#include "header_typedef.h"
#include "header_def.h"
#include "header_extern.h"
#include "header_errors.h"

#include "header_get_next_line.h"

#include <stdio.h>

void	split_wmask(t_bm *rd, t_bm **wr, char c)
{
	size_t	i;
	size_t	cnt;

	cnt = 1;
	i = 0;
	while (rd->buf[i])
	{
		if (rd->msk[i] == c)
			cnt++;
		i++;
	}
	if (!(MALLOC(&(*wr), 24 * cnt + 24)))
		cleanup(EXIT);
	wr[0][cnt].msk = NULL;
	wr[0][cnt].buf = NULL;
	continue_split_wmask(rd, wr, cnt, c);
	rd->cnt = cnt;
}
