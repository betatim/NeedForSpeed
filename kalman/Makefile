CC=/usr/bin/clang++ -std=c++11 -stdlib=libc++
OPT= -O3 -DNDEBUG

all: kalman

%.o: %.cpp
	$(CC) $(OPT) -c $<

kalman: kalman.o
	$(CC) $(LIB) -o kalman.x $^


kalman_eigen.x: kalman_eigen.cpp
	clang++ -O3 -std=c++11 -stdlib=libc++ -I/opt/local/include/eigen3/ -o kalman_eigen.x kalman_eigen.cpp

clean:
	rm -rf *.x *.o *~
