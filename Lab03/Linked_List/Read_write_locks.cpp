#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

struct list_node_s{
    int data;
    struct list_node_s *next;
};

struct list_node_s *head_p;
pthread_rwlock_t    rwlock;

int member(int val){
    struct list_node_s * curr_p = head_p;
    while(curr_p != NULL && curr_p->data < val) curr_p = curr_p->next;
    if(curr_p == NULL || curr_p->data > val) return 0;
    else return 1;
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
        temp_p = malloc(sizeof (struct list_node_s));
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