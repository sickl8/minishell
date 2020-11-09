#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int ac, char **av, char **envp)
{
	char *v[] = (char*[]){ NULL };
	char *p[] = (char*[]){ NULL };
	printf("executing..\n");
	int i = execve("mybash", v, p);
	printf("executed\n");
	printf("errno = %d, strerr = %s\n", errno, strerror(errno));
}
