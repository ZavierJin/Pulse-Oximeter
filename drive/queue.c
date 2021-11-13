#include "queue.h"

//Queue CreateQueue(){
//	Queue q;
//	q.front = 0;
//	q.rear = 0;
//	return q;
//}

bool IsFull(Q_ptr Q)
{
	return ((Q->rear+1)%(Q->Maxsize) == Q->front);
}

bool IsEmpty(Q_ptr Q)
{
    return (Q->front == Q->rear);
}

bool Push(Q_ptr Q,ElementType *x){
	if(IsFull(Q))
		return false;
	else{
		Q->rear = (Q->rear+1)%(Q->Maxsize);
		Q->queue[Q->rear][0] = x[0];
		Q->queue[Q->rear][1] = x[1];
		Q->queue[Q->rear][2] = x[2];
		return true;
	}
}


ElementType* Pop(Q_ptr Q){
	Q->front =(Q->front+1)%(Q->Maxsize);
	return  Q->queue[Q->front];
}
		


