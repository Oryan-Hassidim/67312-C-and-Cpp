#define OH_TESTS_HELPERS_C
#include "oh_tests_helpers.h"
#undef OH_TESTS_HELPERS_C
#include <stdlib.h>
#include <stdio.h>


size_t allocations = 0;
void* test_malloc(size_t _Size)
{
#ifdef LIMIT_ALLOCS
	if (allocations >= LIMIT_ALLOCS)
	{
		return NULL;
	}
#endif // LIMIT_ALLOCS
	void* allocated = malloc(_Size);
	if (allocated != NULL)
	{
		allocations++;
	}
	return allocated;
}
void* test_calloc(size_t _Count, size_t _Size)
{
#ifdef LIMIT_ALLOCS
	if (allocations >= LIMIT_ALLOCS)
	{
		return NULL;
	}
#endif // LIMIT_ALLOCS
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
void set_allocations(size_t n)
{
	allocations = n;
}

size_t get_allocations()
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
