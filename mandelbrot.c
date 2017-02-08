#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

int cal_pixel(double complex d, int b, int N){
    int count = 1;
    int z = 0;
    while ((abs(z) < b) && (count < N)){
        z = powf(z, 2) + d;
        count = count+1;
    }
    int color = 0; //hur får vi fram färgen? mappa?
    return color;
}

void mandelbrot(){
    int N = 256;
    int b = 2;
    int w = 2046;
    int h = 2046;
    double dx = (2*b)/(w-1);
    double dy = (2*b)/(h-1);

    for (int x = 0; x < (w-1); w++){
        int d_real = x*dx-b;
        for (int y = 0; y < (h-1); y++){
            double complex d_imag = y*dy-b;
            double complex d = d_real+I*d_imag;
            int color = cal_pixel(d, b, N);
        }
    }
}

int main(){
    mandelbrot();

}
