git checkout submit
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git checkout master -- markov_chain.c markov_chain.h tweets_generator.c snakes_and_ladders.c makefile
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
wsl clang-format -i -style=GNU markov_chain.c markov_chain.h tweets_generator.c snakes_and_ladders.c
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git add -u
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git commit -m "Submit ex3.b"
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git push
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git checkout master