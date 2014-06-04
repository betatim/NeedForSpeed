Simple Kalman Filter
====================

This implements an incredibly simple Kalman filter. It uses [blaze-lib][blaze]
to perform the matrix multiplications. `blaze-lib` knows about SIMD
instructions.

[blaze]: https://code.google.com/p/blaze-lib/

The Kalman filter is used to estimate the position and velocity of an
object falling from a tower. To run it `make all` in this directory after
installing `blaze-lib`.


Requirements
============

To use this you will have to [install][blazeinstall] `blaze-lib` on
your system. `blaze-lib` needs [Boost][boost] so install that first.

Furthermore, [Eigen3][eigen3] is a requirement. You don't need to "install"
Eigen since it's a template library and consists only of header files. However,
you will have to specify the path to the headers in the Makefile or put them in
your system's standard path for headers.

[blazeinstall]: https://code.google.com/p/blaze-lib/wiki/Configuration_and_Installation
[boost]: http://www.boost.org/
[eigen3]: http://eigen.tuxfamily.org/
