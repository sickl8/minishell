/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:01:33 by aamzouar          #+#    #+#             */
/*   Updated: 2021/01/18 17:32:25 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "libft/libft.h"
#include "header_proto.h"
#include "header_typedef.h"
#include "header_def.h"
#include "header_extern.h"
#include "header_errors.h"

#include <stdio.h>

char	*combine_name_with_value(int j)
{
	char	*env_var;
	char	*tmp;

	env_var = ft_strjoin(g_line->env_var[j].name, "=");
	tmp = env_var;
	env_var = ft_strjoin(env_var, g_line->env_var[j].value);
	free(tmp);
	return (env_var);
}

char	**env_var_copy(char *path2exec)
{
	char	**the_copy;
	int		len;
	int		i;
	int		j;

	len = 0;
	while (g_line->env_var[len].name)
		len++;
	if (!(the_copy = malloc(sizeof(char *) * (len + 2))))
		cleanup(EXIT);
	i = 0;
	j = 0;
	while (g_line->env_var[j].name)
	{
		if (g_line->env_var[j].value && CMP(g_line->env_var[j].name, "_"))
			the_copy[i++] = combine_name_with_value(j);
		j++;
	}
	the_copy[i++] = ft_strjoin("_=", path2exec);
	the_copy[i] = NULL;
	return (the_copy);
}

void	parent_stuff(t_cmd *data)
{
	if (CMP(data->find, "exit"))
		g_program_return = 0;
	if (!CMP(data->find, "cd") && g_cmds_length == 1)
		assign(&g_parent, 1, 4) && bc_cd(data);
	else if (!CMP(data->find, "export"))
		assign(&g_parent, 1, 4) &&
		(!data->args[1] ? 0 : bc_export_bk(data));
	else if (!CMP(data->find, "unset"))
		assign(&g_parent, 1, 4) && bc_unset(data);
	else if (!CMP(data->find, "exit"))
		assign(&g_parent, 1, 4) && bc_exit(data->args, data);
}

void	execute_builtins_continue(t_cmd *data, int *pfd, int cmd, int j)
{
	int		bk[2];

	bk[0] = dup(0);
	bk[1] = dup(1);
	dup2(pfd[j - 1], 0);
	close(pfd[j - 1]);
	dup2(pfd[j], 1);
	close(pfd[j]);
	make_a_redirection(data->redir);
	builtin(data, cmd);
	exit(0);
}

int		execute_builtins(t_cmd *data, int *pfd)
{
	int		j;
	int		cmd;

	j = 1;
	while (data)
	{
		if (data->find && (cmd = is_builtin(data->find)))
		{
			g_parent = 0;
			g_pid = fork();
			if (g_pid == -1)
				return (eerf(pfd) && !failing_error(data));
			if (g_pid == 0)
				execute_builtins_continue(data, pfd, cmd, j);
			close(pfd[j - 1]);
			close(pfd[j]);
			parent_stuff(data);
		}
		j += 2;
		data = data->next;
	}
	return (0);
}
