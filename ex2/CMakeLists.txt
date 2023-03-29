cmake_minimum_required (VERSION 3.8)

#set(CMAKE_C_STANDARD 99)

project ("ex2")

# Create tests
enable_testing()
add_compile_definitions(OH_TESTS)
add_executable(oh_tests "oh_tests/oh_tests.c"
						)

add_executable(try ProductBST.c
				   "oh_tests/oh_tests_helpers.c"
				   try.c)

configure_file("oh_tests/products.txt" products.txt COPYONLY)
configure_file("oh_tests/products_s.txt" products_s.txt COPYONLY)
configure_file("oh_tests/products_s_ex.txt" products_s_ex.txt COPYONLY)
configure_file("oh_tests/products_s_zn.txt" products_s_zn.txt COPYONLY)
configure_file("oh_tests/products_s_zn_ex.txt" products_s_zn_ex.txt COPYONLY)
configure_file("oh_tests/products_s_b.txt" products_s_b.txt COPYONLY)
configure_file("oh_tests/delete_ex.txt" delete_ex.txt COPYONLY)

# Add test
add_test(NAME test1 COMMAND oh_tests 1)
add_test(NAME test2 COMMAND oh_tests 2)
add_test(NAME test3 COMMAND oh_tests 3)
add_test(NAME test4 COMMAND oh_tests 4)
add_test(NAME test5 COMMAND oh_tests 5)
add_test(NAME test6 COMMAND oh_tests 6)
add_test(NAME test7 COMMAND oh_tests 7)
add_test(NAME test8 COMMAND oh_tests 8)
add_test(NAME test9 COMMAND oh_tests 9)
add_test(NAME test10 COMMAND oh_tests 10)
add_test(NAME test11 COMMAND oh_tests 11)
add_test(NAME test12 COMMAND oh_tests 12)
add_test(NAME test13 COMMAND oh_tests 13)
add_test(NAME test14 COMMAND oh_tests 14)
add_test(NAME test15 COMMAND oh_tests 15)
add_test(NAME test16 COMMAND oh_tests 16)
add_test(NAME test17 COMMAND oh_tests 17)
add_test(NAME test18 COMMAND oh_tests 18)
add_test(NAME test19 COMMAND oh_tests 19)
add_test(NAME test20 COMMAND oh_tests 20)
add_test(NAME test21 COMMAND oh_tests 21)
add_test(NAME test22 COMMAND oh_tests 22)
add_test(NAME test23 COMMAND oh_tests 23)
add_test(NAME test24 COMMAND oh_tests 24)
add_test(NAME test25 COMMAND oh_tests 25)
add_test(NAME test26 COMMAND oh_tests 26)
