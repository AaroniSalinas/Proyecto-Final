/*
    Este codigo paraleliza SOLO LA SALIDA de una imagen.
    Osea, agarra la imagen original, se aloja memoria y se guardan todos los valores de sus 3 canales 
    en la memoria, y luego de forma paralela se van sacando de ahi y poniendo en la imagen de salida.
*/

#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

#define NUM_THREADS 200

int main()
{
    FILE *image, *outputImage, *lecturas;
    image = fopen("f1.bmp","rb");          //Imagen original a transformar
    outputImage = fopen("f1_gris.bmp","wb");    //Imagen transformada
    long ancho;
    long alto;
    unsigned char r, g, b;               //Pixel
    unsigned char* ptr;

    unsigned char xx[54];
    int cuenta = 0;
    for(int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    //printf("largo img %li\n",alto);
    //printf("ancho img %li\n",ancho);

    ptr = (unsigned char*)malloc(alto*ancho*3* sizeof(unsigned char));


    while(!feof(image)){
      b = fgetc(image);
      g = fgetc(image);
      r = fgetc(image);


      unsigned char pixel = 0.21*r+0.72*g+0.07*b;

      ptr[cuenta] = pixel; //b
      ptr[cuenta+1] = pixel; //g
      ptr[cuenta+2] = pixel; //r


      //cuenta++;
      cuenta = cuenta + 3;

    }                                        //Grises
    
    omp_set_num_threads(NUM_THREADS);
    const double startTime = omp_get_wtime();

    #pragma omp parallel
    {
        //#pragma omp for nowait
        #pragma omp for schedule(dynamic)
        for (int i = 0; i < alto*ancho*3; i+=3) {
        fputc(ptr[i], outputImage);
        fputc(ptr[i+1], outputImage);
        fputc(ptr[i+2], outputImage);
        
        }
    }



    const double endTime = omp_get_wtime();
    printf("tomo (%lf) segundos\n", (endTime -startTime));

    free(ptr);
    fclose(image);
    fclose(outputImage);
    return 0;
}
