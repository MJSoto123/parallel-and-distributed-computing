#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

struct list_node_s {
   int    data;
   pthread_mutex_t mutex;
   struct list_node_s* next;
};

struct list_node_s* head = NULL;
pthread_mutex_t head_mutex;

int member(int value) {
    struct list_node_s *temp, *old_temp;
    pthread_mutex_lock(&head_mutex);
    temp = head;
    if (temp != NULL) pthread_mutex_lock(&(temp->mutex));
    pthread_mutex_unlock(&head_mutex);
    while(temp != NULL && temp->data < value){
        if (temp->next != NULL) 
            pthread_mutex_lock(&(temp->next->mutex));
        old_temp = temp;
        temp = temp->next;
        pthread_mutex_unlock(&(old_temp->mutex));
    }

    if(temp == NULL || temp->data > value){
        if (temp != NULL) 
            pthread_mutex_unlock(&(temp->mutex));
        return 0;
    }
    else { 
        pthread_mutex_unlock(&(temp->mutex));
        return 1;
    }
} 
int Advance_ptrs(struct list_node_s** curr_pp, struct list_node_s** pred_pp) {
    int rv = 1;
    struct list_node_s* curr_p = *curr_pp;
    struct list_node_s* pred_p = *pred_pp;

    if (curr_p == NULL) {
        if (pred_p == NULL) {
            pthread_mutex_unlock(&head_mutex);
            return -1;
        } else {  
            return 0;
        }
    } else { 
        if (curr_p->next != NULL)
            pthread_mutex_lock(&(curr_p->next->mutex));
        else
            rv = 0;
        if (pred_p != NULL)
            pthread_mutex_unlock(&(pred_p->mutex));
        else
            pthread_mutex_unlock(&head_mutex);
        *pred_pp = curr_p;
        *curr_pp = curr_p->next;
        return rv;
    }
}  
void Init_ptrs(struct list_node_s** curr_pp, struct list_node_s** pred_pp) {
    *pred_pp = NULL;
    pthread_mutex_lock(&head_mutex);
    *curr_pp = head;
    if(*curr_pp != NULL)
        pthread_mutex_lock(&((*curr_pp)->mutex));
}
int insert(int value) {
    struct list_node_s* curr;
    struct list_node_s* pred;
    struct list_node_s* temp;
    int rv = 1;

    Init_ptrs(&curr, &pred);
    
    while (curr != NULL && curr->data < value) {
        Advance_ptrs(&curr, &pred);
    }

    if (curr == NULL || curr->data > value) {
        temp = new list_node_s();
        pthread_mutex_init(&(temp->mutex), NULL);
        temp->data = value;
        temp->next = curr;
        if (curr != NULL) 
            pthread_mutex_unlock(&(curr->mutex));
        if (pred == NULL) {
            head = temp;
            pthread_mutex_unlock(&head_mutex);
        } else {
            pred->next = temp;
            pthread_mutex_unlock(&(pred->mutex));
        }
    } else { 
        if (curr != NULL) 
            pthread_mutex_unlock(&(curr->mutex));
        if (pred != NULL)
            pthread_mutex_unlock(&(pred->mutex));
        else
            pthread_mutex_unlock(&head_mutex);
        rv = 0;
    }

    return rv;
}

void *Thread_Work_Insert(void *arg){
    int target = (int)arg;

    if(insert(target)) std::cout << "Insertado " << target << "\n";
    else std::cout << "Error al insertar " << target << "\n";

    return NULL;
}

void *Thread_Work_Find(void *arg){
    int target = (int)arg;

    if(member(target)) std:: cout << "Encontrado " << target << "\n";
    else std::cout << "NO encontrado " << target << "\n"; 

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