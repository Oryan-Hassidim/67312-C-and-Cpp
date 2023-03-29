#ifdef OH_TESTS
#include "oh_tests/oh_tests_helpers.h"
#define malloc test_malloc
#define calloc test_calloc
#define free test_free
#define stderr get_test_stderr()
#endif // OH_TESTS
