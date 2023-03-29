tweets_generator: out/make/linked_list.o out/make/markov_chain.o out/make/tweets_generator.o
	gcc out/make/linked_list.o out/make/markov_chain.o out/make/tweets_generator.o -o out/make/tweets_generator

out/make/justdoit_tweets.txt: justdoit_tweets.txt
	cp justdoit_tweets.txt out/make/justdoit_tweets.txt

out/make/tweets_generator.o: tweets_generator.c
	gcc -c tweets_generator.c -o out/make/tweets_generator.o

out/make/markov_chain.o: markov_chain.c
	gcc -c markov_chain.c -o out/make/markov_chain.o

out/make/linked_list.o: linked_list.c
	gcc -c linked_list.c -o out/make/linked_list.o
