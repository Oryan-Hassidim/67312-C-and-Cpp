$files = "RSUser.cpp", "Movie.cpp", "RecommenderSystem.cpp", "RecommenderSystemLoader.cpp", "RSUsersLoader.cpp", "RSUser.h", "Movie.h", "RecommenderSystem.h", "RecommenderSystemLoader.h", "RSUsersLoader.h", "presubmit.cpp"

# if the dir ./out/test_valgrind/ does not exist, create it
if (!(Test-Path "./out/test_valgrind/")) {
    New-Item -ItemType Directory -Path "./out/test_valgrind/"
}

wsl g++ -Wextra -Wvla -Werror -g $files -o ./out/test_valgrind/test
cp presubmit.in_m ./out/test_valgrind/presubmit.in_m
cp presubmit.in_u ./out/test_valgrind/presubmit.in_u

wsl valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file="./out/test_valgrind/valgrind.log" ./out/test_valgrind/test

wsl cat "./out/test_valgrind/valgrind.log"
wsl echo ""
wsl echo ""
wsl echo ""
wsl echo "valgrind done"