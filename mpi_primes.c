#include <stdio.h>
#include <mpi.h>

const unsigned int N = 1000;

int find_prime(unsigned int x){
    for(unsigned int i = 2; i < x; ++i){
        if(x%i == 0){
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    int myid, numprocs, source;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    printf("I'm %d of %d\n", myid, numprocs);
    unsigned int start = myid * (N / numprocs), stop = (myid + 1) * (N / numprocs);
    if (myid == numprocs - 1){
        stop = N;
    }
    printf("start from %d to %d\n", start, stop);
    unsigned int ans = 0, i;
    for (i = start; i < stop; i++){
        if (find_prime(i)){
            ans += 1;
        }
    }
    //printf("%d finished calculation with %d numbers\n", myid, ans);

    if (myid != 0)
    {
        MPI_Send(&ans, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        unsigned int tmp;
        for (source = 1; source < numprocs; source++)
        {
            MPI_Recv(&tmp, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
            printf("from %d: %d\n", source, tmp);
            ans += tmp;
        }
        printf("final ans: %d\n", ans);
    }

    MPI_Finalize();
    return 0;
}