#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint32_t ElementType;
#define MAXQSIZE 100

typedef struct Queue{
	ElementType queue[MAXQSIZE][3];
	uint16_t front,rear;
	uint16_t Maxsize;
}Queue;

typedef struct Queue* Q_ptr;


//Queue CreateQueue(void);

bool IsFull(Q_ptr Q);

bool IsEmpty(Q_ptr Q);

bool Push(Q_ptr Q,ElementType *x);

ElementType* Pop(Q_ptr Q);