# Option type for C++14

Provides a C++ option type similar to Haskell's Maybe type.

The test.cpp file shows example usage.

This work has been largely inspired by the Maybe type in[simonask's reflect
repository](https://github.com/simonask/reflect). I decided to make my own
version of Maybe because I found myself unable to use some functionalities
like `maybe_if` or `map` with lambdas, maybe because I used them the wrong way.

Maybe is very basic for now and I might add functionalities (or make it safer to
use) in the future.

