# $files = "srcs/presubmit.cpp"
$files = "main.cpp"

# if the dir ./out/test_valgrind/ does not exist, create it
if (!(Test-Path "./out/test_valgrind/")) {
    New-Item -ItemType Directory -Path "./out/test_valgrind/"
}

wsl g++ -Wextra -Wvla -Werror -g $files -o ./out/test_valgrind/test
# copy io directory to out directory
# Copy-Item -Path "./io/" -Destination "./out/test_valgrind/" -Recurse

wsl valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file="./out/test_valgrind/valgrind.log" ./out/test_valgrind/test

wsl cat "./out/test_valgrind/valgrind.log"
wsl echo ""
wsl echo ""
wsl echo ""
wsl echo "valgrind done"