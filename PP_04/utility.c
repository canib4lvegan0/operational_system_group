/***************************************************************************//**
  @file         myshell.c
  @authors      Elivelton Rangel, Nilvan Santana, Matheus Nishimoto, Robson Sousa
  @brief        UNIVERSIDADE FEDERAL DA BAHIA - UFBA
                Projeto prático 04: Coordenação de Tarefas
                Sistemas Operacionais (MAT566)
*******************************************************************************/

#include "utility.h"

/* BUFFER HANDLING FUNCTIONS (QUEUE) */

/* Initializes with empty a buffer. */
void buffer_init(buffer_t *buf, unsigned int size){
    buf->last = buf->first = NULL;
    buf->items = 0;
    buf->spaces = size;
}

/* Return 1 (true) is buffer is empty. 0(false) if not. */
int buffer_is_empty(buffer_t *buf){
    if(buf->items == 0)
        return 1;
    return 0;
}

/* Return 1 (true) is buffer is full. 0(false) if not. */
int buffer_is_full(buffer_t *buf){
    if(buf->spaces == buf->items)
        return 1;
    return 0;
}

/* Append 'data' to the buffer. (if it's not full) */
void buffer_append(buffer_t *buf, const char data){
    if(buffer_is_full(buf)){
        printf("Buffer full!\n");
        return;
    }

    buffer_elem_t *new_elem = malloc(sizeof(buffer_elem_t));

    new_elem->data = data;
    new_elem->next = NULL;
    if(buf->first == NULL)
        buf->first = new_elem;
    else {
        buf->last->next = new_elem;
    }
    buf->items++;
    buf->last = new_elem;
}

/* Removes the first element from the queue. (if it's not empty) */
void buffer_remove(buffer_t *buf, char *data){
    if(buffer_is_empty(buf)){
        printf("Buffer empty!\n");
        return;
    }

    buffer_elem_t *pop_elem = buf->first;
    (*data) = pop_elem->data;

    buf->items--;
    buf->first = buf->first->next;
    free(pop_elem);         // Free up memory
}

/* Print all elements of the buffer*/
void buffer_print(buffer_t *buf, const char *name) {
    printf("[%s] -> ", name);

    for(buffer_elem_t *e = buf->first; e != NULL; e = e->next)
        printf("%c ", e->data);
    printf("\n");
}

/* END BUFFER HANDLING FUNCTIONS (QUEUE) */