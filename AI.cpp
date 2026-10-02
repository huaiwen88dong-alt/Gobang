#include "AI.h"


Move AI::getMove(Board& board)
{

    Move bestMove;

    int bestScore=-1;


    for(int i=0;i<Board::BOARD_SIZE;i++)
    {
        for(int j=0;j<Board::BOARD_SIZE;j++)
        {

            if(board.getChess(i,j)==0)
            {

                int score=evaluatePosition(board,i,j);


                if(score>bestScore)
                {
                    bestScore=score;

                    bestMove.row=i;

                    bestMove.col=j;

                    bestMove.player=2;
                }

            }

        }
    }


    return bestMove;
}
//计算某个方向连续棋子数量
int AI::countDirection(Board& board,int row,int col,int dx,int dy,int player)
{
    int count=0;
    int x=row+dx;
    int y=col+dy;
    while(x>=0 &&x<Board::BOARD_SIZE &&y>=0 &&y<Board::BOARD_SIZE)
    {
        if(board.getChess(x,y)==player)
        {
            count++;
        }
        else
        {
            break;
        }
        x+=dx;
        y+=dy;
    }
    return count;
}
int AI::evaluatePosition(Board& board,int row,int col)
{

    //AI攻击分
    int attack =evaluatePlayer(board,row,col,2);


    //阻止玩家分
    int defend =evaluatePlayer(board,row,col,1);


    return attack + defend*1.2;

}
int AI::evaluatePlayer(Board& board,int row,int col,int player)
{
    board.placeChess(row,col,player);


    int score=0;


    int directions[4][2]=
    {
        {1,0},
        {0,1},
        {1,1},
        {1,-1}
    };


    for(int i=0;i<4;i++)
    {

        int dx=directions[i][0];
        int dy=directions[i][1];


        int count1=countDirection(board,row,col,dx,dy,player);


        int count2=countDirection(board,row,col,-dx,-dy,player);


        int total=count1+count2+1;


        if(total>=5)
        {
            score+=100000;
        }
        else if(total==4)
        {
            score+=10000;
        }
        else if(total==3)
        {
            score+=1000;
        }
        else if(total==2)
        {
            score+=100;
        }

    }


    board.removeChess(row,col);


    return score;

}
