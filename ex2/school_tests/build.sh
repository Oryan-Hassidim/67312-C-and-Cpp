#!/bin/bash

gcc -c ../oh_tests/oh_tests.c -o oh_tests.o
gcc oh_tests_helpers.o oh_tests.o ProductBST.o -o school