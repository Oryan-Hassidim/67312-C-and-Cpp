wsl gcc -std=c99 -Wextra -Wvla -Werror -g tweets_generator.c markov_chain.c linked_list.c -o ./out/test_valgrind/tweets_generator
wsl gcc -std=c99 -Wextra -Wvla -Werror -g -D LIMIT_ALLOCS=20 -D OH_TESTS tweets_generator.c markov_chain.c linked_list.c oh_tests/oh_tests_helpers.c -o ./out/test_valgrind/tweets_generator_limit
cp justdoit_tweets.txt ./out/test_valgrind/justdoit_tweets.txt

wsl valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file="./out/test_valgrind/valgrind.log" ./out/test_valgrind/tweets_generator 1 10 justdoit_tweets.txt 10000
wsl valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file="./out/test_valgrind/valgrind_limit.log" ./out/test_valgrind/tweets_generator_limit 1 10 justdoit_tweets.txt 10000

wsl cat "./out/test_valgrind/valgrind.log"
wsl cat "./out/test_valgrind/valgrind_limit.log"
