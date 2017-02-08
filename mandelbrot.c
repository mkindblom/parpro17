#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

int cal_pixel(double complex d, int b, int N){
    int count = 1;
    double complex z = 0;
    double complex z_prev = 0;
    while ((cabs(z) < b) && (count < N)){
        z_prev = z;
        z = powf(z, 2) + d;
        count = count+1;
    }

    if (cabs(z) < b && count >= N){
        return 256;   
    } 
    else if ((cabs(z) >= b) && cabs(z_prev) < b){
        return count;
    }
    return -1;  //vad händer?
}

void mandelbrot(){
    int N = 256;
    int b = 2;
    int w = 512;
    int h = 512;
    long dx = (2*b)/(w-1);
    long dy = (2*b)/(h-1);
    double d_real;
    double complex d_imag;
    printf("hej\n");

    for (int x = 1; x < (w-1); x++){
        d_real = x*dx-b;
        for (int y = 1; y < (h-1); y++){
            d_imag = y*dy-b;
            double complex d = d_real+I*d_imag;

            printf("%ld\n", dx);
            printf("%ld\n", dy);
            printf("%f + i%f\n", creal(d), cimag(d));
            int color = cal_pixel(d, b, N);
            if (color == -1) {
                printf("Something's wrong...\n");
            } else {
                printf("%d\n", color);
            }
        }
    }
}

int main(){
    
    printf("hej\n");
    mandelbrot();
    return 0;
}
