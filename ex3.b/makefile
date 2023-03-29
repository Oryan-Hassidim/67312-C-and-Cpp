tweets: linked_list.o markov_chain.o tweets_generator.o
	gcc -std=c99 -Wextra -Wvla -Werror -g linked_list.o markov_chain.o tweets_generator.o -o tweets_generator

snake: linked_list.o markov_chain.o snakes_and_ladders.o 
	gcc -std=c99 -Wextra -Wvla -Werror -g linked_list.o markov_chain.o snakes_and_ladders.o -o snakes_and_ladders

new_type: linked_list.c markov_chain.c oh_tests/oh_tests_helpers.c _new_generic_type.c
	gcc -std=c99 -Wextra -Wvla -Werror -g -D OH_TESTS _new_generic_type.c linked_list.c markov_chain.c oh_tests/oh_tests_helpers.c -o oh_tests/new_generic_type
	chmod a+x oh_tests/new_generic_type
	
justdoit_tweets.txt: justdoit_tweets.txt
	cp justdoit_tweets.txt justdoit_tweets.txt

snakes_and_ladders.o: snakes_and_ladders.c
	gcc -std=c99 -Wextra -Wvla -Werror -g -c snakes_and_ladders.c -o snakes_and_ladders.o

tweets_generator.o: tweets_generator.c
	gcc -std=c99 -Wextra -Wvla -Werror -g -c tweets_generator.c -o tweets_generator.o

markov_chain.o: markov_chain.c
	gcc -std=c99 -Wextra -Wvla -Werror -g -c markov_chain.c -o markov_chain.o

linked_list.o: linked_list.c
	gcc -std=c99 -Wextra -Wvla -Werror -g -c linked_list.c -o linked_list.o
cleano:
	rm *.o
clean:
	rm *.o
	rm *tweets_generator
	rm *snakes_and_ladders
