# include <mpi.h>
# include <stdio.h>
#include <sys/param.h>

#define MAX_NUMBER 4294967295
#define DISPLAY_PROGRESS 1
#define DISPLAY_FACTORY 10

int end_now = 0;
int id;


unsigned int prime_number(unsigned int n, int id, int numProcesses);

int isprime(unsigned int n);

void sig_handler(int signo) {
    if (id == 0) {
        printf("<Signal received>\n");
    }
    if (signo == SIGUSR1) {
        end_now = 1;
    }
}


int main() {
    int numProcesses;
    int display_factor;
    unsigned int n;
    unsigned int primes = 0;
    unsigned int primes_partial;
    signal(SIGUSR1, sig_handler);
    int progress = 0;


    if (DISPLAY_PROGRESS) {
        n = 1;
    } else {
        n = MAX_NUMBER;
    }

    display_factor = DISPLAY_FACTORY;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Status status;

    if (id == 0) {
        printf("%12c\t\t%s\n",'N',"Primes");
        fflush(stdout);
    }

    while (n <= MAX_NUMBER && end_now == 0) {
        MPI_Bcast(&n, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

        primes_partial = prime_number(n, id, numProcesses);
        MPI_Reduce(&primes_partial, &primes, 1, MPI_UNSIGNED, MPI_SUM, 0, MPI_COMM_WORLD);

        if (id == 0 && end_now != 1 && primes != 0) {
            printf("%12d\t%12d\n", n, primes);
            fflush(stdout);
        }
        if (end_now == 1 && id == 0) {
            int temp = 0;
            int count = 1;
            if (numProcesses != count) {
                count = (numProcesses + 1) / 2;
            }
            for (int i = 0; i < count; ++i) {
                MPI_Recv(&temp, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                if (temp > progress) {
                    progress = temp;
                }
            }
            printf("%12d\t%12d\n", progress, primes);
            fflush(stdout);
        }
        n = n * display_factor;
    }

//    if (id == 0) {
//        printf("\nTotal number of prime:%u\n", primes);
//    }
    MPI_Finalize();

    return 0;
}

unsigned int prime_number(unsigned int n, int id, int numProcesses) {
    unsigned int total = 0;
    int progress = 0;

    for (unsigned int i = (unsigned int) (2 + id); i <= n; i += numProcesses) {
        if (isprime(i)) {
            total += 1;
        }
        if (end_now == 1) {
            progress = i;
            MPI_Send(&progress, 1, MPI_INT, 0, 999, MPI_COMM_WORLD);
            return total;
        }
        progress = i;
    }
    return total;
}

int isprime(unsigned int n) {
    for (unsigned int i = 2; i < n; ++i) {
        if ((n % i) == 0) {
            return 0;
        }
    }
    return 1;
}