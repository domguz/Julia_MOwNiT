


#include <gsl/gsl_blas.h>
#include <time.h>


struct timespec spec_diff(struct timespec start, struct timespec stop) {


    struct timespec result;
    if ((stop.tv_nsec - start.tv_nsec) < 0) {
        result.tv_sec = stop.tv_sec - start.tv_sec - 1;
        result.tv_nsec = stop.tv_nsec - start.tv_nsec + 1000000000;
    } else {
        result.tv_sec = stop.tv_sec - start.tv_sec;
        result.tv_nsec = stop.tv_nsec - start.tv_nsec;
    }
    return result;
}

void print(gsl_matrix *A){
    int n = A->size1;
    int m = A->size2;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++)
        {
            printf("%f " ,gsl_matrix_get(A,i,j));

        }
        printf("\n");
    }
    printf("\n");

}

gsl_matrix *rand_matrix_of_size(size_t size) {
    srand(time(NULL));
    int x = size;
    int y = size;
    gsl_matrix *newMatrix = gsl_matrix_alloc(x, y);

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {

            gsl_matrix_set(newMatrix, i, j, rand() % 10);
        }
    }
    return newMatrix;


}

gsl_matrix *zero_matrix_of_size(size_t size) {
    int x = size;
    int y = size;
    gsl_matrix *newMatrix = gsl_matrix_alloc(x, y);


    return newMatrix;


}

void naive_multiplication(gsl_matrix *A,gsl_matrix *B,gsl_matrix *C){

    double *A_d = A->data;
    double *B_d = B->data;
    double *C_d = C->data;
   size_t A_rows = A->size1;
    size_t A_columns = A->size2;
    size_t B_columns = B->size2;
    size_t C_columns = C ->size2;

    for(int i=0; i<B_columns; i++){
        for(int j=0;j<A_rows;j++){
            int c_tmp = j*C_columns+i;
            for(int k=0;k<A_columns;k++){
                int a_tmp = j*A_columns + k;
                int b_tmp = k*B_columns + i;
                C_d[c_tmp] = C_d[c_tmp] + (A_d[a_tmp] * B_d[b_tmp]);
            }

        }
    }


}

void better_multiplication(gsl_matrix *A,gsl_matrix *B,gsl_matrix *C){

    double *A_d = A->data;
    double *B_d = B->data;
    double *C_d = C->data;
    size_t A_rows = A->size1;
    size_t A_columns = A->size2;
    size_t B_columns = B->size2;
    size_t C_columns = C ->size2;

    for(int j=0; j<A_rows; j++){
        for(int k=0;k<A_columns;k++){
            int a_tmp = j*A_columns+k;
            for(int i=0;i<B_columns;i++){
                int c_tmp = j*C_columns + i;
                int b_tmp = k*B_columns + i;
                C_d[c_tmp] = C_d[c_tmp] + (A_d[a_tmp] * B_d[b_tmp]);
            }

        }
    }


}


int main() {


    FILE *file = fopen("../lab3/result2.csv", "w");
    fprintf(file, "size,iteration,time_naive,time_better,time_gsl\n");




    int range = 600;
    int times = 10;
    for (int i = 100; i < range; i += 50) {
        for (int j = 0; j < times; j++) {

            gsl_matrix *A_matrix = rand_matrix_of_size(i);
            gsl_matrix *B_matrix = rand_matrix_of_size(i);
            gsl_matrix *C_matrix = zero_matrix_of_size(i);


            struct timespec start;
            struct timespec end;
            clock_gettime(CLOCK_REALTIME, &start);
            naive_multiplication(A_matrix,B_matrix,C_matrix);
            clock_gettime(CLOCK_REALTIME, &end);

            free(C_matrix);
            C_matrix = zero_matrix_of_size(i);

            struct timespec naive_time = spec_diff(start, end);
            long int naive_t = (naive_time.tv_sec *1000000000) + naive_time.tv_nsec  ;


            clock_gettime(CLOCK_REALTIME, &start);
            better_multiplication(A_matrix,B_matrix,C_matrix);
            clock_gettime(CLOCK_REALTIME, &end);

            free(C_matrix);
            C_matrix = zero_matrix_of_size(i);

            struct timespec better_time = spec_diff(start, end);
            long int better_t = (better_time.tv_sec *1000000000) + better_time.tv_nsec  ;


            clock_gettime(CLOCK_REALTIME, &start);
            gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,1.0, A_matrix, B_matrix, 0.0, C_matrix);
            clock_gettime(CLOCK_REALTIME, &end);

            free(C_matrix);

            struct timespec gsl_time = spec_diff(start, end);
            long int gsl_t = (gsl_time.tv_sec *1000000000) + gsl_time.tv_nsec  ;





            fprintf(file,"%d,%d,%ld,%ld,%ld\n", i,j + 1, naive_t, better_t,gsl_t);

        }

    }


    return 0;
}