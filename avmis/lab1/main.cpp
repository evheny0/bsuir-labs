#include "main.h"

#define INNER_SIZE 8
#define OUTER_SIZE 200
int TO_ALIGN = 32;
typedef float Value_t;


static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

Value_t **make_new_matrix(bool is_random)
{
    Value_t **matrix = (Value_t **) memalign(TO_ALIGN, OUTER_SIZE * OUTER_SIZE * sizeof(Value_t *));
    for (unsigned i = 0; i < OUTER_SIZE * OUTER_SIZE; i++) {
        matrix[i] = (Value_t *) memalign(TO_ALIGN, INNER_SIZE * INNER_SIZE * sizeof(Value_t));
        for (unsigned j = 0; j < INNER_SIZE * INNER_SIZE; j++) {
            if (is_random) {
                matrix[i][j] = rand() % 1000;
            } else {
                matrix[i][j] = 0;
            }
            
        }
    }
    return matrix;
}

void print_matrix(Value_t **matrix)
{
    printf("%f\n", matrix[rand() % (OUTER_SIZE * OUTER_SIZE)][rand() % (INNER_SIZE * INNER_SIZE)]);
    printf("%f\n", matrix[rand() % (OUTER_SIZE * OUTER_SIZE)][rand() % (INNER_SIZE * INNER_SIZE)]);
    printf("%f\n", matrix[rand() % (OUTER_SIZE * OUTER_SIZE)][rand() % (INNER_SIZE * INNER_SIZE)]);
    printf("%f\n", matrix[rand() % (OUTER_SIZE * OUTER_SIZE)][rand() % (INNER_SIZE * INNER_SIZE)]);
    // for (unsigned i = 0; i < OUTER_SIZE; i++) {
    //     for (unsigned j = 0; j < OUTER_SIZE; j++) {
    //         for (unsigned a = 0; a < INNER_SIZE; a++) {
    //             for (unsigned b = 0; b < INNER_SIZE; b++) {
    //                 printf("%.0f ", matrix[j + i * OUTER_SIZE][b + a * INNER_SIZE]);  
    //             }
    //             printf("\n");
    //         }
    //     }
    // }

}

void compare_matrices(Value_t **matrix_a, Value_t **matrix_b)
{
    for (unsigned i = 0; i < OUTER_SIZE; i++) {
        for (unsigned j = 0; j < OUTER_SIZE; j++) {
            for (unsigned a = 0; a < INNER_SIZE; a++) {
                for (unsigned b = 0; b < INNER_SIZE; b++) {
                    if (matrix_a[j + i * OUTER_SIZE][b + a * INNER_SIZE] != matrix_b[j + i * OUTER_SIZE][b + a * INNER_SIZE]) {
                        exit(1);
                        printf("Wrong\n");
                    }
                }
            }
        }
    }
}

void inline add_to_matrix(Value_t *result, Value_t *matrix_b)
{
    __m256 line_a, line_b, result_line;
    for (unsigned i = 0; i < INNER_SIZE * INNER_SIZE; i += INNER_SIZE) {
        line_a = _mm256_load_ps(&(result[i]));
        line_b = _mm256_load_ps(&(matrix_b[i]));
        result_line = _mm256_add_ps(line_a, line_b);
        _mm256_store_ps(&result[i], result_line);
    }
}

void inline mul_matrices(Value_t *matrix_a, Value_t *matrix_b, Value_t *result)
{
    __m256 line_a, line_b, result_line;

    for (unsigned i = 0; i < INNER_SIZE; i++) {
        result_line = _mm256_setzero_ps();
        for (unsigned j = 0; j < INNER_SIZE; j++) {
            line_a = _mm256_load_ps(&(matrix_b[j * INNER_SIZE]));
            line_b = _mm256_set1_ps(matrix_a[j + i * INNER_SIZE]);
            result_line = _mm256_add_ps(_mm256_mul_ps(line_a, line_b), result_line);
        }
        _mm256_store_ps(&result[i * INNER_SIZE], result_line);
    }
}

void multiply_with_manual_optimization(Value_t **matrix_a, Value_t **matrix_b, Value_t **result)
{
    Value_t *temp_small_m = (Value_t *) memalign(TO_ALIGN, INNER_SIZE * INNER_SIZE * sizeof(Value_t));
    /* Outer matrix mul */
    for (unsigned i = 0; i < OUTER_SIZE; i++) {
        // #pragma omp parallel for
        for (unsigned j = 0; j < OUTER_SIZE; j++) {
            for (unsigned k = 0; k < OUTER_SIZE; k++) {
                mul_matrices(matrix_a[k + i * OUTER_SIZE], matrix_b[j + k * OUTER_SIZE], temp_small_m);
                add_to_matrix(result[j + i * OUTER_SIZE], temp_small_m);
            }
        }
    }
}

void inline simple_add_to_matrix(Value_t *result, Value_t *matrix_b)
{
    for (unsigned i = 0; i < INNER_SIZE * INNER_SIZE; i++) {
        result[i] += matrix_b[i];
    }
}

void inline simple_mul_matrices(Value_t *matrix_a, Value_t *matrix_b, Value_t *result)
{
    // for (unsigned i = 0; i < INNER_SIZE; i++) {
    //     for (unsigned j = 0; j < INNER_SIZE; j++) {
    //         for (unsigned k = 0; k < INNER_SIZE; k++) {
    //             result[k + i * INNER_SIZE] += matrix_b[k + j * INNER_SIZE] * matrix_a[j + i * INNER_SIZE];
    //         }
    //     }
    // }
    memset(result, 0, sizeof(Value_t) * INNER_SIZE * INNER_SIZE);

    Value_t temp_a, *temp_resutl, *temp_b;
    for (int a = 0; a < INNER_SIZE; a++) {
        for (int b = 0; b < INNER_SIZE; b++) {
            temp_a = matrix_a[a * INNER_SIZE + b];
            temp_resutl = result + a * INNER_SIZE;
            temp_b = matrix_b + b * INNER_SIZE;
            for (int k = 0; k < INNER_SIZE; k++) {
                temp_resutl[k] += temp_a * temp_b[k];
            }
        }
    }
}

void multiply_simple(Value_t **matrix_a, Value_t **matrix_b, Value_t **result)
{
    Value_t *temp_small_m = (Value_t *) memalign(TO_ALIGN, INNER_SIZE * INNER_SIZE * sizeof(Value_t));
    /* Outer matrix mul */
    Value_t temp_a, *temp_resutl, *temp_b;
    for (unsigned i = 0; i < OUTER_SIZE; i++) {
        for (unsigned j = 0; j < OUTER_SIZE; j++) {
            for (unsigned k = 0; k < OUTER_SIZE; k++) {                
                memset(temp_small_m, 0, sizeof(Value_t) * INNER_SIZE * INNER_SIZE);
                for (int a = 0; a < INNER_SIZE; a++) {
                    for (int b = 0; b < INNER_SIZE; b++) {
                        temp_a = matrix_a[k + i * OUTER_SIZE][a * INNER_SIZE + b];
                        temp_resutl = temp_small_m + a * INNER_SIZE;
                        temp_b = matrix_b[j + k * OUTER_SIZE] + b * INNER_SIZE;
                        for (int k = 0; k < INNER_SIZE; k++) {
                            temp_resutl[k] += temp_a * temp_b[k];
                        }
                    }
                }
                for (unsigned a = 0; a < INNER_SIZE * INNER_SIZE; a++) {
                    result[j + i * OUTER_SIZE][a] += temp_small_m[a];
                }

            }
        }
    }
}

int main(int argc, char const *argv[])
{
    srand(1);

    long long clock_count;
    double start_time, end_time;
    struct timeval start;

    Value_t **matrix_a = make_new_matrix(true);
    Value_t **matrix_b = make_new_matrix(true);
    Value_t **matrix_result = make_new_matrix(false);
    Value_t **matrix_result_simple = make_new_matrix(false);


    gettimeofday(&start, NULL);
    clock_count = rdtsc();
    start_time = (double)(start.tv_sec + start.tv_usec/1000000.0);

    multiply_with_manual_optimization(matrix_a, matrix_b, matrix_result);

    clock_count = rdtsc() - clock_count;
    gettimeofday(&start, NULL);
    end_time = (double)(start.tv_sec + start.tv_usec/1000000.0);
    printf(" -- Manual optimization:\n");
    printf("Clock cycle count is %llu\n", clock_count);
    printf("Execution time is %2.2f seconds\n", end_time - start_time);


    gettimeofday(&start, NULL);
    clock_count = rdtsc();
    start_time = (double)(start.tv_sec + start.tv_usec/1000000.0);

    multiply_simple(matrix_a, matrix_b, matrix_result_simple);

    clock_count = rdtsc() - clock_count;
    gettimeofday(&start, NULL);
    end_time = (double)(start.tv_sec + start.tv_usec/1000000.0);
    printf("\n -- No optimization:\n");
    printf("Clock cycle count is %llu\n", clock_count);
    printf("Execution time is %2.3f seconds\n", end_time - start_time);

    // print_matrix(matrix_result);
    // print_matrix(matrix_result_simple);
    compare_matrices(matrix_result, matrix_result_simple);
    return 0;
}
