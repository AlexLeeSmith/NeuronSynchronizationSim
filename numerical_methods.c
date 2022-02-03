/**
 * This program implements multiple numerical methods for solving ordinary differential equations.
 * 
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @date 1/30/22
 */

/** Preprocessor Directives **/
#include <stdio.h>      // printf()
#include <math.h>       // ceil()
#include <stdlib.h>     // exit(), EXIT_SUCCESS

/** Structures **/
typedef struct {
    float *approx;
    float *x;
    int size;
} Solution;

/** Forward Declarations **/
Solution runEulers(float (*)(float, float), float, float, float, float);
Solution runRungeKutta(float (*)(float, float), float, float, float, float);

/** Functions **/
float dydx1(float x, float y) {return x + y;}
float dydx2(float x, float y) {return x * x;}
float dydx3(float x, float y) {return x + y + x * y;}
float dydx4(float x, float y) {return (x - y) / 2;}

int main(int argc, char const *argv[]) {
    float x0 = 0.0, y0 = 1.0, xEnd = 1.0, step = 0.2;
    float (*dydx)(float, float) = &dydx1;

    runEulers(dydx, x0, y0, xEnd, step);
    runRungeKutta(dydx, x0, y0, xEnd, step);
    
    exit(EXIT_SUCCESS);
}

/**
 * @brief 
 * 
 * @param func 
 * @param x0 
 * @param y0 
 * @param xEnd 
 * @param step 
 * @return Solution 
 */
Solution runEulers(float (*dydx)(float, float), float x0, float y0, float xEnd, float step) {
    Solution sol;
    sol.size = ceil((xEnd - x0) / step);
    float tempY[sol.size];
    float tempX[sol.size];
    sol.approx = tempY;
    sol.x = tempX;
    sol.approx[0] = y0;
    sol.x[0] = x0;
    printf("Euler's: x0 = %f, y0 = %f, xEnd = %f, step = %f\n", x0, y0, xEnd, step);

    printf("(%f, %f)\n", sol.x[0], sol.approx[0]);
    for (int i = 0; i < sol.size; ++i) {
        sol.approx[i + 1] = sol.approx[i] + step * (*dydx)(sol.x[i], sol.approx[i]);
        sol.x[i + 1] = sol.x[i] + step;
        printf("(%f, %f)\n", sol.x[i + 1], sol.approx[i+1]);
    }
    printf("\n");
    
    return sol;
}

/**
 * @brief 
 * 
 * @param func 
 * @param x0 
 * @param y0 
 * @param xEnd 
 * @param step 
 * @return Solution 
 */
Solution runRungeKutta(float (*dydx)(float, float), float x0, float y0, float xEnd, float step) {
    Solution sol;
    sol.size = ceil((xEnd - x0) / step);
    float tempY[sol.size];
    float tempX[sol.size];
    sol.approx = tempY;
    sol.x = tempX;
    sol.approx[0] = y0;
    sol.x[0] = x0;
    printf("Runge-Kutta: x0 = %f, y0 = %f, xEnd = %f, step = %f\n", x0, y0, xEnd, step);

    float k1, k2, k3, k4;
    printf("(%f, %f)\n", sol.x[0], sol.approx[0]);
    for (int i = 0; i < sol.size; ++i) {
        k1 = step * (*dydx)(sol.x[i], sol.approx[i]);
        k2 = step * (*dydx)(sol.x[i] + 0.5 * step, sol.approx[i] + 0.5 * k1);
        k3 = step * (*dydx)(sol.x[i] + 0.5 * step, sol.approx[i] + 0.5 * k2);
        k4 = step * (*dydx)(sol.x[i] + step, sol.approx[i] + k3);

        sol.approx[i + 1] = sol.approx[i] + (k1 + k2 + k2 + k3 + k3 + k4) / 6.0;

        sol.x[i + 1] = sol.x[i] + step;
        printf("(%f, %f)\n", sol.x[i + 1], sol.approx[i + 1]);
    }
    printf("\n");

    return sol;
}