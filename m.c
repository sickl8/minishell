/*
void			*malloc(size_t len);
void			free(void* adr);
int				write(int fd, void *buf, size_t len);
int				read(int fd, void *buf, size_t len);
int				open(char *path, int flags, ...);
int				close(int fd);
pid_t			fork(void);
pid_t			wait(int *wstatus);
pid_t			waitpid(pid_t pid, int *wstatus, int options);
pid_t			wait3(int *wstatus, int options, struct rusage *rusage);
pid_t			wait4(pid_t pid, int *wstatus, int options, struct rusage *rusage);
sighandler_t	signal(int signum, sighandler_t handler);
int				kill(pid_t pid, int sig);
void			exit(int status);
char			*getcwd(char *buf, size_t size);
int				chdir(const char *path);
int				stat(const char *pathname, struct stat *statbuf);
int				lstat(const char *pathname, struct stat *statbuf);
int				fstat(int fd, struct stat *statbuf);
int				execve(const char *pathname, char *const argv[], char *const envp[]);
int				dup(int oldfd);
int				dup2(int oldfd, int newfd);
int				pipe(int pipefd[2]);
DIR				*opendir(const char *name);
struct dirent	*readdir(DIR *dirp);
int				closedir(DIR *dirp);
char			*strerror(int errnum);
int				errno;
*/

#include <sys/resource.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#include <stdio.h>

#include "typedef.h"
#include "def.h"
#include "proto.h"

#define ARG_MAX 2097152

#define P(x) write(STDOUT_FILENO, x, strlen(x))

#define LITERAL 'L'
#define SEMILIT 'S'
#define NONLIT 'N'
#define BASHSYN 'B'
#define ENVVAR 'V'
#define WHTSPC 'W'

#define ENV_NAME_LEN_MAX 32760

t_evar	find_env(t_line line, size_t *ref)
{
	t_evar	ret;
	size_t	i;
	size_t	j;
	char	env_space[ENV_NAME_LEN_MAX];

	(*ref)++;
	i = *ref;
	j = 0;
	while (line.rd_buf[i] && line.mask[i] == ENVVAR &&
	i - *ref < ENV_NAME_LEN_MAX)
		env_space[j++] = line.rd_buf[i++];
	env_space[j] = '\0';
	j = 0;
	while (line.env_var[j].name)
	{
		if (!strcmp(line.env_var[j].name, env_space))
			break;
		j++;
	}
	ret = line.env_var[j];
	if (!ret.name)
		ret.name_len = strlen(env_space);
	i--;
	*ref = i;
	return (ret);
}

t_line	rplc_env_var(t_line line)
{
	size_t	tlen;
	t_evar	env;
	size_t	i;
	size_t	j;

	i = -1;
	tlen = 0;
	while (line.rd_buf[++i])
	{
		if (line.mask[i] == '$')
			tlen += find_env(line, &i).value_len;
		else
			tlen++;
	}
	MALLOC(line.buf, tlen + 1);
	MALLOC(line.msk, tlen + 2);
	line.buf[tlen] = 0;
	line.msk[tlen] = 0;
	line.msk[tlen + 1] = 0;
	// memset(line.buf, 0, tlen + 1);
	// memset(line.msk, 0, tlen + 2);
	i = -1;
	j = 0;
	while (line.rd_buf[++i])
	{
		if (line.mask[i] == '$')
		{
			env = find_env(line, &i);
			strncpy(&line.buf[j], env.value, env.value_len);
			memset(&line.msk[j], LITERAL, env.value_len);
			j += env.value_len;
		}
		else
		{
			line.buf[j] = line.rd_buf[i];
			line.msk[j] = line.mask[i];
			j++;
		}
	}
	return (line);
}

int		is_ws(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' ||
	c == '\v' || c == '\f' || c == '\r' || c == '\0')
		return (1);
	return (0);
}

int		env_var_comp(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || c == '_')
		return (1);
	return (0);
}

void	env_var(t_line line, size_t *ref)
{
	size_t	i;

	i = *ref;
	line.mask[i] = '$';
	if (line.rd_buf[++i] && env_var_comp(line.rd_buf[i]) &&
	!(line.rd_buf[i] >= '0' && line.rd_buf[i] <= '9'))
	{
		*ref = i;
		line.mask[i] = ENVVAR;
		while (line.rd_buf[++i] && env_var_comp(line.rd_buf[i]) &&
		i - *ref < ENV_NAME_LEN_MAX)
			line.mask[i] = ENVVAR;
	}
	i--;
	*ref = i;
}

void	squote(t_line line, size_t *ref)
{
	size_t	i;

	i = *ref;
	line.mask[i] = '\'';
	while (line.rd_buf[++i] && line.rd_buf[i] != '\'')
		line.mask[i] = LITERAL;
	if (line.rd_buf[i])
		line.mask[i] = '\'';
	else
		i--;
	*ref = i;
}

void	dquote(t_line line, size_t *ref)
{
	size_t	i;

	i = *ref;
	line.mask[i] = '"';
	while (line.rd_buf[++i] && line.rd_buf[i] != '"')
	{
		if (line.rd_buf[i] == '\\')
			backslash(line, &i);
		else if (line.rd_buf[i] == '$')
			env_var(line, &i);
		else
			line.mask[i] = SEMILIT;
	}
	if (line.rd_buf[i])
		line.mask[i] = '"';
	else
		i--;
	*ref = i;
}

void	backslash(t_line line, size_t *ref)
{
	size_t	i;

	i = *ref;
	line.mask[i] = '\\';
	if (line.rd_buf[i + 1])
		line.mask[++i] = LITERAL;
	*ref = i;
}

void	set_mask(t_line line)
{
	size_t	i;

	i = -1;
	while (line.rd_buf[++i])
	{
		if (line.rd_buf[i] == '\\')
			backslash(line, &i);
		else if (line.rd_buf[i] == '"')
			dquote(line, &i);
		else if (line.rd_buf[i] == '\'')
			squote(line, &i);
		else if (line.rd_buf[i] == '|' || line.rd_buf[i] == '>' ||
		line.rd_buf[i] == '<' || line.rd_buf[i] == ';')
		{
			line.mask[i] = BASHSYN;
			if (line.rd_buf[i] == '>' && line.rd_buf[i + 1] == '>')
				line.mask[++i] = BASHSYN;
		}
		else if (line.rd_buf[i] == '$')
			env_var(line, &i);
		else if (is_ws(line.rd_buf[i]))
			line.mask[i] = WHTSPC;
		else
			line.mask[i] = NONLIT;
	}
}

t_line	format_string(t_line line)
{
	set_mask(line);
	line = rplc_env_var(line);
	
	return (line);
}

// char	*ft_strncpy(char *d, char *s, size_t len)
// {
// 	size_t	i;

// 	i = 0;
// 	while (s[i] && i < len)
// 	{
// 		d[i] = s[i];
// 		i++;
// 	}
// 	return (d);
// }

char	*ft_strchr(char *s, char c)
{
	while (*s && *s != c)
		s++;
	if (*s == c)
		return (s);
	return (NULL);
}

void	init_env(t_line *ref)
{
	int		i;
	t_line	line;

	line = *ref;
	i = 0;
	while (line.envp[i])
		i++;
	MALLOC(line.env_var, i + 1);
	line.env_var[i].name = NULL;
	line.env_var[i].name_len = 0;
	line.env_var[i].value_len = 0;
	MALLOC(line.env_var[i].value, 1);
	line.env_var[i].value[0] = '\0';
	i = -1;
	while (line.envp[++i])
	{
		line.env_var[i].name_len = ft_strchr(line.envp[i], '=') - line.envp[i];
		MALLOC(line.env_var[i].name, line.env_var[i].name_len + 1);
		line.env_var[i].name[line.env_var[i].name_len] = '\0';
		strncpy(line.env_var[i].name, line.envp[i], line.env_var[i].name_len);
		line.env_var[i].value = ft_strchr(line.envp[i], '=') + 1;
		line.env_var[i].value_len = strlen(line.env_var[i].value);
	}
	*ref = line;
}

int		main(int ac, char **av, char **envp)
{
	t_line line;

	MALLOC(line.rd_buf, ARG_MAX + 2);
	MALLOC(line.mask, ARG_MAX + 3);
	line.envp = envp;
	init_env(&line);
	// for (int i = 0; line.env_var[i].name; i++)
	// {
	// 	printf("name = %s\n", line.env_var[i].name);
	// 	printf("len = %zu\n", line.env_var[i].len);
	// 	printf("value = %s\n", line.env_var[i].value);
	// }
	while (1)
	{
		P("\033[1;31mminishell: \033[0m");
		memset(line.rd_buf, 0, ARG_MAX + 2);
		memset(line.mask, 0, ARG_MAX + 3);
		line.rd_ret = read(STDIN_FILENO, line.rd_buf, ARG_MAX + 1);
		if (line.rd_ret > 0)
			line.rd_buf[line.rd_ret - 1] = '\0';
		line.rd_len = strlen(line.rd_buf);
		line = format_string(line);
		printf("str = |%s|\n", line.rd_buf);
		printf("msk = |%s|\n", line.mask);
		printf("env = |%s|\n", line.buf);
		printf("msk = |%s|\n", line.msk);
	}
}