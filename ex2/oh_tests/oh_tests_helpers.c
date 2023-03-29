#include <stdlib.h>
#include <stdio.h>

#include "oh_tests_helpers.h"

int allocations = 0;
void* test_malloc(size_t _Size)
{
	void* allocated = malloc(_Size);
	if (allocated != NULL)
	{
		allocations++;
	}
	return allocated;
}
void* test_calloc(size_t _Count, size_t _Size)
{
	void* allocated = calloc(_Count, _Size);
	if (allocated != NULL)
	{
		allocations++;
	}
	return allocated;
}
void test_free(void* _Block)
{
	allocations--;
	free(_Block);
}
void set_allocations(int n)
{
	allocations = n;
}
int get_allocations()
{
	return allocations;
}

FILE* test_stderr = NULL;
FILE* get_test_stderr()
{
	return test_stderr == NULL ? stderr : test_stderr;
}

void set_test_stderr(FILE* f)
{
	test_stderr = f;
}
