g++ -std=c++2a -Wall -O2 -c example.cpp -o example.o && g++ -std=c++2a example.o -Wall -O2 -lpthread -lcurl -lcurlpp -o example -lsfml-system
