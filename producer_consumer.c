/*
 * Shannon Ernst, Tanner Cecchetti, W Keith Striby Jr
 * Assignment 1, The Producer-Consumer Problem
 * CS444 - Operating Systems II
 * April 19th, 2017
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "mt19937ar.h"

pthread_t prod_con;
pthread_mutex_t lock;

int buffer = 0;

struct numbers {
   int val;
   int wait;
};

struct numbers list[32];

int rand_num(int min, int max) {
   unsigned int eax;
   unsigned int ebx;
   unsigned int ecx;
   unsigned int edx;

   char vendor[13];

   eax = 0x01;

   __asm__ __volatile__(
      "cpuid;"
      : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
      : "a"(eax)
   );

   if(ecx & 0x40000000){
      unsigned int value;
      __asm__ __volatile__(
         "rdrand %[value];"
         : [value] "=r" (value)
         :
         : "cc"
      );
      return value % (max - min) + min;
   } else {
      return genrand_int32() % (max - min) + min;
   }
}

void add_list() {
   list[buffer].val = rand_num(0, 100);
   list[buffer].wait = rand_num(2, 10);
   printf("adding to list - val: %d, wait: %d\n", list[buffer].val, list[buffer].wait);
   buffer++;
}

struct numbers remove_list() {
   buffer--;
   return list[buffer];
}

void* monitor(void* ptr) {
   while(1) {
      printf("buffer: %d\n", buffer);
      sleep(1);
   }
}

void* producer(void* ptr){
   while(1) {
      pthread_mutex_lock(&lock);      //protect buffer
      if(buffer < 32) {
         add_list();
      }
      pthread_mutex_unlock(&lock);
      sleep(rand_num(3, 8));
   }
}

void* consumer(void* ptr){
   while(1) {
      pthread_mutex_lock(&lock);      //protect buffer
      if(buffer > 0) {
         struct numbers n = remove_list();
         pthread_mutex_unlock(&lock);

         sleep(n.wait);
         printf("val: %d\n", n.val);
      } else {
         pthread_mutex_unlock(&lock);
      }
   }
}

int main(int argc, char* argv[]){
   pthread_t prod;
   pthread_t con;
   pthread_t mon;
   int num_threads = atoi(argv[1]);

   printf("threads: %d\n", num_threads);

   pthread_mutex_init(&lock, NULL);

   //create threads
   for(int i = 0; i < num_threads / 2; i++) {
      pthread_create(&con, NULL, consumer, NULL);
      pthread_create(&prod, NULL, producer, NULL);
   }
   if(num_threads % 2 == 1) {
      pthread_create(&con, NULL, consumer, NULL);
   }

   pthread_create(&mon, NULL, monitor, NULL);

   //wait for other thread to finish
   pthread_join(con, NULL);
}
