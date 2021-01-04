#include <stdlib.h>
#include <stdio.h>
#include "alloc.h"

void	*malloc_wraper_c(size_t i, int line, const char *func, const char *file)
{
	printf("called from function: \"%s\" on line %d in file: \"%s\"\n", func, line, file);
	#if defined(FUNCTION) && defined(FOL) && defined(FILE_TO_FAIL_ON)
	if (!strcmp(func, FUNCTION) && line == FOL && !strcmp(file, FILE_TO_FAIL_ON))
	{
		printf("failing on %dth call...\n");
		return (NULL);
	}
	#endif
	return (malloc(i));
}
