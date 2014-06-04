CXX=clang++
CXXFLAGS=-std=c++11 -stdlib=libc++
CXXFLAGS+=-O3 -DNDEBUG
CXXFLAGS+=$(shell pkg-config --cflags eigen3)

all: kalman_blaze kalman_eigen

clean:
	rm -f kalman_blaze
	rm -f kalman_eigen
