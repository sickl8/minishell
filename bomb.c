#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int ac, char **av)
{
	int fd = open("/Users/isaadi/Desktop/cursus/minishell/bombfile", O_CREAT | O_APPEND | O_RDWR, S_IRWXU);
	write(fd, "success\n", 8);
	//printf("fd = %d\n", fd);
	int i = 1;
	while (i < ac && av[i])
		printf("%s ", av[i++]);
	printf("\n");
}
