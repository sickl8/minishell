#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <stdlib.h>
#define TIME(x) b = clock(); x printf("The section : \"%.20s\"[...] took %lfs = %lfms\n", #x, (double)(clock() - b) / CLOCKS_PER_SEC, ((double)(clock() - b) / CLOCKS_PER_SEC) * 1000)
#define LEN 0xffffffff
clock_t b;

int main()
{
	return (-1);
}
