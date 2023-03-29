cmake_minimum_required (VERSION 3.8)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CPP_STANDARD 14)

project ("ex6")

enable_testing()

add_executable(ex6 main.cpp )

add_executable(presubmit srcs/presubmit.cpp)
add_executable(test1 tests/test_ex6.cpp)
add_executable(test2 tests/tests_ex6_suchetzky.cpp)

add_executable(try try.cpp )
						  
file(COPY io/ DESTINATION io/)

add_test(NAME presubmit COMMAND presubmit)
add_test(NAME test1 COMMAND test1)
add_test(NAME test2 COMMAND test2)

