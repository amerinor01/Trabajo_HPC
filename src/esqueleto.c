#include <stdio.h>
#include "cblas.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "memoryfun.h"
#include <omp.h>

#define SEED 2022
#define TAG 22

#define ALPHA 1
#define BETA 0

#define ROOT 0

#define DEBUG 0

#define NUM_THREADS 4

int main(int argc, char **argv){

    int i,j;              /* Indices para bucles */
    int n;                  /* Tamanyo de las matrices */

    double *A,*B,*C, *D;                    /*Punteros de las matrices globales*/

    /*Metrics vars*/
    double inicio, fin;                                                                                                                                                                                        
    double  duration;  

    srand(SEED);
    
    /* Comprobación numero de argumentos correcD=dmatrix(ntos. Se pasa m */
    if (argc!=2){
    printf("Error de Sintaxis. Uso: mpi_gemm n \n");
    exit(1);
    }

    /* Lectura de parametros de entrada */
    n=atoi(argv[1]); 


    /* Reserva de espacio para las matrices locales utilizando las rutinas en memoryfun.c */

    /*Alloc Global non-shared variables*/
    A=dmatrix(n,n);
    B=dmatrix(n,n);    
    C=dmatrix(n,n);	
    D=dmatrix(n,n);

        /* Relleno de las matrices. Uso de macro propia o memset para inicializar a 0*/
    for (i = 0; i < n; ++i)
        for(j = 0; j < n; ++j){
            M(A,i,j,n) = drand48();
            M(B,i,j,n) = drand48();
        }

    #if DEBUG
        printf("MATRIX A\n");
        printMatrix(A,n,n);
        printf("\n");
        printf("MATRIX B\n");
        printMatrix(B,n,n);
        printf("\n");
    #endif

    //init metrics
    inicio = omp_get_wtime();

    // Multiply A * B and B * A
    #pragma omp parallel private(i)
    {  
        printf("Thread %d started\n", omp_get_thread_num());
        #pragma omp for
            for(i = 0; i < n; ++i){                                          
            cblas_dgemv(CblasRowMajor,CblasNoTrans,n,n,ALPHA,A,n,&M(B,i,0,1),n,BETA,&M(C,i,0,1),n); // C = A * B
            cblas_dgemv(CblasRowMajor,CblasNoTrans,n,n,ALPHA,B,n,&M(A,i,0,1),n,BETA,&M(D,i,0,1),n); // D = B * A
            }
    }

    // Multiply C * D 
    #pragma omp parallel private(i)
    {  
        #pragma omp for
            for(i = 0; i < n; ++i){
            cblas_dgemv(CblasRowMajor,CblasNoTrans,n,n,ALPHA,C,n,&M(D,i,0,1),n,BETA,&M(B,i,0,1),n);
            }
    }

    //end metrics
    fin = omp_get_wtime();

    duration = (fin - inicio);
    printf("%f\n", duration);

    //print matrix result
    #if DEBUG
        printMatrix(C, n, n);
        printf("\n");
        printMatrix(D, n, n);
        printf("\n");
        printMatrix(B, n, n);
    #endif

    //exit program
    return 0;

}

/* 


// Expected pattern:
C = A * B
D = B * A
B = C * D

// Example:
A =

     1     5     9     13
     2     6    10     14
     3     7    11     15
     4     8    12     16
	
B =

    17    21    25    29
    18    22    26    30
    19    23    27    31
    20    24    28    32


Resultado 
         538(250)         650(260)         762 (270)         874(280)
         612 (618)        740(644)         868 (670)         996(696)
         686 (986)        830 (1028)       974 (1070)        1118(1112)
         760 (1354)       920(1412)        1080(1470)        1240(1528)
		

*/




