#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;

void *sayHello(void* p) {
	for (int i = 0; i < 200; i++) {
		int* a = (int*) p;
		{
			pthread_mutex_lock(&mutex);
			printf("Thread_%d: Hello %d!!\n", *a, i);
			pthread_mutex_unlock(&mutex);
		}
	}
	return NULL;
}

int main() {
	if (pthread_mutex_init(&mutex, NULL) != 0) {
		fprintf(stderr, "Cannot init mutex\n");
		return 1;
	}

	int status = 0;
	int thread_nb = 10;
	pthread_t threads[thread_nb];
	int id[thread_nb];

	for (int i = 0; i < thread_nb; i++) {
		id[i] = i;
		pthread_create(&threads[i], NULL, sayHello, &id[i]);
	}

	for (int i = 0; i < thread_nb; i++) {
		pthread_join(threads[i], (void*)&status);
	}

	if (pthread_mutex_destroy(&mutex) != 0) {
		fprintf(stderr, "Cannot destroy mutex\n");
		return 1;
	}
	return 0;
}
