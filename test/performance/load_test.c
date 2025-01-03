#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define NUM_THREADS 50
#define NUM_REQUESTS 500

void *load_test_thread(void *arg);

int
main(void)
{
    pthread_t threads[NUM_THREADS];
    int i, ret;

    for (i = 0; i < NUM_THREADS; i++) {
        ret = pthread_create(&threads[i], NULL, load_test_thread, NULL);
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

    printf("Load test completed successfully.\n");
    return EXIT_SUCCESS;
}

void *
load_test_thread(void *arg)
{
    int i;
    for (i = 0; i < NUM_REQUESTS; i++) {
        // Simulate a request
        usleep(2000);
    }
    return NULL;
}
