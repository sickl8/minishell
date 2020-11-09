#include <stdlib.h>
#include <stdio.h>
typedef struct sss
{
	char *s;
	struct sss *next;
}				lst;

int main(int ac, char **av)
{
	int i = 0;
	lst *t = NULL, *h;
	ac--;
	while (i < ac)
	{
		h = malloc(sizeof(*t));
		h->s = av[i + 1];
		h->next = t;
		t = h;
		i++;
	}
	while (h)
	{
		printf("%s->", h->s);
		h = h->next;
	}
	printf("\n");
}
