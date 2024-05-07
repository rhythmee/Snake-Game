#include "m0_matrix_lib.h"
 
 
#define CHROMA  1
#define TRANS   1
#define NO      0
 
unsigned int COLOR[] = {BLACK, RED, GREEN, BLUE, YELLOW, CYAN, VIOLET, WHITE};
unsigned int icon[][2] = {{0x0066FFFF, 0xFF7E3C18}, {0xAAAAAAAA,0xAAAAAAAA}};
 
#define BASE  (500)
enum key_setting{C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2, C3, C3_, D3, D3_, E3, F3, F3_, G3, G3_, A3, A3_, B3, C4, C4_, D4, D4_, E4, F4, F4_, G4, G4_, A4, A4_, B4, C5, C5_, D5, D5_, E5, F5, F5_, G5, G5_, A5, A5_, B5};
enum note{N16=BASE/4, N8=BASE/2, N4=BASE, N2=BASE*2, N1=BASE*4};
 
const int song[][2] ={
    {G1_,N16},{G1_,N16},{G1_,N16},{G1_,N16},/*{G1_,N4},{G1_,N4},{G1_,N4},{G1_,N4},*/
    {A1_,N16},{A1_,N16},{A1_,N16},{G1_,N16},{F1,N16},{F1,N16},{F1,N16},{F1,N16},
    {F1,N16},{G1_,N16},{G1_,N16},{C2,N16},{G1,N16},{G1,N16},{G1,N16},{G1,N16},
    {G1,N16},{G1,N16},{G1,N16},{G1_,N16},{G1_,N16},{G1_,N16},{G1_,N16},{G1_,N16},
    {C1_,N16},{C1_,N16},{C1_,N16},{C1_,N16},{G1_,N16},{G1_,N16},{G1_,N16},{G1_,N16},
};
 
 
//Timer 설정
static unsigned int limit[] = {5, 40};
static unsigned int iter[] = {0, 0};
static unsigned int tflag[] = {0, 0};
 
int lock, key;
 
void chk_timer(void)
{
    int i;
    Timer3_Flag_on = 0;
 
    for (i=0; i<sizeof(limit)/sizeof(limit[0]); i++)
    {
        iter[i] = (iter[i] + 1) % limit[i];
        if(iter[i] == 0) tflag[i] = 1;
    }
}
 
void init_game(void)
{
    RGB_Clr_Screen(BLACK);
 
    Timer3_Period = 100;
    Timer3_Flag_on = 0;
    TIM3_Start();
}
 
 //head position
int main(void)
{
    Uart2_Printf("\n===============");
    Uart2_Printf("\n| Snake Game! |\n");
    Uart2_Printf("===============\n");
 
    //Intro song
//  int i;
//  for (i=0; i<(sizeof(song)/sizeof(song[0])); i++)
//  {
//      Buzzer_Beep(song[i][0], song[i][1]);
//      if (JOYSTICK_Get_Joystick_Pressed()) break;
//  }
 
    int saved_dir = RIGHT;
    int num_fruit=0;
    int num_diet=0;
    int score=0;
 
    Node _node_arr[65];
    node_arr = _node_arr;
 
 
    int map[8][8];
    map_clear(map);
 
    Snake queue;
    queue_init(&queue);
 
    // 시작포인트
    SnakePos init_data = {3, 3};
    enqueue(&queue, init_data);
    map[init_data.y][init_data.x] = SNAKE;
 
    //queue_view(&queue);
 
//  FruitPos fruit;
//  fruit.num_fruit = 0;
 
 
    srand(RTC_Get_Time(&RTC_Time));
 
    init_game();
 
    for(;;)
    {
        if(Timer3_Flag_on == 1)
        {
            chk_timer();
            if (tflag[0])
            {
                int new_x, new_y, check;
 
                set_fruit(map, &num_fruit);
 
 
                //방향반전
                if (saved_dir == PUSH)
                {
                    if (queue.tail == queue.head)
                    {
                        queue.dir = 5-queue.dir;
                    }
 
                    else if (queue.tail->data.x - queue.tail->next->data.x == 0)
                    {
                        if (queue.tail->data.y - queue.tail->next->data.y == 1)
                            queue.dir = DOWN;
                        else
                            queue.dir = UP;
                    }
                    else
                    {
                        if (queue.tail->data.x - queue.tail->next->data.x == 1)
                            queue.dir = RIGHT;
                        else
                            queue.dir = LEFT;
                    }
 
                    reverse_dir(&queue);
                    saved_dir = queue.dir;
                    //Uart2_Printf("saved_dir: %d\n", saved_dir);
                    continue;
                }
 
                queue.dir = saved_dir;
 
                check = check_collision(map, &queue, &new_x, &new_y);
 
                if (check == 1) // wall or snake
                {
                    Uart2_Printf("\nGame Over!\n");
                    Uart2_Printf("\nSCORE: %d\n", score);
                    break;
                }
 
                if (check == 2) // fruit
                {
                    score++;
                    (num_fruit)--;
                    SnakePos new_head = {new_x, new_y};
                    enqueue(&queue, new_head);
                    map[new_y][new_x] = SNAKE;
                    if(Timer3_Period > 30) Timer3_Period -= 1; //snake 가속
                }
 
                if (check == 3) // diet fruit
                {
                    (num_diet)--;
                    SnakePos ps;
                    if (dequeue(&queue, &ps)) // 소멸
                    {
                        Uart2_Printf("\nGame Over!\n");
                        Uart2_Printf("\nSCORE: %d\n", score);
                        break;
                    }
                    else
                    {
                        map[ps.y][ps.x] = 0;
                        snake_move(map, &queue);
                    }
                }
 
                if (check == 0) // no collision
                {
                    snake_move(map, &queue);
                }
 
                visual_snake(map);
                tflag[0] = 0;
            }
 
 
            if (tflag[1])
            {
                set_diet_fruit(map, &num_diet);
                tflag[1] = 0;
            }
        }
 
        key = JOYSTICK_Get_Joystick_Pressed();
 
        if((lock == 0) && (key != 0))
        {
            saved_dir = key;
            lock = 1;
        }
 
        if((lock == 1) && (key == 0))
        {
            lock = 0;
        }
 
    }
 
    int k;
    for(k=0; k<3; k++) // 전체 화면 깜빡깜빡 구현
    {
        TIM_Delay_ms(250);
        RGB_Set_View_Port(8, 8);
        TIM_Delay_ms(250);
        RGB_Set_View_Port(0, 0);
    }
 
    IMG_Num_Disp_2digit(0, 0, score, RED, BLACK);
    return 0;
}
