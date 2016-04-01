#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#define BLOCKSIZE 128
#define MAX_N 25
#define OFFSET 1024
#define ARRAY_LENGTH  OFFSET * MAX_N



static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

void make_new_array(long * buffer, int block_size, int offset, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < block_size; j++) {
            if (i != n - 1) {
                buffer[i * offset + j] = offset * (i + 1) + j;
                continue;
            }

            if (j + 1 == block_size) {
                buffer[i * offset + j] = 0;
            } else {
                buffer[i * offset + j] = j + 1;
            }            
        }
    }
}

int main()
{
    long *buffer = (long *)calloc((size_t)(ARRAY_LENGTH * sizeof(long)), (size_t)128);
    double result[MAX_N];
    double begin_time = 0.0;
    double end_time = 0.0;
    

    int offset = OFFSET;// / sizeof(int);
    long iterations;

    for (int i = 1; i < MAX_N; i++) {
        int block_size = BLOCKSIZE / (i);    
        long index = 0;
        iterations = block_size * i;

        make_new_array(buffer, block_size, offset, i); 
        
        for(int j = 0, index = 0; j < iterations; j++) {
            index = buffer[index];
        }

        begin_time = rdtsc();      

        for(int j = 0; j < iterations; j++) {
            index = buffer[index];
        }
        end_time = rdtsc();

        result[i] = (end_time - begin_time) / (double)(iterations);
    }

      
    // FILE *file;
    // file = fopen("results.txt","w");
    for (int i = 1; i < MAX_N; i++) {
        printf("%d %lf\r\n", i , result[i]);
        // fprintf(file,"%lf\n", result[i]);
    }
    // fclose(file);
}
