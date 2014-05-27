NeedForSpeed
============

Fast or not? Collective wisdom on what is faster than other things.

This is a collection of recipes/dirty tricks for how to write C++11
code which will run fast. Some will be examples of how _not_ to do it.
Some will show that it does not matter what you do.

The aim is to provide a collection of examples that can be consulted for
enlightenment. Things to ponder when investigating speed ups to your C++.


Reading material
================

Links to other places that collect information on this topic:

* Andrei Alexandrescu talking about [Writing Quick Code in C++, 
  Quickly][quickcpp] and [Three Optimization Tips for C++][threetips]
* Herb Sutter's [Modern C++: What You Need to Know][moderncpp]
* Agner Fog's excellent guide to fast C++ http://www.agner.org/optimize/

[quickcpp]:http://channel9.msdn.com/Events/GoingNative/2013/Writing-Quick-Code-in-Cpp-Quickly
[threetips]:https://www.facebook.com/notes/facebook-engineering/three-optimization-tips-for-c/10151361643253920
[moderncpp]:http://channel9.msdn.com/Events/Build/2014/2-661


Binary searches
===============

searching.cpp compares three different way of finding the first
element larger than X in a vector. Compile it with:

```
for d in FIND STD SELF; do
  clang++ -std=c++11 -stdlib=libc++ -O3 searching.cpp -D$d -o search_$d.x;
done;
```

This creates three executables one for each method. The three methods
are a home made binary search, std::lower_bound and std::find_if.


Timers
======

timing.cpp is an example of how to measure the passage of time. Benchmarks
all require some kind of time measurement. The more precise the clock
the better. Though there are a lot of pitfalls when it comes to building
a good benchmark (are you testing what you think you are?), especially when
it is a micro-benchmark.

```
clang++ -std=c++11 -stdlib=libc++ -O3 timing.cpp -o timing.x;
```

The timers which are part of the C++11 standard library are pretty good!


Cache sizes
===========

Do you know how big your various caches are? This example measures the
throughput by reading from an array about 100MB in size. Each read
uses a different `step size`, making it increasingly harder for the
computer to prefetch elements of the array.

```
clang++ -std=c++11 -stdlib=libc++ -O3 cache_sizes.cpp -o cache.x;
```

It prints the step size as well as MB/s read for that step size. The script
`cache_plot.gp` is a gnuplot script that will plot the results.
