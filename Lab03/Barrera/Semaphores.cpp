#include <iostream> 
#include <pthread.h> 
#include <semaphore.h> 
#define MAX_THREAD 4
using namespace std; 

int counter = 0;
int thread_count = 4;
sem_t count_sem;
sem_t barrier_sem;

void* Thread_work(void* arg){
    sem_wait(&count_sem);
    if (counter == thread_count - 1) {
        cout << "worker " << counter << " liberando a los demas\n";
        counter = 0;
        sem_post(&count_sem);
        for (int j = 0; j < thread_count - 1; j++) sem_post(&barrier_sem);
    } else {
        cout << "Worker " << counter << " esperando!\n";
        counter++;
        sem_post(&count_sem);
        sem_wait(&barrier_sem);
    }
    cout << "Terminando\n";
}

int main(){
    pthread_t threads[MAX_THREAD]; 
    sem_init(&count_sem, 0, 1);
    sem_init(&barrier_sem, 0, 0);
 
    for (int i = 0; i < MAX_THREAD; i++) 
        pthread_create(&threads[i], NULL, Thread_work, (void*)NULL); 
 
    for (int i = 0; i < MAX_THREAD; i++) 
        pthread_join(threads[i], NULL);
    return 0; 
} 