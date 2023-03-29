cmake_minimum_required (VERSION 3.8)

set(CMAKE_C_STANDARD 99)

project ("ex3.a")

add_compile_definitions(OH_TESTS)
add_executable(ex3 main.c)

add_executable(tweets_generator tweets_generator.c
								markov_chain.c
								linked_list.c
								"oh_tests/oh_tests_helpers.c")

configure_file("justdoit_tweets.txt" "justdoit_tweets.txt" COPYONLY)