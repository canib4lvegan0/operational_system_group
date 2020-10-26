/***************************************************************************//**
  @file         myshell.c
  @authors      Elivelton Rangel, Nilvan Santana, Matheus Nishimoto, Robson Sousa
  @brief        UNIVERSIDADE FEDERAL DA BAHIA - UFBA
                Projeto prático 04: Coordenação de Tarefas
                Sistemas Operacionais (MAT566)
*******************************************************************************/

#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <malloc.h>
#include <stddef.h>
#include <pthread.h>
#include <string.h>


/* Defines a data basic struct.
 * For simplicity, the data type is 'char' but it could be any other */
typedef struct buffer_elem{
    char data;
    struct buffer_elem *next;   // next queue element
} buffer_elem_t;

/* Defines the buffer structure in the FIFO standard. */
typedef struct{
    unsigned int spaces;           // available spaces
    unsigned int items;            // buffered items
    buffer_elem_t *first;   // first queue element
    buffer_elem_t *last;    // last queue element
} buffer_t;

/* Initializes with empty a buffer. */
void buffer_init(buffer_t *buf, unsigned int size);

/* Return 1 (true) is buffer is empty. 0(false) if not. */
int buffer_is_empty(buffer_t *buf);

/* Return 1 (true) is buffer is full. 0(false) if not. */
int buffer_is_full(buffer_t *buf);

/* Append 'data' to the buffer. (if it's not full) */
void buffer_append(buffer_t *buf, char data);

/* Removes the first element from the queue. (if it's not empty) */
void buffer_remove(buffer_t *buf, char *data);

/* Print all elements of the buffer*/
void buffer_print(buffer_t *buf, const char *name);



#endif //UTILITY_H
