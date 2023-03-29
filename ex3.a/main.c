#include <stdlib.h>
#include <stdio.h>

void f2(int p2[][])
{
	printf("%lu %lu %lu", sizeof(p2), sizeof(*p2), sizeof(**p2));
}

int main()
{
	int x = 1;
	int* const i = &x;
	*i = 1;
	i = NULL;
}