# include <mpi.h>
# include <stdio.h>

#define MAX_NUMBER 1000000
#define DISPLAY_PROGRESS 1
#define DISPLAY_FACTORY 10

unsigned int prime_number(unsigned int n, int id, int numProcesses);
int isprime(unsigned int n);

int main() {
    int id;
    int numProcesses;
    int display_factor;
    unsigned int n;
    unsigned int primes;
    unsigned int primes_partial;

    if(DISPLAY_PROGRESS){
        n = 1;
    }
    else{
        n = MAX_NUMBER;
    }
    
    display_factor = DISPLAY_FACTORY;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if(id == 0) {
        printf ( "%d processes available\n\n", numProcesses );
    }

    while(n <= MAX_NUMBER){
        MPI_Bcast (&n, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

        primes_partial = prime_number (n, id, numProcesses);
        MPI_Reduce (&primes_partial, &primes, 1, MPI_UNSIGNED, MPI_SUM, 0, MPI_COMM_WORLD);

        if (id == 0){
            printf ("%10d    %10d\n", n, primes);
        }
        n = n * display_factor;
    }

    if(id == 0) {
        printf ("\nTotal number of prime:%u\n",primes);
    }
    MPI_Finalize ();
    
    return 0;
}

unsigned int prime_number(unsigned int n, int id, int numProcesses) {
    unsigned int total = 0;

    for(unsigned int i = 2 + id; i <= n; i += numProcesses) {
        if(isprime(i)){
            total += 1;
        }
    }
    return total;
}

int isprime(unsigned int n) {
    for(unsigned int i = 2; i < n; ++i) {
        if(( n % i ) == 0) {
            return 0;
        }
    }
    return 1;
}