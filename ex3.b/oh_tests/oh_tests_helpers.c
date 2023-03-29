#define OH_TESTS_HELPERS_C
#include "oh_tests_helpers.h"
#undef OH_TESTS_HELPERS_C
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#ifdef LIMIT_ALLOCS
size_t limit_allocs = LIMIT_ALLOCS;
#else
size_t limit_allocs = ULLONG_MAX;
#endif // LIMIT_ALLOCS


size_t allocations = 0;
void set_limit_allocs(size_t n)
{
	limit_allocs = n;
}
void* test_malloc(size_t _Size)
{
	if (allocations >= limit_allocs)
	{
		return NULL;
	}
	void* allocated = malloc(_Size);
	if (allocated != NULL)
	{
		allocations++;
	}
	return allocated;
}
void* test_calloc(size_t _Count, size_t _Size)
{
	if (allocations >= limit_allocs)
	{
		return NULL;
	}
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
void* test_realloc(void* _Block, size_t _Size)
{
	if (allocations >= limit_allocs)
	{
		return NULL;
	}
	void* allocated = realloc(_Block, _Size);
	if (_Block == NULL && allocated != NULL)
	{
		allocations++;
	}
	return allocated;
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
