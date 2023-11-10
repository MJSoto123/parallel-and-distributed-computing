#include <iostream> 
#include <pthread.h> 
#define MAX_THREAD 4
using namespace std; 

int counter = 0;
int thread_count = 4;
pthread_mutex_t barrier_mutex;
int do_nothing = 0;

void* Thread_work(void* arg) {
    pthread_mutex_lock(&barrier_mutex);
    counter++;

    cout << "Worker " << counter << " start!\n";
    for(int i = 0; i < 1e4; i++) do_nothing++;
    std::cout << "Worker " << counter << " finish!\n";
    
    pthread_mutex_unlock(&barrier_mutex);
    while (counter < thread_count); // Esperando al ultimo hilo
    cout << "Terminando\n";
}

int main(){
    pthread_t threads[MAX_THREAD]; 
 
    for (int i = 0; i < MAX_THREAD; i++) 
        pthread_create(&threads[i], NULL, Thread_work, (void*)NULL); 
 
    for (int i = 0; i < MAX_THREAD; i++) 
        pthread_join(threads[i], NULL);
    return 0; 
} 