* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:08:48 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/18 14:53:05 by aamzouar         ###   ########.fr       */
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
#include "proto.h"
#include "typedef.h"
#include "def.h"
#include "global.h"
#include "errors.h"

#include <stdio.h>

int		check_var_name(char *name, int end)
{
	int		i;

	i = 0;
	if (!ft_isalpha(name[i]) && name[i] != '_')
		return (0);
	i++;
	while (i < end)
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void		duplicated_variable(char *arg_name, int len)
{
	int	i;
	char	tmp[len + 1];

	i = 0;
	// if it's name only without equal don't consider it so i won't add it to env
	ft_strncpy(tmp, arg_name, len);
	tmp[len] = '\0';
	if (arg_name[len - 1] != '\0' && g_dup == 0)
	{
		while (g_line->env_var[i].name)
		{
			if (!CMP(tmp, g_line->env_var[i].name))
			{
				g_dup = i + 1;
				break ;
			}
			i++;
		}
	}
}

int		*check_errors_of_args(char **args, int len, int i, int j)
{
	int		*valid_args;
	char	*name;

	if (!(valid_args = ft_calloc(len, sizeof(int))))
		cleanup(EXIT);
	while (i < len + 1)
	{
		j = 0;
		while (args[i][j] != '=' && args[i][j] != '\0')
			j++;
		if (!check_var_name(args[i], j))
		{
			g_bash_errno = E_BUILTIN;
			g_builtin_errno = EB_UNSET_EXPORT_NVI;
			g_bash_commandid = BC_EXPORT;
			STRCPY(g_bash_error, args[i] ? args[i] : "");
			g_program_return = 1;
			bash_error();
		}
		duplicated_variable(args[i], j);
		// if it doesn't have equal on it consider it as invalid
		if (!check_var_name(args[i], j) || args[i][j] == '\0')
			valid_args[i - 1] = 1;
		i++;
	}
	return (valid_args);
}

static	t_evar	ft_realloc(char *name, char *value)
{
	t_evar	tmp;
	int		name_len;
	int		value_len;

	if (name)
	{
		name_len = ft_strlen(name);
		value_len = ft_strlen(value);
		if (!(MALLOC(tmp.name, name_len + 1)))
			cleanup(EXIT);
		ft_strcpy(tmp.name, name);
		if (!(MALLOC(tmp.value, value_len + 1)))
			cleanup(EXIT);
		ft_strcpy(tmp.value, value);
		tmp.name_len = name_len;
		tmp.value_len = value_len;
	}
	else
	{
		tmp.name = NULL;
		(tmp.value = ft_strdup("")) ? 0 : cleanup(EXIT);
		tmp.name_len = -1;
		tmp.value_len = -1;
	}
	return (tmp);
}

char	*name_or_value(int sign, char *arg)
{
	char	*ret;
	int		i;
	int		equal;

	i = 0;
	equal = 0;
	ret = NULL;
	while (arg[equal] != '=')
		equal++;
	if (sign == 0)
	{
		if (!(MALLOC(ret, sizeof(char) * equal + 1)))
			cleanup(EXIT);
		ret = ft_strncpy(ret, arg, equal);
		ret[equal] = '\0';
	}
	else if (sign == 1)
	{
		equal += 1;
		if (!(MALLOC(ret, sizeof(char) * ft_strlen(arg + equal) + 1)))
			cleanup(EXIT);
		ft_strcpy(ret, arg + equal);
	}
	return (ret);
}

t_export	calc_lengths(int *valid, int len)
{
	int			i;
	t_export	res;

	i = 0;
	res.env_len = 0;
	res.new_var_len = 0;
	// count existed env variables
	while (g_line->env_var[res.env_len].name)
		res.env_len++;
	// add the valid args
	res.new_var_len = 0;
	i = 0;
	while (i < len)
		if (valid[i++] == 0)
			res.new_var_len++;
	// remove from the existing ones
	res.env_len -= g_dup ? 1 : 0;
	res.new_var_len += env_len;
	return (res);
}

void	export_new_vars(char **args, int len, int *valid)
{
	int		new_var_len;
	int		i;
	int		j;
	int		env_len;
	t_evar	*tmp;
	char	*name;
	char	*value;

	env_len = 0;
	// count existed env variables
	while (g_line->env_var[env_len].name)
		env_len++;
	// add the valid args
	new_var_len = 0;
	i = 0;
	while (i < len)
		if (valid[i++] == 0)
			new_var_len++;
	// remove from the existing ones
	env_len -= g_dup ? 1 : 0;
	new_var_len += env_len;
	if (!(tmp = malloc(sizeof(t_evar) * (new_var_len + 1))))
		cleanup(EXIT);
	// re-initialize i
	i = 0;
	j = 0;
	// fill tmp with the existing variables
	while (i < env_len)
	{
		if (g_dup == 0 || j != (g_dup - 1))
		{
			tmp[i] = ft_realloc(g_line->env_var[j].name, g_line->env_var[j].value);
			i++;
		}
		j++;
	}
	// fill the rest with args and terminate with NULL
	// re-initialize j
	j = 1;
	while (i < new_var_len)
	{
		name = name_or_value(0, args[j]);
		value = name_or_value(1, args[j]);
		tmp[i] = ft_realloc(name, value);
		free(name);
		free(value);
		i++;
		j++;
	}
	tmp[i] = ft_realloc(NULL, NULL);
	free_envar();
	g_line->env_var = tmp; // 52 lines
}

int		bc_export(t_cmd *data)
{
	int			args_len;
	int			*valid_args;
	t_export	lengths;

	g_dup = 0;
	args_len = count_args(data->args);
	valid_args = check_errors_of_args(data->args, args_len, 1, 0);
	lengths = calc_lengths(valid_args, args_len);
	export_new_vars(data->args, args_len, valid_args);
	free(valid_args);
	return (0);
}

/*
int		bc_export(t_cmd *data)
{
	t_evar	var;
	char	*tmp;

	var.name = data->args[1];
	var.value = data->args[1];
	tmp = ft_strchr(data->args[1], '=');
	if (!env_var_comp(var.name))
	{
		g_bash_errno = E_BUILTIN;
		g_builtin_errno = EB_UNSET_EXPORT_NVI;
		g_bash_commandid = BC_EXPORT;
		STRCPY(g_bash_error, data->args[1] ? data->args[1] : "");
		bash_error();
		//continue;
	}
	else if (tmp)
	{
		*tmp = '\0';
		var.value = tmp + 1;
		var.name_len = ft_strlen(var.name);
		var.value_len = ft_strlen(var.value);
		//var is ready

	}
	return (0);
}*/

int		bc_env(void)
{
	int		i;

	i = 0;
	while (g_line->env_var[i].name)
	{
		OPRINT(g_line->env_var[i].name);
		OPRINTS("=");
		OPRINT(g_line->env_var[i].value);
		OPRINTS("\n");
		i++;
	}
	return (0);
}

int		bc_echo(t_cmd *data)
{
	int		i;
	int		pn;

	pn = 1;
	i = 1;
	if (data->args[i] && !CMP(data->args[i], "-n"))
	{
		pn = 0;
		i++;
	}
	while (data->args[i] && data->args[i + 1])
	{
		OPRINT(data->args[i]);
		i++;
		OPRINTS(" ");
	}
	if (data->args[i])
		OPRINT(data->args[i]);
	if (pn)
		OPRINT("\n");
	return (0);
}

int		bc_pwd(t_cmd *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (-1);
	OPRINT(cwd);
	OPRINTS("\n");
	free(cwd);
	return (0);
}
