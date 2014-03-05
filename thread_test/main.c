#include <stdio.h>
#include <pthread.h>

void *sayHello(void* p) {
	for (int i = 0; i < 5; i++) {
		int* a = (int*) p;
		printf("Thread_%d: Hello %d!!\n", *a, i);
	}
	return NULL;
}

int main() {
	int status = 0;
	int thread_nb = 2;
	pthread_t threads[thread_nb];
	int id[thread_nb];

	for (int i = 0; i < thread_nb; i++) {
		id[i] = i;
		pthread_create(&threads[i], NULL, sayHello, &id[i]);
	}

	for (int i = 0; i < thread_nb; i++) {
		pthread_join(threads[i], (void*)&status);
	}

}
