# dsa
This repository contains implementations of various data structures and algorithms in C++. I have implemented them in a STL-like way. This is primarily an educational project and is not intended to be used otherwise.

The implementations are in the `include` folder and the `tests` folder contains various tests to check the working of the algorithm/datastructure.

Other small programs leveraging the use of Linux APIs are stored in the `linux` directory. You will need a linux installation to try them out.


## How to run

Firstly, install [meson](https://github.com/mesonbuild/meson) and [ninja](https://github.com/ninja-build/ninja)

Clone the repository

```
$ git clone https://github.com/ananthvk/algorithms
$ cd dsa
```

Then build and run the tests

```
$ meson setup builddir
$ cd builddir
$ ninja -j8 test
```
