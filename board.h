#ifndef BOARD_H
#define BOARD_H
//创建一个棋盘类
class  Board{
    private:
    //保存棋子
    int chess[15][15];
    public:
    //棋盘大小
    static const int BOARD_SIZE = 15;


    //每个格子的距离
    static const int GRID = 40;


    //棋盘左上角位置
    static const int START_X = 20;
    static const int START_Y = 20;
    //构造函数
    Board();
    //初始化，清空棋盘
    void init();
    //画线画棋子
    void draw();
    //增加落子函数
    bool placeChess(int row,int col,int player);
    //增加判断输赢函数
    bool checkWin(int row,int col,int player);
    //清空棋盘
    void clear();
}
;
#endif