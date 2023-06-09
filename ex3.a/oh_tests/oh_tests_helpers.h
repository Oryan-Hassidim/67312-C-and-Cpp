#ifndef OH_TESTS_HELPERS
#define OH_TESTS_HELPERS

#include <stdlib.h>
#include <stdio.h>

void* test_malloc(size_t _Size);
void* test_calloc(size_t _Count, size_t _Size);
void test_free(void* _Block);

void set_allocations(size_t n);
size_t get_allocations();

FILE* get_test_stderr();
void set_test_stderr(FILE* f);

#ifndef OH_TESTS_HELPERS_C
#undef stderr
#define malloc test_malloc
#define calloc test_calloc
#define free test_free
#define stderr get_test_stderr()
#endif // !OH_TESTS_HELPERS_C
#endif