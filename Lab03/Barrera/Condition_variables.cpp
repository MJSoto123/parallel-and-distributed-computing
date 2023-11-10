#include <iostream> 
#include <pthread.h> 
#include <semaphore.h> 
#define MAX_THREAD 4
using namespace std; 

int counter = 0;
int thread_count = 4;
pthread_mutex_t mutex;
pthread_cond_t cond_var;


void* Thread_work(void* arg) {
    pthread_mutex_lock(&mutex);
    counter++;
    cout << "Worker " << counter << " start\n";

    if (counter == thread_count) {
        cout << "Ultimo worker liberando a los demas\n";
        counter = 0;
        pthread_cond_broadcast(&cond_var);
    } else {
        while (pthread_cond_wait(&cond_var, &mutex) != 0);
    }
    pthread_mutex_unlock(&mutex);
    cout << "Terminando\n";
    return NULL;
}

int main(){
    pthread_t threads[MAX_THREAD]; 
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_var, NULL);
    for (int i = 0; i < MAX_THREAD; i++) 
        pthread_create(&threads[i], NULL, Thread_work, (void*)NULL); 
 
    for (int i = 0; i < MAX_THREAD; i++) 
        pthread_join(threads[i], NULL);
    return 0; 
}