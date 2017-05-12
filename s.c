#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#define MAX_H 40
#define MAX_W 15
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
    printf("得分:%d\n", score);
    exit(0);
}
void eat(){
    food->next = head;
    head = food;
    food = (struct Node *)malloc(sizeof(struct Node));
    srand((unsigned)time(NULL));
    food->x = rand()%MAX_W+1;
    food->y = rand()%MAX_H+1;
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
        printf("\33[%d;%dH*", li->x, li->y);
        li = li->next;
    }
    printf("\33[%d;%dH+", food->x, food->y);
    if(tail->x == food->x && tail->y == food->y)
        eat();
}
void draw_head(){
    printf("\33[%d;%dH*\33[0;0H\n", tail->x, tail->y);
}
int main(int argc, char const *argv[])
{
    /* 初始化键盘监听 */
    struct termios init_te;
    tcgetattr(0, &init_te);
    init_te.c_lflag&=~ICANON;
    init_te.c_lflag&=~ECHO;
    init_te.c_cc[VMIN]=0;
    init_te.c_cc[VTIME]=0;
    tcsetattr(0, TCSANOW, &init_te);
    int buf = 0;
    head = (struct Node *)malloc(sizeof(struct Node));
    head->x = 10;
    head->y = 10;
    head->next = NULL;
    tail = head;
    food = (struct Node *)malloc(sizeof(struct Node));
    srand((unsigned)time(NULL));
    food->x = rand()%MAX_W+1;
    food->y = rand()%MAX_H+1;
    printf("按方向键开始游戏\n");
    for(;;){
        read(0,&buf,1);
        usleep(20000);
        switch(buf){
            case 65:
                draw();
                tail->x--;
                draw_head();
                break;
            case 67:
                draw();
                tail->y++;
                draw_head();
                break;
            case 66:
                draw();
                tail->x++;
                draw_head();
                break;
            case 68:
                draw();
                tail->y--;
                draw_head();
                break;
        }
    }
    return 0;
}