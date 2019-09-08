#include <iostream>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <ctime>
#include <fstream>
#include <windows.h>


using namespace std;

int test_vm(long N)
{

    double * a = new double [N*N];

    for (long j = 0; j < N*N; j++)
    a[j] = rand()%100;

    gsl_matrix_view A = gsl_matrix_view_array(a, N, N);

    gsl_vector * v1 = gsl_vector_alloc (N);
    gsl_vector * v2 = gsl_vector_alloc (N);

    for (long j = 0; j < N; j++)
            gsl_vector_set (v1, j, 1.9 + j);

    int start = clock();
    gsl_blas_dgemv(CblasNoTrans,
                    1.0, &A.matrix , v1,
                    0.0, v2);
    int stop = clock();

    gsl_vector_free (v1);
    gsl_vector_free (v2);
    delete a;
    return stop-start;
}



int test_vv(long length)
{
    double v3[1];

    gsl_vector * v1 = gsl_vector_alloc (length);
    gsl_vector * v2 = gsl_vector_alloc (length);

    for (long j = 0; j < length; j++)
    {
        gsl_vector_set (v1, j, 2 + j);
        gsl_vector_set (v2, j, 5 + j);
    }

    int start = clock();
    gsl_blas_ddot (v1, v2, v3);
    int stop = clock();

    gsl_vector_free (v1);
    gsl_vector_free (v2);

    return stop-start;
}
int main ()
{


    fstream plik( "lab2.csv", ios::out );
    plik << "Iteration;VectorSize;VxVTime;MxVTime" << endl;
    for (long i = 1; i <= 10; i++)
    {
        for (long j = 1; j <= 10; j++)
        {
            long N = i*1000;
            int times=test_vv(N*1000);
            int times2=test_vm(N);
            cout<<j<<" "<<N<<" "<<times<<" "<<times2<<endl;
            plik <<j<< ";"<<N<<";"<<times<<";"<<times2<<endl;

        }
    }
    plik.close();



    return 0;
}
