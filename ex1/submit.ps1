git checkout submit
git checkout master -- main.c cipher.c cipher.h tests.c tests.h
wsl clang-format -i -style=GNU main.c tests.c cipher.c
git add -u
git commit -m "Submit ex1"
git push
git checkout master