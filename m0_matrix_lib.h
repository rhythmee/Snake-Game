
#include "stm32f0xx.h"
#include <stdlib.h>
#define BLACK   0x000000
#define RED     0x0f0000
#define GREEN   0x000f00
#define BLUE    0x00000f
#define YELLOW  0x0f0f00
#define CYAN    0x000f0f
#define VIOLET  0x0f000f
#define WHITE   0x0f0f0f
#define UP      2
#define DOWN    3
#define LEFT    4
#define RIGHT   1
#define PUSH    5
#define SNAKE   1
#define FRUIT   2
#define DIET    3
 
typedef struct _st{
    int x;
    int y;
} SnakePos;
 
typedef struct _st2{
    int x;
    int y;
    int num_fruit;
} FruitPos;
 
typedef struct _node{
    SnakePos data;
    struct _node *next;
} Node;
 
extern Node *node_arr;
 
typedef struct _snake{
    Node * tail;
    Node * head;
    int dir;
} Snake;
 
extern void queue_init(Snake * pq);
extern int is_empty(Snake *pg);
extern void enqueue(Snake *pq, SnakePos data);
extern int dequeue(Snake *pq, SnakePos *pos);
extern void set_fruit(int map[8][8], int *num_fruit);
extern void set_diet_fruit(int map[8][8], int *num_diet);
extern void map_clear(int map[8][8]);
extern void snake_move(int map[8][8], Snake *pq);
extern void visual_snake(int map[8][8]);
extern void queue_view(Snake *pq);
extern int check_collision(int map[8][8], Snake *pq ,int *a, int *b);
extern Node * prev_node(Node * tail, Node * obj);
extern void reverse_dir(Snake *queue);
