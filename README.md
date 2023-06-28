# euraud
#### Introduction

This app reads through each line of quote presented in a JSON file dump. The quotes belong to the EURAUD currency pair in FX-trading. The main goal of this exercise is to efficiently parse quote-batches of 30-minute time intervals from the dump and for each batch of quotes, the following stats are to presented either in a text file or to the standard output:

- Minimum Bid Price
- Maximum Bid Price
- Minimum Ask Price
- Maximum Ask Price
- Mean Bid Price
- Mean Ask Price
- Median Bid Price
- Median Ask Price

At this time, the stats are written to a text file called `output.txt`.

#### System Requirements

- Linux OS
- GCC Compiler. Version >= 8.1
- CMake >= 3.12
- GoogleTest

#### Notes and Caveats

- The code adheres to the C++17 standard.
- For Performance-testing, `cmake` compile-time flags related to Serial or Parallel execution are provided.
    - `-DSERIAL=ON` to test serial execution.
    - `-DPOOL=ON` to enable concurrent execution via a thread pool. This is the **default**.
    - `-DASYNC=ON` for an experimental monitoring exercise with `<future>`'s `std::async()` for concurrent tasks.
- It was observed that a _thread pool_ with `4` threads produced the best performance on a system with specs:
    - CPU(s):                          12
    - Model name:                      Intel(R) Core(TM) i7-10710U CPU @ 1.10GHz
    - Thread(s) per core:              2
    - Core(s) per socket:              6
    - Socket(s):                       1
- Performance numbers on the *Ubuntu 22.04* system have been recorded in `PerfNumsEURAUD.pdf`.
- `GTest` was chosen for unit-testing. **NOTE** that `googletest` has not been made available internally in this project, unfortunately the user needs to have the `GTest` package pre-installed on their system.
- Input file `euraud.json` is made available in the project's root directory along with the exercise description in `exercise.txt`.

#### Run

Unarchive the tar file, compile and run:
```sh
tar xvJf euraud.tar.xz
mkdir build && cd $_
cmake ..
make -j4
```

Run the executable:
```sh
./euraud
```
Examine stats in the `output.txt` file produced by the run.

**NOTE** that the `cmake ..` step configures the default _thread pool_-based run. To optionally, try out serial and `std::async`-based runs, you may use cmake setups like this:
```sh
cmake -DSERIAL=ON ..
```

Hopefully, you do not have to try out the Debug mode, but if you do, you may do:
```sh
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

The unit-tests segment definitely needs to grow. An executable called `utest` gets generated under `build` at `build/tests/utest`. Post the `make`, you may try:
```sh
ctest
```
or the full path to `tests/utest` to run the unit test/s.