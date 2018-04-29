# MPI-Course-Project

This is a simple counter for prime numbers made by C with MPI.

## GUIDE

##### Compile and Run

make<br/>
mpiexec -n 2 hw4.out (2 means the number of processes will be used, can be changed)

##### Send stop signal

ps aux | grep hw4 (find the PID of the process)<br/>
kill -SIGUSR1 PID (change PID to the PID found in previous command)

##### Change range of calculation
change the definition of MAX_NUMBER to desired max number in mpi_primes.c

##### Only show result without progress (slightly shorter finish time)
set DISPLAY_PROGRESS to 0 in mpi_primes.c

##### Show result with progress (slightly longer finish time)
set DISPLAY_PROGRESS to 1 in mpi_primes.c

##### Show progress with different interval
change DISPLAY_FACTORY to desired number