all:
	gcc -c main.cc -o main.o -Wall -std=c++11 -Wno-write-strings -Wno-unused-variable -Wno-unused-but-set-variable
	g++ -o keywords main.o -Wall -O -L${BOOST_ROOT}/stage/lib \
	-lboost_system-mgw48-mt-1_55 \
	-lboost_thread-mgw48-mt-1_55

clean:
	rm -f *.o *.exe

test: all
	./keywords