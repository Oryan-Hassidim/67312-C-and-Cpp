####################################################################
# Oryan Hassidim
# Oryan.Hassidim@mail.huji.ac.il
# last update: 22/05/2022  21:38
####################################################################


from random import randint, choice
import sys
from subprocess import run

INT_MAX = 2147483647
school_solution = [
    "/cs/course/current/labcc2/school_solution/ex3b/schoolSolution"]
valgrind = ["valgrind", "--leak-check=full"]
tweets_file = "/cs/usr/oryan.hassidim/ex3b/ex3b-oryan.hassidim/justdoit_tweets_s.txt"


class Output:
    def __init__(self, args):
        if isinstance(args, str):
            args = args.split(" ")
        completed_proccess = run(map(str, args), capture_output=True)
        self.stdout = completed_proccess.stdout.decode('utf-8')
        self.stderr = completed_proccess.stderr.decode('utf-8')
        self.exit_code = completed_proccess.returncode

    def __eq__(self, other):
        return isinstance(other, Output) and self.exit_code == other.exit_code and self.stderr == other.stderr and self.stdout == other.stdout

    def __str__(self):
        return f'{{ exit_code: {self.exit_code}\n  stdout:\n"{self.stdout}"\n  stderr:\n"{self.stderr}"\n}}'


def file_contains(file, *s):
    with open(file, 'r') as f:
        content = f.read()
        return all(i in content for i in s)


def rand_num(max):
    return str(randint(1, max))


def test_valgrind(args):
    args = valgrind + args
    output = Output(args)
    return "0 errors from 0 contexts" in output.stderr, output.stderr

def get_dots():
    with open("/cs/usr/oryan.hassidim/ex3b/ex3b-oryan.hassidim/oh_tests/dots_s.txt", 'r') as f:
        content = f.read()
        return list(content.split(' '))

def test_tweets_generator():
    Output("rm *tweets_generator")
    build = Output("make tweets")
    if build.exit_code != 0:
        print("Error occurred when call make command!")
        print(f"command: make tweets")
        print(build.stdout)
        print(build.stderr)
        return False
    dots = get_dots()
    for i in range(4):
        args = ["tweets_generator",
                rand_num(INT_MAX),
                rand_num(50),
                tweets_file]
        got = Output(args)
        expected = Output(school_solution + args)
        if got != expected:
            print("args: ", end="")
            print(*args)
            print(
                f"Got is different from expected!\nGot:\n{got}\nExpected:\n{expected}\n\n")
            return False
        # check valgrind
        s, o = test_valgrind(args)
        if not s:
            print("valgrind error!\n")
            print("args: ", end="")
            print(*args)
            print(o)
            return False
    for i in range(4):
        args = ["tweets_generator",
                rand_num(INT_MAX),
                rand_num(50),
                tweets_file,
                choice(dots)]
        got = Output(args)
        expected = Output(school_solution + args)
        if got != expected:
            print("args: ", end="")
            print(*args)
            print(
                f"Got is different from expected!\nGot:\n{got}\nExpected:\n{expected}\n\n")
            return False
        # check valgrind
        s, o = test_valgrind(args)
        if not s:
            print("valgrind error!\n")
            print("args: ", end="")
            print(*args)
            print(o)
            return False
    return True


def test_conitues_tweets_generator():
    Output("rm conitues_tweets_generator")
    Output("rm conitues_tweets_generator.c")
    copy = Output("cp /cs/usr/oryan.hassidim/ex3b/ex3b-oryan.hassidim/_conitues_tweets_generator.c conitues_tweets_generator.c")
    if copy.exit_code != 0:
        print("failed to copy conitues_tweets_generator.c")
        print(copy.stdout)
        return False
    build = Output("gcc -std=c99 -Wextra -Wvla -g conitues_tweets_generator.c markov_chain.c linked_list.c -o conitues_tweets_generator")
    if build.exit_code != 0:
        print("Error occurred when try to build test!")
        print(build.stdout)
        print(build.stderr)
        Output("rm conitues_tweets_generator.c")
        return False
    dots = get_dots()
    for i in range(4):
        args = [rand_num(INT_MAX), rand_num(50), tweets_file]
        got = Output(["conitues_tweets_generator"] + args)
        expected = Output(school_solution + ["tweets_generator"] + args)
        if got != expected:
            print("args: ", end="")
            print(*args)
            print(
                f"Got is different from expected!\nGot:\n{got}\nExpected:\n{expected}\n\n")
            Output("rm conitues_tweets_generator")
            Output("rm conitues_tweets_generator.c")
            return False
        # check valgrind
        s, o = test_valgrind(["conitues_tweets_generator"] + args)
        if not s:
            print("valgrind error!\n")
            print("args: ", end="")
            print(*args)
            print(o)
            Output("rm conitues_tweets_generator.c")
            Output("rm conitues_tweets_generator")
            return False
    for i in range(4):
        args = [rand_num(INT_MAX), rand_num(50), 
                tweets_file, choice(dots)]
        got = Output(["conitues_tweets_generator"] + args)
        expected = Output(school_solution + ["tweets_generator"] + args)
        if got != expected:
            print("args: ", end="")
            print(*args)
            print(
                f"Got is different from expected!\nGot:\n{got}\nExpected:\n{expected}\n\n")
            Output("rm conitues_tweets_generator")
            Output("rm conitues_tweets_generator.c")
            return False
        # check valgrind
        s, o = test_valgrind(["conitues_tweets_generator"] + args)
        if not s:
            print("valgrind error!\n")
            print("args: ", end="")
            print(*args)
            print(o)
            Output("rm conitues_tweets_generator")
            Output("rm conitues_tweets_generator.c")
            return False
    Output("rm conitues_tweets_generator")
    Output("rm conitues_tweets_generator.c")
    return True


def test_snakes_and_ladders():
    Output("rm *snakes_and_ladders")
    build = Output("make snake")
    if build.exit_code != 0:
        print("Error occurred when call make command!")
        print(f"command: make snake")
        print(build.stdout)
        print(build.stderr)
        return False
    for i in range(50):
        args = [rand_num(INT_MAX), rand_num(5)]
        got = Output(["snakes_and_ladders"] + args)
        expected = Output(school_solution + ["snakes"] + args)
        if got != expected:
            print("args: ", end="")
            print(*(["snakes_and_ladders"] + args))
            print(
                f"Got is different from expected!\nGot:\n{got}\nExpected:\n{expected}\n\n")
            return False
        # check valgrind
        s, o = test_valgrind(["snakes_and_ladders"] + args)
        if not s:
            print("valgrind error!\n")
            print("args: ", end="")
            print(*(["snakes_and_ladders"] + args))
            print(o)
            return False
    return True


def test_new_generic_type():
    Output("rm new_generic_type")
    Output("rm new_generic_type.c")
    include_strings = ["#ifdef OH_TESTS", '#include "/cs/usr/oryan.hassidim/ex3b/ex3b-oryan.hassidim/oh_tests/oh_tests_helpers.h"']
    if not file_contains("markov_chain.c", *include_strings):
        print("You didn't add the include in the right place! file: markov_chain.c")
        return False
    if not file_contains("linked_list.c", *include_strings):
        print("You didn't add the include in the right place! file: linked_list.c")
        return False
    copy = Output(
        "cp /cs/usr/oryan.hassidim/ex3b/ex3b-oryan.hassidim/_new_generic_type.c new_generic_type.c")
    if copy.exit_code != 0:
        print("failed to copy new_generic_type.c")
        print(copy.stdout)
        return False
    build = Output("gcc -std=c99 -Wextra -Wvla -D OH_TESTS -g new_generic_type.c /cs/usr/oryan.hassidim/ex3b/ex3b-oryan.hassidim/oh_tests/oh_tests_helpers.c markov_chain.c linked_list.c -o new_generic_type")
    if build.exit_code != 0:
        print("Error occurred when try to build test!")
        print(build.stdout)
        print(build.stderr)
        Output("rm new_generic_type.c")
        return False

    
    for i in range(50):
        args = [rand_num(INT_MAX), rand_num(50)]
        got = Output(["new_generic_type"]+args)
        expected = Output(
            ["/cs/usr/oryan.hassidim/ex3b/ex3b-oryan.hassidim/oh_tests/new_generic_type"] + args)
        if got != expected:
            print("args: ", end="")
            print(*args)
            print(
                f"Got is different from expected!\nGot:\n{got}\nExpected:\n{expected}\n\n")
            Output("rm new_generic_type")
            Output("rm new_generic_type.c")
            return False
        # check valgrind
        s, o = test_valgrind(["new_generic_type"] + args)
        if not s:
            print("valgrind error!\n")
            print("args: ", end="")
            print(*args)
            print(o)
            Output("rm new_generic_type")
            Output("rm new_generic_type.c")
            return False
    # with allocations limit
    for i in range(50):
        args = [rand_num(INT_MAX), rand_num(50), 1000]
        got = Output(["new_generic_type"]+args)
        expected = Output(
            ["/cs/usr/oryan.hassidim/ex3b/ex3b-oryan.hassidim/oh_tests/new_generic_type"] + args)
        if got != expected:
            print("args: ", end="")
            print(*args)
            print(
                f"Got is different from expected!\nGot:\n{got}\nExpected:\n{expected}\n\n")
            Output("rm new_generic_type")
            Output("rm new_generic_type.c")
            return False
        if got.exit_code != 0 and "Allocation failure" not in got.stdout:
            print("args: ", end="")
            print(*args)
            print("the program didn't print 'Allocation failure' exit with code 1 after allocation error!")
            Output("rm new_generic_type")
            Output("rm new_generic_type.c")
            return False
        # check valgrind
        s, o = test_valgrind(["new_generic_type"] + args)
        if not s:
            print("valgrind error!\n")
            print("args: ", end="")
            print(*args)
            print(o)
            Output("rm new_generic_type")
            Output("rm new_generic_type.c")
            return False
    Output("rm new_generic_type")
    Output("rm new_generic_type.c")
    return True


tests = [test_tweets_generator,
         test_conitues_tweets_generator,
         test_snakes_and_ladders,
         test_new_generic_type]


def main():
    if len(sys.argv) > 1:
        i = int(sys.argv[1]) - 1
        test = tests[i]
        print(f"starting test {i+1} - {test.__name__}: ")
        if test():
            print(f"test {i+1} succeed:)")
            return 0
        print(f"test {i+1} failed:(")
        return 1
    passed = 0
    for i, test in enumerate(tests):
        print(f"starting test {i+1} - {tests[i].__name__}: ")
        if test():
            print(f"test {i+1} succeed:)")
            passed += 1
        else:
            print(f"test {i+1} failed:(")
        print()
    print(f"passed {passed} from {len(tests)}:)")
    return 0 if passed == len(tests) else 1


if __name__ == "__main__":
    sys.exit(int(main() or 0))
