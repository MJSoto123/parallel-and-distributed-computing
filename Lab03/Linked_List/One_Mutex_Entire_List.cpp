#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

struct list_node_s{
    int data;
    struct list_node_s *next;
};

struct list_node_s *head_p;
pthread_mutex_t list_mutex;

int member(int val){
    struct list_node_s * curr_p = head_p;
    while(curr_p != NULL && curr_p->data < val) curr_p = curr_p->next;
    if(curr_p == NULL || curr_p->data > val)
        return 0;
    else
        return 1;
}
int insert(int val) {
    struct list_node_s* curr_p = head_p;
    struct list_node_s* pred_p;
    struct list_node_s* temp_p;
    while(curr_p != NULL && curr_p->data < val){
        pred_p = curr_p;
        curr_p = curr_p->next;
    }
    if(curr_p == NULL || curr_p->data > val){
        temp_p = new list_node_s();
        temp_p->data = val;
        temp_p->next = curr_p;
        if(pred_p == NULL) 
            head_p = temp_p;
        else
            pred_p->next = temp_p;
        return 1;
    }
    else{
        return 0; 
    }   
}

void *Thread_Work_Insert(void *arg){
    int target = (int)arg;
    pthread_mutex_lock(&list_mutex);

    if(insert(target)) std::cout << "Insertado " << target << "\n";
    else std::cout << "Error al insertar " << target << "\n";
    
    pthread_mutex_unlock(&list_mutex);
    return NULL;
}

void *Thread_Work_Find(void *arg){
    int target = (int)arg;
    pthread_mutex_lock(&list_mutex);
    if(member(target)) std:: cout << "Encontrado " << target << "\n";
    else std::cout << "NO encontrado " << target << "\n"; 
    pthread_mutex_unlock(&list_mutex);

    return NULL;
}

int main(){
    pthread_t a[10];
    for(int i = 0; i < 5; i++){
        pthread_create(&a[i], NULL , &Thread_Work_Insert , (void*)(i));
    }

    for(int i = 5; i < 10; i++){
        pthread_create(&a[i], NULL , &Thread_Work_Find , (void*)(i - 5));
    }
    for(int i = 0; i < 10; i++){
        pthread_join(a[i] , NULL);
    }
    return 0;
}