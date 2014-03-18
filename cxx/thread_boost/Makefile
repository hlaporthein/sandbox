BOOST_ROOT=/c/jlouis/install/boost_1_55_0

all:
	g++ -c main.cc -o main.o -std=c++11 -I${BOOST_ROOT}
	g++ -o thread.exe main.o -L${BOOST_ROOT}/stage/lib -lboost_system-mgw48-mt-1_55 -lboost_thread-mgw48-mt-1_55

clean:
	rm -f *.o *.exe
