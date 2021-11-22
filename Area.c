/*
    Actividad 1.5 Multiprocesadores

    Jonatan Emanuel Salinas Avila       A01731815
    30/Ago/2021

    Numero optimo de Threads para este ejercicio: 130
*/

#include<stdio.h>
#include "omp.h"

static long num_pasos = 100000000;

#define NUM_THREADS 890

double function(double valor_x){
    return (1.0/valor_x);
}

int main()
{   
    int i;
    int nthreads;
    double pi;
    double sum[NUM_THREADS];
    double t1;
    double t2;
    double tiempo;
    double delta_x = 1.0/num_pasos;
    
    omp_set_num_threads(NUM_THREADS);

    const double startTime = omp_get_wtime();

    #pragma omp parallel
    {
        int id;
        int i;
        int nthrds;

        double x = 0.0;

        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();

        //printf("id: %d , nthrds: %d\n", id, nthrds);

        if(id == 0)
            nthreads = nthrds;
        
        for(i = id, sum[id] = 0.0; i<num_pasos; i = i+nthrds){
            x = ((i + 0.5) * delta_x)+1;    
            sum[id] += function(x);
        }        
    }

    for(i = 0, pi = 0.0; i<nthreads; i++){
        pi+=sum[i] * delta_x;
    }

    t2 = omp_get_num_threads();
    const double endTime = omp_get_wtime();
    tiempo = t2-t1;
    //printf("pi = (%lf)\n", pi);
    printf("tomo (%lf) segundos\n", (endTime -startTime));

}