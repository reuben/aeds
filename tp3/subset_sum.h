#pragma once

#include <pthread.h>
#include <stdbool.h>

#include "bigint.h"

typedef struct
{
    bigint start;
    bigint end;
    int* values;
    int n;
    int target;
} thread_data_t;

bool global_solution;

void* subset_sum_thread(void* in)
{
    thread_data_t* data = (thread_data_t*)in;

    bool solution = false;

    // for i = 0 to 2^n - 1
    //     testa solução i
    bigint i;
    for (i = data->start; big_lt(i, data->end); i = big_add(i, 1)) {
        if (global_solution) return (void*)0;

        int sum = 0;
        int j;
        for (j = 0; j < data->n; ++j) {
            if (big_bit(i, j)) {
                sum += data->values[j];
            }
        }
        if (sum == data->target) {
            solution = true;
            break;
        }
    }

    return (void*)solution;
}

bool subset_sum(int* values, int n, int target, int thread_count)
{
    bigint subproblems = big_pow_2(n); // 2^n - 1
    pthread_t* threads = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
    thread_data_t* data = (thread_data_t*)malloc(thread_count * sizeof(thread_data_t));

    global_solution = false;

    int i;
    for (i = 1; i <= thread_count; ++i) {
        data[i-1].end = big_sub(big_mul(subproblems, ((double)i / thread_count)), 1);
        data[i-1].start = i == 1 ? big_init(0) : big_add(data[i-2].end, 1);
        data[i-1].values = values;
        data[i-1].n = n;
        data[i-1].target = target;

        pthread_create(&threads[i-1], NULL, subset_sum_thread, (void*)&data[i-1]);
    }

    for (i = 0; i < thread_count; ++i) {
        void* result;
        pthread_join(threads[i], &result);
        if (result) {
            global_solution = true;
        }
    }

    free(threads);
    free(data);

    return global_solution;
}
