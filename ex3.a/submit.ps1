git checkout submit
git checkout master -- markov_chain.c markov_chain.h tweets_generator.c
wsl clang-format -i -style=GNU markov_chain.c markov_chain.h tweets_generator.c
git add -u
git commit -m "Submit ex3.a"
git push
git checkout master