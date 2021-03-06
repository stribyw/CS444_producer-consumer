/*
* Shannon Ernst, Tanner Cecchetti, W Keith Striby
* Concurrency Assignment 3, Catergorical Mutual Exclusion
* CS444 - Operating Systems II - Spring 2017
* May 16, 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

/*
* Establish locking variables to control searches,
* insertions, deletions, and search count.
*/
pthread_mutex_t insert_lock;
pthread_mutex_t delete_lock;
pthread_mutex_t search_count_lock;

/*
* Global variables.
*/
struct node *head = NULL;
int search_count = 0;

/*
* Structure for singly linked list.
*/
struct node {
        int val;
        struct node* next;
};

/*
* Adds new nodes to the end of the list.
*/
void push()
{
        struct node* new_node = malloc(sizeof(struct node));
        new_node->val = rand() % 100 + 1;
        new_node->next = NULL;

        if (head == NULL) {
                head = new_node;
        } else {
                struct node* current = head;
                while (current->next != NULL){
                        current = current->next;
                }
                current->next = new_node;
        }
}

/*
* Removes front node from the list.
*/
void pop()
{
        struct node* temp;

        if (head != NULL) {
                temp = head;
                head = head->next;
                free(temp);
        }
}

/*
* Print function.
*/
void print_list()
{
        struct node* current = head;
        while (current != NULL) {
                printf(" %d ", current->val);
                current = current->next;
        }
        printf("\n");
}

/*
* Controlling function for deleting from the list.
*/
void* delete(void* ptr)
{
        pthread_mutex_lock(&insert_lock);
        pthread_mutex_lock(&delete_lock);

        printf("deleting start\n");

        pop();
        print_list();

        printf("deleting finish\n");

        pthread_mutex_unlock(&insert_lock);
        pthread_mutex_unlock(&delete_lock);

        pthread_exit(NULL);
}

/*
* "Searches" list.
*/
void* search(void* ptr)
{
        pthread_mutex_lock(&search_count_lock);
        search_count++;
        printf("searches in progress: %d\n", search_count);
        pthread_mutex_unlock(&search_count_lock);

        struct node* current = head;
        while (current != NULL) {
                pthread_mutex_lock(&delete_lock);
                current = current->next;
                printf("looking at current\n");
                pthread_mutex_unlock(&delete_lock);
        }

        pthread_mutex_lock(&search_count_lock);
        search_count--;
        printf("finished search: %d\n", search_count);
        pthread_mutex_unlock(&search_count_lock);

        pthread_exit(NULL);
}

/*
* Controlling function for inserting into the list.
*/
void* insert(void* ptr)
{
        pthread_mutex_lock(&insert_lock);

        printf("insert start\n");

        push();
        print_list();

        printf("insert finish\n");

        pthread_mutex_unlock(&insert_lock);

        pthread_exit(NULL);
}

int main()
{
        int choice = 0;
        time_t t;

        pthread_mutex_init(&insert_lock, NULL);
        pthread_mutex_init(&delete_lock, NULL);
        pthread_mutex_init(&search_count_lock, NULL);

        srand ((unsigned) time(&t));
        pthread_t thread;

        while (1) {
                choice = rand () % 3 + 1;
                if (choice == 1){
                        pthread_create(&thread, NULL, search, NULL);
                } else if (choice == 2) {
                        pthread_create(&thread, NULL, insert, NULL);
                } else if (choice == 3) {
                        pthread_create(&thread, NULL, delete, NULL);
                }
        }
}
