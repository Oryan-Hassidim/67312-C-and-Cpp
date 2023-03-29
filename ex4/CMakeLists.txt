cmake_minimum_required (VERSION 3.8)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CPP_STANDARD 14)

project ("ex4")

enable_testing()

add_executable(mlpnetwork main.cpp
				          MlpNetwork.cpp
				          Activation.cpp
				          Dense.cpp
				          Matrix.cpp)
						  
add_executable(presubmit presubmit.cpp
				         MlpNetwork.cpp
				         Activation.cpp
				         Dense.cpp
				         Matrix.cpp)
enable_testing()
						 
add_executable(oh_tests_const oh_tests_const.cpp
				              MlpNetwork.cpp
				              Activation.cpp
				              Dense.cpp
				              Matrix.cpp)
enable_testing()

add_executable(oh_tests_exceptions oh_tests_exceptions.cpp
					               Matrix.cpp
					               MlpNetwork.cpp
				                   Activation.cpp
				                   Dense.cpp)
# add tests
add_test(NAME presubmit COMMAND presubmit)
add_test(NAME oh_tests_const COMMAND oh_tests_const)
add_test(NAME oh_tests_exceptions_test_matrix_ctor_negative_or_zero_dimesions COMMAND oh_tests_exceptions 1)
add_test(NAME oh_tests_exceptions_test_matrix_brackets_index COMMAND oh_tests_exceptions 2)
add_test(NAME oh_tests_exceptions_test_matrix_arithmetics COMMAND oh_tests_exceptions 3)
add_test(NAME oh_tests_exceptions_test_matrix_loading_from_stream COMMAND oh_tests_exceptions 4)
add_test(NAME oh_tests_exceptions_test_dense_ctor_bias_non_vector COMMAND oh_tests_exceptions 5)
add_test(NAME oh_tests_exceptions_test_dense_ctor_different_rows COMMAND oh_tests_exceptions 6)
add_test(NAME oh_tests_exceptions_test_mlp_ctor_different_dimensions COMMAND oh_tests_exceptions 7)
add_test(NAME oh_tests_exceptions_test_mlp_ctor_non_4_arrays COMMAND oh_tests_exceptions 8)

file(COPY parameters/ DESTINATION parameters/)
file(COPY io/ DESTINATION .)
file(COPY images/ DESTINATION images/)

