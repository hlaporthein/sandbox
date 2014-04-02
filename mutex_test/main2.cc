#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;
#define thread_nb 10

int ret_status[thread_nb];

void *sayHello(void* p) {
	int* a = (int*) p;
	for (int i = 0; i < 200; i++) {
		{
			pthread_mutex_lock(&mutex);
			printf("Thread_%d: Hello %d!!\n", *a, i);
			pthread_mutex_unlock(&mutex);
		}
	}
	ret_status[*a] = (*a) + 10000;
	return &ret_status[*a];
}

int main() {
	if (pthread_mutex_init(&mutex, NULL) != 0) {
		fprintf(stderr, "Cannot init mutex\n");
		return 1;
	}

	pthread_t threads[thread_nb];
	int id[thread_nb];


	for (int i = 0; i < thread_nb; i++) {
		id[i] = i;
		pthread_create(&threads[i], NULL, sayHello, &id[i]);
	}

	for (int i = 0; i < thread_nb; i++) {
		void *r = &(ret_status[i]);
		pthread_join(threads[i], &r);
		printf("main: thread exit with %d\n", ret_status[i]);
	}

	printf("main: Finished\n");

	if (pthread_mutex_destroy(&mutex) != 0) {
		fprintf(stderr, "Cannot destroy mutex\n");
		return 1;
	}
	return 0;
}
