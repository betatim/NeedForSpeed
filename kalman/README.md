Simple Kalman Filter
====================

This implements an incredibly simple Kalman filter. It uses [blaze-lib](blaze)
to perform the matrix multiplications. `blaze-lib` knows about SIMD
instructions.

[blaze]: https://code.google.com/p/blaze-lib/

The Kalman filter is used to estimate the position and velocity of an
object falling from a tower. To run it `make all` in this directory after
installing `blaze-lib`.


Requirements
============

To use this you will have to install `blaze-lib` on your system.