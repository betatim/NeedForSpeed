Performance Counter Monitor
===========================

These are some examples on how to use and make sense of Performance
Counter Monitors. They allow you to get much more detailed information
about the execution of your code compared to just timing it.

The aim of these snippets is not to provide the fastest possible
implementation but to be simple enough so that you can reason about what
is going on and from the figure out what it is the PCM is telling you.

Code examples which are hard to understand, together with a new tool would
make it impossible to figure out how the new tool works.


Requirements
============

This uses code from the IntelPerformanceCounterMonitor V2.6 SDK. You will
have to download and install it. It can be found at:

https://software.intel.com/en-us/articles/intel-performance-counter-monitor-a-better-way-to-measure-cpu-utilization

In order to install it, follow the instructions specified in the *_HOWTO.txt
files. On a Mac, installing the IntelPerformanceCounterMonitor is as easy as
running `sudo make install` in the `MacMSRDriver` sub-directory.

Once you have installed it, edit $PERFSDK in the Makefile to point at the
directory to which you unzip'ed the download. It will then attempt to
copy what it needs from there.

Be aware that the PCM tools will only work with certain Intel processors,
quote:

```Only Intel(R) processors are supported (Atom(R) and microarchitecture
codename Nehalem, Westmere, Sandy Bridge and Ivy Bridge)```

Getting started
===============

Currently there are three examples:

* `vector.cpp` illustrates the difference between adding a new element to a
  vector at the beginning or the end.
* `example.cpp` is an example illustrating how your L2 and L3 cache work.
* `search.cpp` the search example from the main directory

If you have a machine with multiple cores/CPUs you need to make sure your
process is pinned to one of them and not being switched by the scheduler. On
Mac OS X you can achieve this by reducing the number of available cores to one.
As explained for example [here](http://jesperrasmussen.com/blog/2013/03/07/limiting-cpu-cores-on-the-fly-in-os-x-mountain-lion/)
