cmake_minimum_required (VERSION 3.8)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CPP_STANDARD 14)

project ("ex3.b")

add_executable(ex3 main.c)

add_compile_definitions(OH_TESTS)
add_executable(tweets_generator tweets_generator.c
								markov_chain.c
								linked_list.c
								"oh_tests/oh_tests_helpers.c")

add_executable(snakes_and_ladders snakes_and_ladders.c
								  markov_chain.c
								  linked_list.c
								  "oh_tests/oh_tests_helpers.c")

add_executable(new_generic_type "_new_generic_type.c"
								markov_chain.c
								linked_list.c
								"oh_tests/oh_tests_helpers.c")
								  
add_executable(orel orel.cpp)

configure_file("justdoit_tweets.txt" "justdoit_tweets.txt" COPYONLY)