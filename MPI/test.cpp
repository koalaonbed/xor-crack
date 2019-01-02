#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[])
{
    int nprocs,rank;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    int n = 100;
    int sendBuf[n],*recvBuf;
    int *displs,stride,*recvCount; //stride:步长
    for (int i=0; i<n; i++){
        sendBuf[i] = i;
    }
    stride = n;
    displs    = (int *)malloc(sizeof(int)*nprocs); //一个进程一个偏移
    recvCount = (int *)malloc(sizeof(int)*nprocs); //一个进程一个接收数据量
    for (int i=0; i<nprocs; i++){
        displs[i] = i*stride;
        recvCount[i] = n;
    }
    recvBuf = (int *)malloc(stride*nprocs*sizeof(int));
    int root = 0;
    MPI_Gatherv(sendBuf,n,MPI_INT,recvBuf,recvCount,displs,MPI_INT,root,MPI_COMM_WORLD);
    if(rank == 0){
        for (int i=0; i<nprocs; i++){
            for (int j=0; j<recvCount[i]; j++){
                printf("%d ",recvBuf[j]);
            }
            printf("\n");
        }
    }
    MPI_Finalize();
    return 0;
}
