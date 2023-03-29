cmake_minimum_required (VERSION 3.8)

project ("ex1")

add_executable(cipher cipher.h cipher.c
                      tests.h tests.c
                      main.c)
