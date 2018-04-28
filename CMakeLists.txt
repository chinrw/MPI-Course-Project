cmake_minimum_required(VERSION 3.10)
project(MPI_Course_Project C)

set(CMAKE_C_STANDARD 11)
include_directories(/usr/local/Cellar/open-mpi/3.0.1/include/)
set(CMAKE_C_COMPILER /usr/local/bin/mpicc)


add_executable(MPI_Course_Project
        mpi_primes.c)
