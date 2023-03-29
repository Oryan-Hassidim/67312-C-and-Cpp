git checkout submit
git checkout master -- ProductBST.h ProductBST.c
wsl clang-format -i -style=GNU ProductBST.h ProductBST.c
git add -u
git commit -m "Submit ex2"
git push
git checkout master