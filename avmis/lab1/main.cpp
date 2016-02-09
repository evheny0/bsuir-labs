#include "main.h"

int OUTER_SIZE = 3000;
int INNER_SIZE = 4;
int TO_ALIGN = 32;
typedef float Value_t;


int get_clock_count()
{
    return time(NULL);
}

Value_t **make_new_matrix(bool is_random)
{
    Value_t **matrix = (Value_t **) memalign(TO_ALIGN, OUTER_SIZE * OUTER_SIZE * sizeof(Value_t *));
    for (unsigned i = 0; i < OUTER_SIZE * OUTER_SIZE; i++) {
        matrix[i] = (Value_t *) memalign(TO_ALIGN, INNER_SIZE * INNER_SIZE * sizeof(Value_t));
        for (unsigned j = 0; j < INNER_SIZE * INNER_SIZE; j++) {
            if (is_random) {
                // matrix[i][j] = rand() % 1000;
                matrix[i][j] = 1;
            } else {
                matrix[i][j] = 0;
            }
            
        }
    }
    return matrix;
}

void inline add_to_matrix(Value_t *result, Value_t *matrix_b)
{
    __m128 line_a, line_b, result_line;
    for (unsigned i = 0; i < INNER_SIZE * INNER_SIZE; i += 4) {
        line_a = _mm_load_ps(&(result[i]));
        line_b = _mm_load_ps(&(matrix_b[i]));
        result_line = _mm_add_ps(line_a, line_b);
        _mm_store_ps(&result[i], result_line);
    }
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

void inline mul_matrices(Value_t *matrix_a, Value_t *matrix_b, Value_t *result)
{
    __m128 line_a, line_b, result_line;

    for (unsigned i = 0; i < INNER_SIZE; i++) {
        result_line = _mm_setzero_ps();
        for (unsigned j = 0; j < INNER_SIZE; j++) {
            line_a = _mm_load_ps(&(matrix_b[j * INNER_SIZE]));
            line_b = _mm_set1_ps(matrix_a[j + i * INNER_SIZE]);
            result_line = _mm_add_ps(_mm_mul_ps(line_a, line_b), result_line);
        }

        _mm_store_ps(&result[i * INNER_SIZE], result_line);
    }
}

void multiply_with_manual_optimization(Value_t **matrix_a, Value_t **matrix_b, Value_t **result)
{
    Value_t *temp_small_m = (Value_t *) memalign(TO_ALIGN, INNER_SIZE * INNER_SIZE * sizeof(Value_t));
    /* Outer matrix mul */
    for (unsigned i = 0; i < OUTER_SIZE; i++) {
        printf("%d\n", i);
        
        for (unsigned j = 0; j < OUTER_SIZE; j++) {
            for (unsigned k = 0; k < OUTER_SIZE; k++) {
                // a11*b11 + a12*b21 + a13*b31
                // a11*b11 + a12*b21 + a13*b31
                mul_matrices(matrix_a[k + i * OUTER_SIZE], matrix_b[j + k * OUTER_SIZE], temp_small_m);
                
                add_to_matrix(result[j + i * OUTER_SIZE], temp_small_m);
                // result[j + i * OUTER_SIZE]
                // matrix_b[j + k * OUTER_SIZE] * matrix_a[k + i * OUTER_SIZE]
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    srand(1);

    Value_t **matrix_a = make_new_matrix(true);
    Value_t **matrix_b = make_new_matrix(true);
    Value_t **matrix_result = make_new_matrix(false);

    print_matrix(matrix_a);
    int start_time = get_clock_count();

    multiply_with_manual_optimization(matrix_a, matrix_b, matrix_result);

    int stop_time = get_clock_count();
    printf("%d\n", stop_time - start_time);

    print_matrix(matrix_result);
    return 0;
}
