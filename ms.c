#include <unistd.h>
#include <stdio.h>

int main()
{
	char str[1000];

	long i = read(0, str, 999);
	printf("read = %ld \"%s\"\n", i, str);
}
