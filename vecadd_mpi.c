#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#define TOL 0.0000001

int main(int argc, char *argv[]) {
	
    //iniciando as variáveis que auxiliarão a quebrar o vetor em blocos e para medir o tempo com MPI_Wtime	
    int start, end, size, rank;
    double t1, t2;
    
    // definindo o tamanho do vetor
    int vetsize = 15000000;
    int err = 0;
    
    // Abordagem: dividir o vetor em blocos, onde o cada processo irá encher e fazer a soma dos vetores da posição [start] até a posição [end]
    	MPI_Init(&argc,&argv);
	t1 = MPI_Wtime();
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	start = rank*(vetsize/size);
	if(rank==(size-1)){
		end = vetsize;
	}else{
		end = start + (vetsize/size)-1;
	}
    
    float *a = (float*) malloc(vetsize * sizeof(float));
    float *b = (float*) malloc(vetsize * sizeof(float));
    float *c = (float*) malloc(vetsize * sizeof(float));
    float *res = (float*) malloc(vetsize * sizeof(float));

    // enchendo os vetores
    for (int i = start; i <= end; i++){
        a[i] = (float) i;
        b[i] = 2.0 * (float) i;
        c[i] = 0.0;
        res[i] = i + 2 * i;
    }
    
    // adicionando os vetores
    for (int i = start; i <= end; i++){
        c[i] = a[i] + b[i];
    }
    
    // testando os resultados
    for (int i = start; i <= end; i++){
        float val = c[i] - res[i];
        val = val * val;

        if(val > TOL)
            err++;
    }
    	t2 = MPI_Wtime();
    	MPI_Finalize();


    printf("vectors added with %d errors in %lf seconds\n", err, (t2-t1));

    free(a);
    free(b);
    free(c);
    free(res);
    
    return 0;
}
