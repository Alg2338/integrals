#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "header.h"

double h1(double x)
{
    return sqrt (x + 3);    
}
double k1(double x)
{
    return 1 / (2 * sqrt(x + 3));
}

double h2(double x)
{
    return (x * x * (0.5*x - 3.25));
}
double k2(double x)
{
    return (x * (1.5*x - 6.5));
}

double h3(double x)
{
    return -4 * x + 1.25;
}
double k3(double x)
{
    return -4;
}
//Functions h1-k3 are used to test
//root

int q_test;       /*should the program write the 
                  quontity of iterations in root cycle*/

int count;   /*if yes, it is counter*/      

double (*f[])(double) = {f1, f2, f3, g1, g2, g3,
                         h1, h2, h3, k1, k2, k3}; 

//array for testing integral and root               

double root(double (*f)(double), double (*g)(double), double (*h)(double),
            double (*k)(double), double a, double b, double eps1)
{ //df/dx = h
    //dg/dx = k

    //F(x) = f(x) - g(x)
    double y1, y2; //y1 = F(a)
                   //y2 = F(a + eps1)
    count = 1;
    y1 = f(a) - g(a);
    y2 = f(a + eps1) - g(a + eps1);

    while ((y1 > 0 && y2 > 0) ||
           (y1 < 0 && y2 < 0))
    {
        y2 = h(a) - k(a); //y2 = F'(a)
        a -= y1 / y2;     //a -= F(a)/F'(a)

        y1 = f(a) - g(a);
        y2 = f(a + eps1) - g(a + eps1);
        count++;
    }

    return a + eps1 / 2;
}

double integral(double (*f)(double),
                double a, double b, double eps2)
{
    int i, n = 2;
    double I, tmpI, h = (b - a) / 2;
    double *F = malloc((n + 1) * sizeof(double));
    if (!F)
        exit(1);

    F[0] = f(a);
    F[1] = f((a + b) / 2);
    F[2] = f(b);
    tmpI = (F[0] + 4 * F[1] + F[2]) * h / 3;

    do
    {
        n *= 2;
        F = realloc(F, (n + 1) * sizeof(double));
        if (!F)
            exit(2);
        I = tmpI;
        tmpI = 0;
        h = (b - a) / n;
        F[n] = F[n / 2];
        tmpI = F[n] + F[0];
        for (i = n - 1; i > 0; i--)
        {
            if (i % 2)
            {   //i is odd
                //calculate the function
                F[i] = f(a + i * h);
                tmpI += 4 * F[i];
            }
            else
            { //i is even
                F[i] = F[i / 2];
                tmpI += 2 * F[i];
            }
        }
        tmpI *= (h / 3); // I2
    } while ((I - tmpI) / 15 >= eps2 || (I - tmpI) <= -eps2);
    free(F);
    return tmpI; // I(2n)
}

int main(int argc, char *argv[])
{
    //df1/dx = g1
    //df2/dx = g2
    //df3/dx = g3

    int i, print_roots = 0;
    for (i = 1; i < argc; i++)
    {
        if (strncmp(argv[i], "-help", 6) == 0)
        {
            printf ("-print_roots \t print roots\n");
            printf ("-print_q \t print quontity of iterations\n");
            printf ("-test_root \t test function root\n");
            printf ("-test_integral \t test the function integral\n");
	    return 0;		        
	}
        else if (strncmp(argv[i], "-print_roots", 13) == 0){
            print_roots = 1;
	}
        else if (strncmp(argv[i], "-print_q", 9) == 0){
            q_test = 1;
	}
        else if (strncmp(argv[i], "-test_root", 11) == 0){
            int j, k;
            double a, b, eps1;
            printf ("test the function:\n");
            printf ("root (f, g, f', g', a, b, eps1)\n");
            printf ("functions:\n\t1: sqrt(x+3)\n\t2: 0.5*x^3 - (13/4)*x^2\n");
            printf ("\t3: -4x + 5/4\n");
            printf ("choose 2 f-s\n");
            scanf ("%d%d", &j, &k);
            printf ("type a, b, eps (eps != 0)\n");
            printf ("a is between:\n\t(5, 6.6) for f-s 1,2\n");
            printf ("\t(4, 5) for f-s 2,3\n");
            printf ("\t(-1, -0.12) for f-s 1,3\n");
            scanf ("%lf%lf%lf", &a, &b, &eps1);
            printf ("The root is\t");
            printf ("%lf\n", root(f[j + 5], f[k + 5],
                     f[j + 8], f[k + 8], a, b, eps1)); 
            return 0;    
		//at the (a, b), exist (f'(x)-g'(x))(f''(x)-g''(x))<0          
        }
        else if (strncmp(argv[i], "-test_integral", 15) == 0){
             int j;
             double a, b, eps2;
             printf ("test the function:\n");
             printf ("integral (f, a, b, eps2)\n");
             printf ("functions:\n\t1: lnx\n\t2: -2x + 14\n");
             printf ("\t3: 6 + 1 / (2 - x)\n");
             printf ("\t4: 1 / x\n\t5: -2\n");
             printf ("\t6: 1 / (2 - x) ^ 2\n");
             printf ("choose the function\n");
             scanf ("%d", &j);
             printf ("type a, b, eps (eps != 0)\n");
             scanf ("%lf%lf%lf", &a, &b, &eps2);
             printf ("The integral is\t");
             printf ("%lf\n", integral (f[j - 1], a, b, eps2));
	     return 0;        
	}
    }

    double root1, root2, root3;
    double I;
    root1 = root(f1, f2, g1, g2, 1, 7, 0.0000001);
    if (q_test)
    {
        printf ("num of iterations root1: %d\n", count);
    }
    root2 = root(f2, f3, g2, g3, 4, 5, 0.0000001);
    if (q_test)
    {
        printf ("num of iterations root2: %d\n", count);
    }
    root3 = root(f1, f3, g1, g3, 2.01, 3, 0.0000001);
    if (q_test)
    {
        printf ("num of iterations root3: %d\n", count);
    }
    if (print_roots)
    {
        printf("root1 = %lf \t(lnx = -2x + 14)\n", root1);
        printf("root2 = %lf \t(-2x + 14 = 6 + 1 / (2 - x)\n", root2);
        printf("root3 = %lf \t(lnx = 6 + 1 / (2 - x)\n", root3);
    }

    I = integral(f3, root3, root2, 0.001);
    I += integral(f2, root2, root1, 0.001);
    I -= integral(f1, root3, root1, 0.001);
    printf("The square is %lf\n", I);
    return 0;
}
