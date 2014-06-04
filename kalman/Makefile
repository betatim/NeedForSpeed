CXX=clang++
CXXFLAGS=-std=c++11 -stdlib=libc++
CXXFLAGS+=-O3 -DNDEBUG
CXXFLAGS+=$(shell pkg-config --cflags eigen3)

all: kalman kalman_eigen

clean:
	rm -f kalman
	rm -f kalman_eigen
