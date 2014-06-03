CC=/usr/bin/clang++ -std=c++11 -stdlib=libc++
OPT= -O3 -DNDEBUG

all: kalman

%.o: %.cpp
	$(CC) $(OPT) -c $<

kalman: kalman.o
	$(CC) $(LIB) -o kalman.x $^

clean:
	rm -rf *.x *.o *~
