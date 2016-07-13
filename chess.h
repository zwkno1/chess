#ifndef CHESS_H
#define CHESS_H

#include <stack>
#include <vector>
#include <stdint.h>
#include <stddef.h>

struct Move
{
    uint8_t from;
    uint8_t to;
    uint8_t capture;
};

class Chess
{
public:
    Chess();

    uint8_t getPiecePos(uint8_t p) const { return p >=48 ? 0 : piece_[p]; }
    uint8_t getSide() const { return side_; }
    uint8_t getPiece(uint8_t pos) const { return board_[pos]; }

    //初始化
    void init();

    //将FEN串表示的局面转换成一维数组
    int loadFromFen(const char * fen);

    //将局面转换成FEN串
    size_t getFenStr(char * str, size_t size);

    //检测side一方是否被将军，是被将军返回true，否则返回false
    bool check();

    //走棋
    bool move(Move mv);

    //悔棋
    bool undo();

    void search(int depth);

    Move getBestMove() const { return best_move_; }

    //检查对局是否结束
    // 0 红胜, 1 黑胜, 2 平局, 3 未结束
    int getChessResult();

    //判断走法是否合理
    bool isLegalMove(Move mv);

    //输出棋盘数组
    void outputBoard();

    //输出棋子数组
    void outputPiece();

    //输出所有走法
    void outputAllMove();

private:
    //清空棋盘数组
    void clearBoard();

    //增加棋子
    void addPiece(uint8_t pos, uint8_t  p);

    //保存走法
    void saveMove(std::vector<Move> & moves, uint8_t from, uint8_t to);

    //改变走棋方
    void changeSide();

    //生成所有走法
    void genAllMove(std::vector<Move> & moves);

    int eval();

    int maxSearch(int depth);

    int minSearch(int depth);

    void makeMove(Move mv);

    void unmakeMove();

    uint8_t board_[256];	    // 棋盘数组
    uint8_t piece_[48];         // 棋子数组

    uint8_t side_;              // 轮到哪方走，0表示红方，1表示黑方

    std::vector<Move> moves_;   // 当前的走法

    std::stack<Move> move_stack_;

    Move best_move_;
    int max_depth_;
};




#endif // CHESS_H
