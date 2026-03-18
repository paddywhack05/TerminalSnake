#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#ifdef _WIN32
#include <ncurses/ncurses.h>
#include <windows.h>
#else
#include <ncurses.h>
#endif
long uniTime(){
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC,&t);
    long millisec = t.tv_sec*1000;
    long milli = t.tv_nsec/1e+6;
    return millisec + milli;
}
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
char getHeadChar(){
    switch (direction)
    {
    case UP:
        return '^';
        break;
    case DOWN:
        return 'v';
        break;
    case LEFT:
        return '<';
        break;
    case RIGHT:
        return '>';
        break;                    
    default:
        return '@';
        break;
    }
}
void printGameState(WINDOW *pSnakeWin,int rows, int cols ,int **array,int *snakeCords){
    werase(pSnakeWin);
    int i , j;
    char snakeHead=getHeadChar();//<>v^
for (i = 0; i < cols; i++) {
  for (j = 0; j < rows; j++) {
    wmove(pSnakeWin,i+1,j+1);
    if (array[i][j]==0){
        wprintw(pSnakeWin,".");
    } 
   if (array[i][j]==1){
        if(i==snakeCords[snakeSize*2-2]&&j==snakeCords[snakeSize*2-1]){
            wprintw(pSnakeWin,"%c",snakeHead);
        }else{
        wprintw(pSnakeWin,"#");
        }
    }
    if (array[i][j]==2){
        wprintw(pSnakeWin,"2");
    }
    if(j==rows-1){
        wprintw(pSnakeWin,"\n");
    }
  }
}
box(pSnakeWin,0,0);
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
    array[cols/2][rows/4]=1;
    snakeCords[0]=cols/2;
    snakeCords[1]=rows/4;
    snakeSize++;
}
void appendCords(int rows,int cols,int **array,int *oldSnakeCords,int *newSnakeCords,int sizeReduce){
    int size = (snakeSize-sizeReduce)*2;
    for(int i=0;i<size;i+=2){
        array[oldSnakeCords[i]][oldSnakeCords[i+1]] = 0;
    }
    for(int i=0;i<size;i+=2){
        array[newSnakeCords[i]][newSnakeCords[i+1]] = 1;
    }
    free(oldSnakeCords);
}
void eatApple(int rows, int cols,int **array,int *snakeCords,int *oldCords,int nextX,int nextY){
    int xIndex = (snakeSize*2)+1;
    int yIndex = (snakeSize*2);
    snakeCords[yIndex] = nextY;
    snakeCords[xIndex] = nextX;
    snakeSize++;
    array[snakeCords[yIndex]][snakeCords[xIndex]] = 1;
    appendCords(rows,cols,array,oldCords,snakeCords,1);
    spawnApple(rows,cols,array);
}
int moveSnake(int rows, int cols ,int **array,int *snakeCords,int xT,int yT){
    int *oldCords = malloc(sizeof(int)*rows*cols*2);
    memcpy(oldCords,snakeCords,sizeof(int)*snakeSize*2);
    int size = snakeSize*2;
    int headY = snakeSize*2-2;
    int headX = snakeSize*2-1;
    int snakeHeadY = snakeCords[headY];
    int snakeHeadX = snakeCords[headX];
    int nextX = snakeHeadX+xT;
    int nextY = snakeHeadY+yT;
    if(nextX<0||nextY<0){
        return -1;
    }
    if(nextX>rows-1||nextY>cols-1){
        return -1;
    }
    if(snakeCords[0]==nextY&&snakeCords[1]==nextX){
        //printw("tail hit");
        //refresh();
    }else if(array[nextY][nextX]==1){
        return -1;
    }
    if(array[nextY][nextX]==2){
        eatApple(rows,cols,array,snakeCords,oldCords,nextX,nextY);
        return 1;
    }
    for(int i=size-2;i>=0;i-=2){
        int tempY=snakeCords[i];
        int tempX=snakeCords[i+1];
        snakeCords[i]=nextY;
        snakeCords[i+1]=nextX;
        nextX=tempX;
        nextY=tempY;
    }
    appendCords(rows,cols,array,oldCords,snakeCords,0);
}
int addInput(int bufferIndex,int *inputBuffer,int direct,int oppositeDirect){
    if(snakeSize>1&&bufferIndex > 0){
        if(inputBuffer[bufferIndex-1]==oppositeDirect){
            return 0;
        }
    }
    if((bufferIndex > 3 || (bufferIndex > 0 && inputBuffer[bufferIndex - 1] == direct))){
        return 0;
    }else{
        inputBuffer[bufferIndex]=direct;
        return 1;
    }
}
int main(void) {
        srand(time(NULL));
    int rows=0;
    int columns=0;
       initscr();
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
    erase();
    refresh();
        WINDOW *pSnakeWin = newwin(columns+2,rows+2,0,0);
    int *snakeCords = malloc(sizeof(int)*rows*columns*2);
    int **GameState = malloc(sizeof(int *)*columns);
    if(GameState == NULL){
        printw("malloc is fucked");
        refresh();
        return 1;
    }
    resetGameState(rows,columns,GameState);
    long t = uniTime();
    long currentT = uniTime();
    int num=1;
    int tickRate=300;
    spawnSnake(rows,columns,GameState,snakeCords);
    spawnApple(rows,columns,GameState);
    int bufferIndex=0;
    int inputBuffer[4]={0};
    while(1==1){
        currentT = uniTime();
        if(currentT - t >= tickRate){
            t=currentT;
            currentT = time(NULL);
            int code;
            if(bufferIndex==0){
                
            }else{
                direction = inputBuffer[0];
                for(int i = 0;i<bufferIndex-1;i++){
                    inputBuffer[i]=inputBuffer[i+1];
                }
                bufferIndex--;
            }
            switch (direction)
            {
            case UP:
            code = moveSnake(rows,columns,GameState,snakeCords,0,-1);
                break;
            case DOWN:
            code = moveSnake(rows,columns,GameState,snakeCords,0,1);
                break;
            case LEFT:
            code = moveSnake(rows,columns,GameState,snakeCords,-1,0);
                break;
            case RIGHT:
            code = moveSnake(rows,columns,GameState,snakeCords,1,0);
                break;

            default:
                break;
            }
            if(code==-1){
                break;
            }
            printGameState(pSnakeWin,rows,columns,GameState,snakeCords);
            move(columns+2,0);
            printw("%d\n",num);
            num++;
            wrefresh(pSnakeWin);
        }
        int input;
        input = getch();
        if(input){
            //printw("\n Input = %c",input);
            move(columns+3,0);
        if(input == 'q'||input ==  27){
            break;
        }
        if(input == 'p'){
            int y,x;
            nodelay(stdscr,0);
            WINDOW *pPauseWin = newwin(columns/2,rows/2,columns/3,rows/3);
            getmaxyx(pPauseWin,y,x);
            mvwprintw(pPauseWin,y/2,x/2-1,"||");
            box(pPauseWin,0,0);
            wrefresh(pPauseWin);
            getch();
            delwin(pPauseWin);
            nodelay(stdscr,1);
        }
        if(input == 'w'||input == KEY_UP){
            if(snakeSize>1&&bufferIndex == 0&&direction==DOWN){

            }else{
                bufferIndex += addInput(bufferIndex,inputBuffer,UP,DOWN);
            }
            printw("w,%d",bufferIndex);
        }
        if(input == 's'||input == KEY_DOWN){
            if(snakeSize>1&&bufferIndex == 0&&direction==UP){

            }else{
                bufferIndex += addInput(bufferIndex,inputBuffer,DOWN,UP);
            }
            printw("s,%d",bufferIndex);
        }
        if(input == 'a'||input == KEY_LEFT){
            if(snakeSize>1&&bufferIndex == 0&&direction==RIGHT){

            }else{
            bufferIndex += addInput(bufferIndex,inputBuffer,LEFT,RIGHT);
            }
            printw("a,%d",bufferIndex);
        }
        if(input == 'd'||input == KEY_RIGHT){
            if(snakeSize>1&&bufferIndex == 0&&direction==LEFT){

            }else{
                bufferIndex += addInput(bufferIndex,inputBuffer,RIGHT,LEFT);
            }
            printw("d,%d",bufferIndex);
        }
                refresh();
        }
    }
        nodelay(stdscr,0);
        erase();
        printw("\nPress any key to exit \n");
        refresh();
        getch();
        endwin();
        freeGameState(rows,columns,GameState);
        free(snakeCords);
        free(GameState);
    return 0;
}