#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#define MAX_SIZE 5000

typedef struct {
    uint16_t marker;
    uint16_t x;
    uint16_t y;
} MarkerCord;

typedef struct {
    MarkerCord items[MAX_SIZE];
    int front;
    int rear;
    int size;
} Queue;

void initializeQueue(Queue* q) {
    q->front = 0;
    q->rear = 0;
    q->size = 0;
}

bool isEmpty(Queue* q) { return q->size == 0; }
bool isFull(Queue* q) { return q->size == MAX_SIZE; }

//adds an element at the back of the queue
void enqueue(Queue* q, MarkerCord cord) {
    if (isFull(q)) {
        printf("Queue is full\n");
        return;
    }
    q->items[q->rear] = cord;
    q->rear = (q->rear + 1) % MAX_SIZE;
    q->size++;
}

// Removes first element
void dequeue(Queue* q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }
    q->front = (q->front + 1) % MAX_SIZE;
    q->size--;
}

// Returns the first element of the queue
MarkerCord peek(Queue* q) {
    MarkerCord empty = {0,0,0};
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return empty;
    }
    return q->items[q->front];
}

// Prints the queue
void printQueue(Queue* q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }
    printf("Current Queue:\n");
    for (int i = 0; i < q->size; i++) {
        int idx = (q->front + i) % MAX_SIZE;
        printf("Marker: %u, X: %u, Y: %u\n",
               q->items[idx].marker,
               q->items[idx].x,
               q->items[idx].y);
    }
}

// int main() {
//     Queue q;
//     initializeQueue(&q);
// 
//     MarkerCord c1 = {1, 10, 20};
//     MarkerCord c2 = {2, 30, 40};
//     MarkerCord c3 = {3, 50, 60};
//     MarkerCord c4 = {4, 70, 80};
//     MarkerCord c5 = {5, 90, 100};
// 
//     enqueue(&q, c1);
//     enqueue(&q, c2);
//     enqueue(&q, c3);
//     enqueue(&q, c4);
// 
//     printQueue(&q);
// 
//     dequeue(&q);
//     enqueue(&q, c5); // reuses space at the start
//     printQueue(&q);
// 
//     return 0;
// }
