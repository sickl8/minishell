#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int		main()
{
	char str[1000];
	int i = read(0, str, 999);
	printf("str = %s\n", str);
}
