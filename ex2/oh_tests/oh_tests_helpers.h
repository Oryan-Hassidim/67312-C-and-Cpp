#ifndef OH_TESTS_HELPERS
#define OH_TESTS_HELPERS

#include <stdlib.h>
#include <stdio.h>

void* test_malloc(size_t _Size);
void* test_calloc(size_t _Count, size_t _Size);
void test_free(void* _Block);

void set_allocations(int n);
int get_allocations();

FILE* get_test_stderr();
void set_test_stderr(FILE* f);

#endif