#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#define MAX_H 45
#define MAX_W 20
struct Node{
    unsigned int x;
    unsigned int y;
    struct Node * next
} *head, *tail, *food;
unsigned int score = 0;
void clear(){
    puts("\33[2J\33[0;0H");
}
void die(){
    clear();
    printf("得分:\033[0;31;40m%d\33[0m\n", score);
    exit(0);
}
void eat(){
    /* 其实食物也是一个蛇节点，吃掉后添加进来 */
    food->next = head;
    head = food;
    food = (struct Node *)malloc(sizeof(struct Node));
    srand((unsigned)time(NULL));
    food->x = rand()%MAX_W+3;
    food->y = rand()%MAX_H+3;
    score ++;
}
void draw(){
    usleep(120000);
    clear();
    struct Node* li = head;
    /* 遍历蛇身 */
    while(li->next != NULL){
        if(li->x == tail->x && li->y == tail->y)
            die();
        li->x = li->next->x;
        li->y = li->next->y;
        printf("\33[%d;%dH\33[0;46m  \33[0m", li->x, li->y);
        li = li->next;
    }
    /* 绘制食物 */
    printf("\33[%d;%dH\33[0;41m  \33[0m", food->x, food->y);
    if((tail->x == food->x && (tail->y == food->y||tail->y == food->y-1||tail->y == food->y+1) ))
        eat();
}
void draw_head(){
    printf("\33[%d;%dH\33[0;45m  \33[0m\33[0;0H\n", tail->x, tail->y);
}
int main(int argc, char const *argv[]){
    /* 初始化键盘监听 */
    struct termios init_te;
    tcgetattr(0, &init_te);
    init_te.c_lflag&=~ICANON;
    init_te.c_lflag&=~ECHO;
    init_te.c_cc[VMIN]=0;
    init_te.c_cc[VTIME]=0;
    tcsetattr(0, TCSANOW, &init_te);
    /* 游戏初始化 */
    int buf = 0;
    head = (struct Node *)malloc(sizeof(struct Node));
    head->x = 10;
    head->y = 10;
    head->next = NULL;
    tail = head;
    food = (struct Node *)malloc(sizeof(struct Node));
    srand((unsigned)time(NULL));
    food->x = rand()%MAX_W+3;
    food->y = rand()%MAX_H+3;
    printf("按方向键开始游戏\n");
    for(;;){
        /* 监听键盘事件 */
        read(0,&buf,1);
        usleep(20000);
        switch(buf){
            /* 上右下左，绘制蛇身，再绘制移动后的蛇头 */
            case 65:
                draw();
                tail->x--;
                draw_head();
                break;
            case 67:
                draw();
                tail->y+=2;
                draw_head();
                break;
            case 66:
                draw();
                tail->x++;
                draw_head();
                break;
            case 68:
                draw();
                tail->y-=2;
                draw_head();
                break;
        }
    }
    return 0;
}