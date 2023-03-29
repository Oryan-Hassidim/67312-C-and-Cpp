cmake_minimum_required (VERSION 3.8)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CPP_STANDARD 14)

project ("ex5")

enable_testing()

add_executable(ex5 print_main.cpp
				   RSUser.cpp
				   RSUsersLoader.cpp
				   Movie.cpp
				   RecommenderSystem.cpp
				   RecommenderSystemLoader.cpp)

add_executable(presubmit presubmit.cpp
				         RSUser.cpp
				         RSUsersLoader.cpp
				         Movie.cpp
				         RecommenderSystem.cpp
				         RecommenderSystemLoader.cpp)


add_executable(try try.cpp)
						  
file(COPY RSUsersLoader_input.txt DESTINATION .)
file(COPY RecommenderSystemLoader_input.txt DESTINATION .)
file(COPY presubmit.in_m DESTINATION .)
file(COPY presubmit.in_u DESTINATION .)

