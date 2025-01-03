#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define NUM_THREADS 100
#define NUM_ITERATIONS 1000

void *stress_test_thread(void *arg);

int
main(void)
{
    pthread_t threads[NUM_THREADS];
    int i, ret;

    for (i = 0; i < NUM_THREADS; i++) {
        ret = pthread_create(&threads[i], NULL, stress_test_thread, NULL);
        if (ret != 0) {
            fprintf(stderr, "Error creating thread %d: %s\n", i, strerror(errno));
            return EXIT_FAILURE;
        }
    }

    for (i = 0; i < NUM_THREADS; i++) {
        ret = pthread_join(threads[i], NULL);
        if (ret != 0) {
            fprintf(stderr, "Error joining thread %d: %s\n", i, strerror(errno));
            return EXIT_FAILURE;
        }
    }

    printf("Stress test completed successfully.\n");
    return EXIT_SUCCESS;
}

void *
stress_test_thread(void *arg)
{
    int i;
    for (i = 0; i < NUM_ITERATIONS; i++) {
        // Simulate some work
        usleep(1000);
    }
    return NULL;
}
