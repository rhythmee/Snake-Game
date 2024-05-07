
#include "m0_matrix_lib.h"
 
Node *node_arr;
 
void queue_init(Snake * pq)
{
    pq->head = NULL;
    pq->tail = NULL;
    pq->dir = RIGHT;
 
    // node_arr 초기화
    int i;
    for (i=0; i<65; i++)
    {
        node_arr[i].data.x = -1;
    }
}
 
int is_empty(Snake *pg)
{
    if (pg->head == NULL)
        return 1;
    else
        return 0;
}
 
void enqueue(Snake *pq, SnakePos data) // 뱀 노드 추가
{
    //Node * newNode = (Node *) malloc(sizeof(Node));
    int i;
    Node * newNode;
    for (i=0; i<65; i++)
    {
        if (node_arr[i].data.x == -1)
        {
            newNode = &node_arr[i];
            break;
        }
    }
    Uart2_Printf("[ENQUEUE] Node alloc: %d\n", i);
 
    newNode->data = data;
    newNode->next = NULL;
 
    if (pq->head == NULL) // empty queue
    {
        pq->tail = newNode;
        pq->head = newNode;
    }
    else
    {
        pq->head->next = newNode; //link 연결
        pq->head = newNode; //head 지정
    }
}
 
 
int dequeue(Snake *pq, SnakePos *pos)
{
    if (pq->tail == pq->head)
        return 1;
 
    //save tail pos
    (*pos).x = pq->tail->data.x;
    (*pos).y = pq->tail->data.y;
 
    // mem free
    int i;
    for (i=0; i<65; i++)
    {
        if (&node_arr[i] == pq->tail)
        {
            node_arr[i].data.x = -1;
            break;
        }
    }
 
    // tail node 제거
    Node * newTail = pq->tail->next;
    pq->tail = newTail;
    Uart2_Printf("[DEQUEUE] Node free: %d\n", i);
 
    return 0;
}
 
void snake_move(int map[8][8], Snake *pq)
{
    map[pq->tail->data.y][pq->tail->data.x] = 0;
    switch(pq->dir)
    {
        case RIGHT:
        {
            if (pq->head->data.x == 7) break;
            else // 꼬리부터 한칸씩 앞으로 이동
            {
                Node * p = pq->tail;
                Node * n = pq->tail->next;
 
                for (;;)
                {
                    if (p->next == NULL) // head 까지 읽음
                    {
                        (pq->head->data.x)++; //head 이동
                        break;
                    }
                    else // 이건 공통으로 묶어줄수도?
                    {
                        p->data.x = n->data.x;
                        p->data.y = n->data.y;
 
                        p = n;
                        n = n->next;
                    }
                }
                break;
            }
        }
 
        case UP:
        {
            if (pq->head->data.y == 0) break;
            else
            {
                Node * p = pq->tail;
                Node * n = pq->tail->next;
                for (;;)
                {
                    if (p->next == NULL)
                    {
                        (pq->head->data.y)--;
                        break;
                    }
                    else
                    {
                        p->data.x = n->data.x;
                        p->data.y = n->data.y;
 
                        p = n;
                        n = n->next;
                    }
                }
                break;
            }
        }
 
        case DOWN:
        {
            if (pq->head->data.y == 7) break;
            else
            {
                Node * p = pq->tail;
                Node * n = pq->tail->next;
                for (;;)
                {
                    if (p->next == NULL)
                    {
                        (pq->head->data.y)++;
                        break;
                    }
                    else
                    {
                        p->data.x = n->data.x;
                        p->data.y = n->data.y;
 
                        p = n;
                        n = n->next;
                    }
                }
                break;
            }
        }
 
        case LEFT:
        {
            if (pq->head->data.x == 0) break;
            else
            {
                Node * p = pq->tail;
                Node * n = pq->tail->next;
                for (;;)
                {
                    if (p->next == NULL)
                    {
                        (pq->head->data.x)--;
                        break;
                    }
                    else
                    {
                        p->data.x = n->data.x;
                        p->data.y = n->data.y;
 
                        p = n;
                        n = n->next;
                    }
                }
                break;
            }
        }
    }
 
    map[pq->head->data.y][pq->head->data.x] = SNAKE;
}
 
void visual_snake(int map[8][8])
{
    int x, y;
    for (y=0; y<8; y++)
    {
        for (x=0; x<8; x++)
        {
            //Uart2_Printf("0x%0.8x\n", RGB_Get_Pixel(x,y));
            if (RGB_Get_Pixel(x,y) == GREEN) RGB_Put_Pixel(x,y,BLACK);
            if (map[y][x]==SNAKE) RGB_Put_Pixel(x,y,GREEN);
        }
    }
}
 
void set_fruit(int map[8][8], int *num_fruit)
{
    srand(RTC_Get_Time(&RTC_Time));
 
    int x, y;
    x = rand() % 8;
    y = rand() % 8;
 
    if (map[y][x] == 0 && *num_fruit < 3) // empty
    {
        map[y][x] = FRUIT;
        RGB_Put_Pixel(x, y, CYAN);
        (*num_fruit)++;
    }
}
 
void set_diet_fruit(int map[8][8], int *num_diet)
{
    srand(RTC_Get_Time(&RTC_Time));
 
    int x, y;
    x = rand() % 8;
    y = rand() % 8;
 
    if (map[y][x] == 0 && *num_diet < 1) // empty
    {
        map[y][x] = DIET;
        RGB_Put_Pixel(x, y, YELLOW);
        (*num_diet)++;
    }
}
 
 
// 2차원 배열 map[8][8]
void map_clear(int map[8][8])
{
    int i, j;
    for (i=0; i<8; i++)
    {
        for (j=0; j<8; j++)
        {
            map[i][j] = 0;
        }
    }
}
 
void queue_view(Snake *pq)
{
 
    int cnt=0;
    Node * current = pq->tail;
    for (;;)
    {
        Uart2_Printf("<Tail node + %d>\n", cnt);
        Uart2_Printf("x: %d\n", current->data.x);
        Uart2_Printf("y: %d\n", current->data.y);
        Uart2_Printf("\n");
 
        if (current == pq->head) break; // head 까지 읽음
        else current = current->next;
    }
}
 
int check_collision(int map[8][8], Snake *pq ,int *a, int *b) //check before move, 1==game over
{
 
    switch(pq->dir)
    {
        case RIGHT:
        {
            if (pq->head->data.x == 7) return 1; // wall
            *a = pq->head->data.x + 1;
            *b = pq->head->data.y;
            break;
        }
        case UP:
        {
            if (pq->head->data.y == 0) return 1; // wall
            *a = pq->head->data.x;
            *b = pq->head->data.y - 1;
            break;
        }
        case DOWN:
        {
            if (pq->head->data.y == 7) return 1; // wall
            *a = pq->head->data.x;
            *b = pq->head->data.y + 1;
            break;
        }
        case LEFT:
        {
            if (pq->head->data.x == 0) return 1; // wall
            *a = pq->head->data.x - 1;
            *b = pq->head->data.y;
            break;
        }
    }
    return map[*b][*a];
}
 
Node * prev_node(Node * tail, Node * obj) //obj node의 prev node 리턴
{
    Node * current = tail;
    for (;;)
    {
 
        if (current->next == obj)
            return current;
        else
        {
            current = current->next;
        }
    }
}
 
void reverse_dir(Snake *queue) // head 부터 link 반전
{
    Node * current = queue->head;
    for(;;)
    {
        if (current == queue->tail)
        {
            current->next = NULL;
            break;
        }
        current->next = prev_node(queue->tail, current);
        current = current->next;
    }
    current = queue->tail;
    queue->tail = queue->head;
    queue->head = current;
}
