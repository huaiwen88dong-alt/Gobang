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
    //先模拟在当前位置落下一颗棋
    board.placeChess(row,col,player);

    int score=0;

    //记录这一步棋在四个方向上形成的高级棋型数量
    int liveThree=0;  //活三数量
    int rushFour=0;   //冲四数量
    int liveFour=0;   //活四数量

    //五子棋只需要判断四个方向：
    //竖直、水平、主对角线、副对角线
    int directions[4][2]=
    {
        {1,0},
        {0,1},
        {1,1},
        {1,-1}
    };

    //分别评价四个方向
    for(int i=0;i<4;i++)
    {
        int dx=directions[i][0];
        int dy=directions[i][1];

        //evaluateDirection不仅返回该方向的分数，
        //还会通过引用参数记录活三、冲四、活四的数量
        score+=evaluateDirection(board,row,col,dx,dy,player,liveThree,rushFour,liveFour);
    }

    //如果一次落子同时形成两个及以上活三，
    //说明对手很难同时防守两个方向，因此给予额外奖励
    if(liveThree>=2)
    {
        score+=50000;
    }

    //如果一次落子同时形成冲四和活三，
    //对手通常必须优先防守冲四，而活三可以继续制造威胁，
    //因此给予较高的组合奖励
    if(rushFour>=1&&liveThree>=1)
    {
        score+=100000;
    }

    //活四本身已经是极强威胁，
    //这里额外提高其优先级，避免AI放弃明显的制胜机会
    if(liveFour>=1)
    {
        score+=200000;
    }

    //评价完成后撤销刚才模拟的棋子，
    //不能让模拟落子真正影响棋盘
    board.removeChess(row,col);

    return score;
}
//判断某个方向的棋型价值
int AI::evaluateDirection(Board& board,int row,int col,int dx,int dy,int player,int& liveThree,  int& rushFour,int & liveFour)
{

    int count1 =countDirection(board,row,col,dx,dy,player);


    int count2 =countDirection(board,row,col,-dx,-dy,player);


    int total=count1+count2+1;


    //五连
    if(total>=5)
    {
        return 1000000;
    }


    //判断两端是否开放

    int open=0;

    //判断正方向是否开放
    int x=row+(count1+1)*dx;
    int y=col+(count1+1)*dy;


    if(x>=0&&x<Board::BOARD_SIZE
       &&y>=0&&y<Board::BOARD_SIZE
       &&board.getChess(x,y)==0)
    {
        open++;
    }


    x=row-(count2+1)*dx;
    y=col-(count2+1)*dy;

     //判断反方向是否开放
    if(x>=0&&x<Board::BOARD_SIZE
       &&y>=0&&y<Board::BOARD_SIZE
       &&board.getChess(x,y)==0)
    {
        open++;
    }


    //活四
    if(total==4&&open==2)
    {
        liveFour++;
        return 100000;
    }


    //冲四
    if(total==4&&open==1)
    {
        rushFour++;
        return 10000;
    }


    //活三
    if(total==3&&open==2)
    {
        liveThree++;
        return 5000;
    }


    //眠三
    if(total==3&&open==1)
    {
        return 1000;
    }


    //活二
    if(total==2&&open==2)
    {
        return 500;
    }


    return 0;

}