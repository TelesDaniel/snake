
#include "tonc_types.h"
#include "tonc_memdef.h"
#include "tonc_video.h"
#include "tonc_input.h"
#include <stdlib.h>

int max=100;
int min=1;

void m3_plot_pos(int pos, COLOR clr){	
    vid_mem[pos]= clr;	
}

int generate_fruit(){
    int i = rand() % max;
    int j = rand() % max;
    i = i < min ? i+1 : i;
    j = j < min ? j+1 : j;
    m3_plot(i, j, CLR_RED);
    return i+j*max;
}

void scene() {

    m3_fill(CLR_BLACK);
    int frame=0;
    bool exist_fruit=false;
    int fruit_position=0;
    int dir=0; //0 down, 1 up , 2 left, 3 right
    int x=min;
    int y=min;
    size_t snake_size=5;
    int *snake = NULL;

    snake = (int *) malloc(0 * sizeof(int*));
    m3_frame(min-1, min-1, max+1, max+1, CLR_YELLOW);
    while(1) {
        vid_vsync();
        key_poll();

        if(key_hit(KEY_DOWN) && dir != 1)
            dir = 0;
        else if(key_hit(KEY_UP) && dir != 0)
            dir = 1;
        if(key_hit(KEY_LEFT) && dir != 3)
            dir = 2;
        else if(key_hit(KEY_RIGHT) && dir != 2)
            dir = 3;

        if((frame & 2) == 0) {
            for(int i=snake_size-1; i>=0; i--) 
                m3_plot_pos((int)snake[i], CLR_BLACK);
            
            if(!exist_fruit) {
                m3_plot_pos(fruit_position, CLR_BLACK);
                fruit_position=generate_fruit();
                exist_fruit=true;
            }

            if(dir == 0)
                y = y+1 >= max ? min : y+1;
            else if(dir == 1)
                y = y-1 < min ? max-1 : y-1;
            if(dir == 2)
                x = x-1 < min ? max-1 : x-1;
            else if(dir == 3)
                x = x+1 >= max ? min : x+1;

            if(fruit_position==(x+y*max) && exist_fruit){
                exist_fruit=false;
                snake_size=snake_size+5;
                snake = (int *) realloc(snake, snake_size * sizeof(int*));
            }

            for(int i=snake_size-1; i>=0; i--) {
                if(i==0) {
                    snake[i] = y*M3_WIDTH+x;
                    m3_plot_pos((int)snake[i], CLR_GREEN);
                }else{
                     if(i==(snake_size-1) && snake[i])
                        m3_plot_pos((int)snake[i], CLR_BLACK);

                    snake[i] = snake[i-1];
                    m3_plot_pos((int)snake[i], CLR_GREEN);
                }
            }

            for(int i=1; i<snake_size; i++) {
                if(snake[i] == snake[0])
                    return;
            }
        }
        frame++;
    }
}

int main() {
    REG_DISPCNT=DCNT_MODE3 | DCNT_BG2;

    while(1)
        scene();
    return 0;
}

