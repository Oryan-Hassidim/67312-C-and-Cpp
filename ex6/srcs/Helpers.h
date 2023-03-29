#ifndef HELPERS_H
#define HELPERS_H

/************************************************************
 * Helper macros
 ************************************************************/
#define __STR(x) #x
#define STR(x) __STR(x)

/************************************************************
 * Main UT macros
 *************************************************************/

/**
 * Asserts that a tests passes.
 * @param package The package that this test belongs to.
 * @param testName a name of a "int (void)" function that should execute the test.
 *
 * MACRO EXPANSION:
 * try {
 *      if (!testName()) {
 *          print error
 *          return 0;
 *      }
 *      print test passed
 * } catch (exception) {
 *      print test failed
 *      return 0;
 * }
 */
#define __ASSERT_TEST(package, testName) try { \
        if (!testName()) { \
        std::cerr << "[" package "] Test "#testName"... FAILED! Abort." << std::endl; \
        return 0; \
        } \
        std::cout << "[#" STR(__COUNTER__) "][" package "] Test "#testName"... OK!" << std::endl; \
    } catch (std::exception& __e) { \
        std::cerr << "[" package "] Test "#testName"... FAILED! Unexpected exception was thrown: " << __e.what() << ". Abort." << std::endl; \
        return 0; \
    }

/**
 * Asserts that the given condition is true.
 * @param condition A C++ expression to evaluate.
 *
 * MACRO EXPANSION:
 * if (!condition) {
 *      print error
 *      return 0;
 * }
 */
#define ASSERT_TRUE(condition) if (!(condition)) { \
        std::cerr << "Assertion failed: Unsatisifed constraint: "#condition << std::endl; \
        return 0; \
    }

/**
* Asserts that the given condition is true and return the evaluation result.
* @param condition A C++ expression to evaluate.
*
* MACRO EXPANSION:
* if (!condition) {
*      print error
*      return 0;
* } else {
 *      return 1;
 * }
*/
#define RETURN_ASSERT_TRUE(condition) ASSERT_TRUE(condition); \
    return 1;

    /**
     * Asserts that the given load factor matches our map value, +- some epsilon.
     * @param map The map to check.
     * @param value The load factor expected value.
     *
     * NOTE: epsilon = 0.001
     */
#define ASSERT_VALID_LOAD_FACTOR(map, value) ASSERT_TRUE(map.get_load_factor() >= (value - 0.001) && map.get_load_factor() <= (value + 0.001));

/**
 * Asserts that the entire map properties match our expectations.
 * @param map The map.
 * @param expectedLoadFactor The expected load factor.
 * @param expectedCapacity The expected capacity.
 * @param expectedSize The expected size.
 */
#define ASSERT_MAP_PROPERTIES(map, expectedLoadFactor, expectedCapacity, expectedSize) \
    ASSERT_VALID_LOAD_FACTOR(map, expectedLoadFactor); \
    ASSERT_TRUE(map.capacity() == expectedCapacity); \
    ASSERT_TRUE(map.size() == expectedSize); \
    ASSERT_TRUE((expectedSize == 0) ? (map.empty()) : (!map.empty()));

/**
 * Asserts that the given code scope doesn't throw any exception
 * @param scope The code scope.
 *
 * MACRO EXPANSION:
 * try {
 *      code scope
 * } catch (exception) {
 *      print error
 *      return 0;
 * }
 *
 * NOTE:
 * To use ASSERT_NOT_THROWING and ASSERT_THROWING you need to use the following syntax
 * ASSERT_NOT_THROWING(
 *      stmt1
 *      stmt2
 *      ...
 * );
 * Why? because macros interpret commas in the arguments we send as macro expansion
 * macros, so sending "std::map<int, int>" will considered to be "std::map<int" arg1 and "int>" arg2.
 * See: https://stackoverflow.com/a/23292958/1497516
 */
#define ASSERT_NOT_THROWING(...) { \
    try \
    { \
        __VA_ARGS__ \
    } catch (std::exception& e) { \
        std::cerr << "Assertion failed: Unexpected exception was thrown. Exception: " << e.what() << std::endl; \
        return 0; \
    } \
    }
/**
 * Asserts that the given code scope throws an exception.
 * @param scope The code scope.
 *
 * MACRO EXPANSION:
 * try {
 *      code scope
 *      print error
 *      return 0;
 * } catch (exception) { }
 */
#define ASSERT_THROWING(...) { \
    try \
    { \
        __VA_ARGS__ \
        std::cerr << "Assertion failed: Expected exception, but no exception was thrown." << std::endl; \
        return 0; \
     } catch (std::exception& e) { \
     } \
     }


#endif //HELPERS_H
