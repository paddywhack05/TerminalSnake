#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#ifdef _WIN32
#include <ncurses/ncurses.h>
#include <windows.h>
#else
#include <ncurses.h>
#include <sys/time.h>
#endif
enum controls{
UP=1,
DOWN,
LEFT,
RIGHT,
};
int snakeSize = 0;
int direction=4;
void resetGameState(int rows, int cols ,int **array){
    int i , j;
for (i = 0; i < cols; i++) {
    array[i] = calloc(rows,sizeof(int)); 
}
return;
}
void freeGameState(int rows, int cols ,int **array){
    int i , j;
for (i = 0; i < cols; i++) {
    free(array[i]);
}
return;
}
void printGameState(int rows, int cols ,int **array){
    erase();
    int i , j;
for (i = 0; i < cols; i++) {
  for (j = 0; j < rows; j++) {
    if (array[i][j]==0){
        printw(".");
    } 
   if (array[i][j]==1){
        printw("#");
    }
    if (array[i][j]==2){
        printw("2");
    }
    if(j==rows-1){
        printw("\n");
    }
  }
}
}
int findNumEmpty(int rows, int cols ,int **array){
int a,b;
int num=0;
//num = rows*cols - snakeSize;
for(a=0; a < cols; a++){
  for(b=0; b<rows;b++){
    if(array[a][b]==0){
        num++;
    }
  }
}
return num;
}

int spawnApple(int rows, int cols ,int **array){
     int emptyPlaces = findNumEmpty(rows, cols ,array);
     if(emptyPlaces==0){
        return 1;
     }
    int rpNum = (rand() % (emptyPlaces - 1 + 1));
    int num = 0;
    for(int i=0; i < cols; i++){
     for(int j=0; j<rows;j++){
        if(array[i][j]==0){
        if(num==rpNum){
            array[i][j]=2;
          }
            num++;
        }
      }
    }
     return 0;
}
void spawnSnake(int rows, int cols ,int **array,int *snakeCords){
    //array[cols/2][rows/4]=1;
    //snakeCords[0]=cols/2;
    //snakeCords[1]=rows/4;
    array[cols/2][rows/4]=1;
    snakeCords[0]=cols/2;
    snakeCords[1]=rows/4;
    snakeSize++;
    array[cols/2][(rows/4)+1]=1;
    snakeCords[2]=cols/2;
    snakeCords[3]=(rows/4)+1;
    snakeSize++;
}
void appendCords(int rows,int cols,int **array,int *oldSnakeCords,int *newSnakeCords){
    int size = snakeSize*2;
    for(int i=0;i<size;i+=2){
        array[oldSnakeCords[i]][oldSnakeCords[i+1]] = 0;
    }
    for(int i=0;i<size;i+=2){
        array[newSnakeCords[i]][newSnakeCords[i+1]] = 1;
    }
    free(oldSnakeCords);
}
void moveSnake(int rows, int cols ,int **array,int *snakeCords,int xT,int yT){
    printw("Moving snake");
    int *oldCords = malloc(sizeof(int)*rows*cols*2);
    memcpy(oldCords,snakeCords,sizeof(int)*snakeSize*2);
    refresh();
    int size = snakeSize*2;
    int headY = snakeSize*2-2;
    int headX = snakeSize*2-1;
    int snakeHeadY = snakeCords[headY];
    int snakeHeadX = snakeCords[headX];
    int nextX = snakeHeadX+xT;
    int nextY = snakeHeadY+yT;
    for(int i=size-2;i>=0;i-=2){
        int tempY=snakeCords[i];
        int tempX=snakeCords[i+1];
        snakeCords[i]=nextY;
        snakeCords[i+1]=nextX;
        nextX=tempX;
        nextY=tempY;
    }
    appendCords(rows,cols,array,oldCords,snakeCords);
}
int main(void) {
        srand(time(NULL));
    int rows=0;
    int columns=0;
       initscr();
       //curs_set(0);
       printw("how many rows");
       int y,x;
       getyx(stdscr,y,x);
       move(y+1,0);
       refresh();
       scanw("%d",&rows);
       printw("how many cols");
       getyx(stdscr,y,x);
       move(y+1,0);
       refresh();
       scanw("%d",&columns);
       nodelay(stdscr,1);
       //raw();
       noecho();
       keypad(stdscr,1);
    if(rows < 5||columns<5){
        printw("to few rows/columns");
        refresh();
        rows=15;
        columns=10;
    }
    int *snakeCords = malloc(sizeof(int)*rows*columns*2);
    int **GameState = malloc(sizeof(int *)*columns);
    if(GameState == NULL){
        printw("malloc is fucked");
        refresh();
        return 1;
    }
    resetGameState(rows,columns,GameState);
    time_t t = time(NULL);
    time_t currentT = time(NULL);
    int num=1;
    spawnSnake(rows,columns,GameState,snakeCords);
    spawnApple(rows,columns,GameState);
    while(1==1){
        currentT = time(NULL);
        if((int)currentT- (int)t == 1){
            t=currentT;
            currentT = time(NULL);
            switch (direction)
            {
            case UP:
            moveSnake(rows,columns,GameState,snakeCords,0,-1);
                break;
            case DOWN:
            moveSnake(rows,columns,GameState,snakeCords,0,1);
                break;
            case LEFT:
            moveSnake(rows,columns,GameState,snakeCords,-1,0);
                break;
            case RIGHT:
            moveSnake(rows,columns,GameState,snakeCords,1,0);
                break;

            default:
                break;
            }
            printGameState(rows,columns,GameState);
            printw("%d\n",num);
            num++;
            refresh();
        }
        int input;
        input = getch();
        if(input){
            //printw("\n Input = %c",input);
        if(input == 'q'||input ==  27){
            break;
        }
        if(input == 'w'||input == KEY_UP){
            direction=UP;
            printw("w");
        }
        if(input == 's'||input == KEY_DOWN){
            direction=DOWN;
            printw("s");
        }
        if(input == 'a'||input == KEY_LEFT){
            direction=LEFT;
            printw("a");
        }
        if(input == 'd'||input == KEY_RIGHT){
            direction=RIGHT;
            printw("d");
        }
                refresh();
        }
    }
        nodelay(stdscr,0);
        printw("\nPress any key to exit \n");
        refresh();
        getch();
        endwin();
        freeGameState(rows,columns,GameState);
        free(snakeCords);
    return 0;
}