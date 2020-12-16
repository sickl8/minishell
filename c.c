#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void	print(void)
{
	fprintf(stderr, "caught signal\n");
	fprintf(stderr, "exiting\n");
}

void	call(int sig)
{
	exit(0);
}

int		main()
{
	signal(SIGINT, call);
	atexit(&print);
	while (1) { }
}