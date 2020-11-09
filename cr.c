#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int ac, char **av)
{
	int fd = 123;
	if (ac > 1)
		fd = open(av[1], O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	printf("fd = %d, errno = %d, strerr = %s\n", fd, errno, strerror(errno));
}
