#include "chess.h"
#include <string.h>
#include <stdio.h>

namespace
{
// 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16
// 01 ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
// 02 ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
// 03 ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
// 04 ** ** ** 00 00 00 00 00 00 00 00 00 ** ** ** **
// 05 ** ** ** 00 00 00 00 00 00 00 00 00 ** ** ** **
// 06 ** ** ** 00 00 00 00 00 00 00 00 00 ** ** ** **
// 07 ** ** ** 00 00 00 00 00 00 00 00 00 ** ** ** **
// 08 ** ** ** 00 00 00 00 00 00 00 00 00 ** ** ** **
// 09 ** ** ** 00 00 00 00 00 00 00 00 00 ** ** ** **
// 10 ** ** ** 00 00 00 00 00 00 00 00 00 ** ** ** **
// 11 ** ** ** 00 00 00 00 00 00 00 00 00 ** ** ** **
// 12 ** ** ** 00 00 00 00 00 00 00 00 00 ** ** ** **
// 13 ** ** ** 00 00 00 00 00 00 00 00 00 ** ** ** **
// 14 ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
// 15 ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
// 16 ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **

//各种棋子走法数组
short king_dir[8]    = {-0x10,	-0x01,	+0x01,	+0x10,	0,		0,		0,		0       };  //将
short advisor_dir[8] = {-0x11,	-0x0f,	+0x0f,	+0x11,	0,		0,		0,		0       };	//士
short bishop_dir[8]  = {-0x22,	-0x1e,	+0x1e,	+0x22,	0,		0,		0,		0       };	//象
short knight_dir[8]  = {-0x21,	-0x1f,	-0x12,	-0x0e,	+0x0e,	+0x12,	+0x1f,	+0x21   };  //马
short rook_dir[8]    = {-0x01,	+0x01,	-0x10,	+0x10,	0,		0,		0,		0       };	//车
short cannon_dir[8]  = {-0x01,	+0x01,	-0x10,	+0x10,	0,		0,		0,		0       };	//炮
short pawn_dir[2][8] =
{
    {-0x01,		+0x01,	-0x10,	0,		0,		0,		0,		0},
    {-0x01,		+0x01,	+0x10,	0,		0,		0,		0,		0}
};		//兵

short knight_check[8] = {-0x10,-0x10,-0x01,+0x01,-0x01,+0x01,+0x10,+0x10};//马腿位置
short bishop_check[8] = {-0x11,-0x0f,+0x0f,+0x11,0,0,0,0};	//象眼位置

//各种棋子合理位置数组
unsigned char legal_position[2][256] ={
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
        0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
        0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
        0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
        0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
        0, 0, 0, 9, 1,25, 1, 9, 1,25, 1, 9, 0, 0, 0, 0,
        0, 0, 0, 9, 1, 9, 1, 9, 1, 9, 1, 9, 0, 0, 0, 0,
        0, 0, 0, 17, 1, 1, 7, 19, 7, 1, 1, 17, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 3, 7, 3, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 17, 7, 3, 7, 17, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 17, 7, 3, 7, 17, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 3, 7, 3, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 17, 1, 1, 7, 19, 7, 1, 1, 17, 0, 0, 0, 0,
        0, 0, 0, 9, 1, 9, 1, 9, 1, 9, 1, 9, 0, 0, 0, 0,
        0, 0, 0, 9, 1,25, 1, 9, 1,25, 1, 9, 0, 0, 0, 0,
        0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
        0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
        0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
        0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
        0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};

unsigned char position_mask[7] = {2, 4, 16, 1, 1, 1, 8};

char getPieceTypeFromChar(char ch)
{
    switch(ch)
    {
    case 'k':
    case 'K':return 0;
    case 'a':
    case 'A':return 1;
    case 'b':
    case 'B':return 2;
    case 'n':
    case 'N':return 3;
    case 'r':
    case 'R':return 4;
    case 'c':
    case 'C':return 5;
    case 'p':
    case 'P':return 6;
    default:return 7;
    }
}

char pieceToFenChar(uint8_t ch)
{
    if(ch <32)
    {
        switch(ch)
        {
        case 16:	return 'K';
        case 17:
        case 18:	return 'A';
        case 19:
        case 20:	return 'B';
        case 21:
        case 22:	return 'N';
        case 23:
        case 24:	return 'R';
        case 25:
        case 26:	return 'C';
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:	return 'P';
        default:	return 0;
        }
    }
    else
    {
        ch = ch-16;
        switch(ch)
        {
        case 16:	return 'k';
        case 17:
        case 18:	return 'a';
        case 19:
        case 20:	return 'b';
        case 21:
        case 22:	return 'n';
        case 23:
        case 24:	return 'r';
        case 25:
        case 26:	return 'c';
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:	return 'p';
        default:	return 0;
        }
    }
}

} //namespace

Chess::Chess()
{
    init();
}

void Chess::init()
{
    loadFromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1");
    genAllMove();
}

void Chess::clearBoard()
{
    memset(board_, 0, sizeof(board_));
    memset(piece_, 0, sizeof(piece_));
    side_ = 0;
    moves_size_ = 0;
    std::stack<Move> empty_stack;
    move_stack_.swap(empty_stack);
}

int Chess::loadFromFen(const char * fen)
{
    clearBoard();

    char red[7] = {16,17,19,21,23,25,27};
    char black[7] = {32,33,35,37,39,41,43};
    const char *str = fen;

    int i = 3;
    int j = 3;
    int k;
    int result = 0;
    while(true)
    {
        switch(*str)
        {
        case '/':
        {
            if( i > 11 || j != 12)
            {
                result  = -1;
                break;
            }
            j = 3;
            ++i;
        }
            break;
        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
        {
            j += *str - '0';
            if(j > 12)
            {
                result = -1;
                break;
            }
        }
            break;
        case 'K': case 'A': case 'B': case 'N':
        case 'R': case 'C': case 'P':
        {
            if(j > 11)
            {
                result = -1;
                break;
            }
            k = getPieceTypeFromChar(*str);

            if(k >= 7 || red[k] >= 32)
            {
                result = -1;
                break;
            }
            addPiece((i<<4)+j, red[k]);
            ++red[k];
            ++j;

        }
            break;
        case 'k': case 'a': case 'b': case 'n':
        case 'r': case 'c': case 'p':
        {
            if(j > 11)
            {
                result = -1;
                break;
            }
            k = getPieceTypeFromChar(*str);

            if(k >= 7 || black[k] >= 48)
            {
                result = -1;
                break;
            }
            addPiece((i<<4)+j, black[k]);
            ++black[k];
            ++j;
        }
            break;
        case ' ':
            if(i != 11 && j != 12)
                result = -1;
            else
                result = 1;
            break;
        default:
            result = -1;
            break;
        }
        if(result == -1)
            return result;
        else if(result == 1)
            break;
        else
            ++str;
    }
    ++str;
    side_ = (*str == 'b' ? 1: 0);
    return 1;
}

void Chess::addPiece(uint8_t pos, uint8_t  p)
{
    board_[pos] = p;
    piece_[p] = pos;
}

void Chess::changeSide()
{
    side_ = 1 - side_;
}

//检测side一方是否被将军，是被将军返回1，否则返回0
bool Chess::check()
{
    unsigned char wKing,bKing; //红黑双方将帅的位置
    unsigned char p,q;
    int r;	//r=1表示将军，否则为0
    int s = 32 - side_ * 16;	//此处表示side_对方的将的值
    int fSide = 1-side_;	//对方标志
    int i;
    int PosAdd;	//位置增量

    wKing = piece_[16];
    bKing = piece_[32];
    if(!wKing || !bKing)
        return false;

    //检测将帅是否照面
    r=1;
    if(wKing%16 == bKing%16)
    {
        for(wKing=wKing-16; wKing!=bKing; wKing=wKing-16)
        {
            if(board_[wKing])
            {
                r=0;
                break;
            }
        }
        if(r)
            return true;	//将帅照面
    }

    q = piece_[48-s];	//side_方将的位置

    //检测将是否被马攻击
    int k;
    unsigned char n;//下一步可能行走的位置
    unsigned char m;//马腿位置

    for(i=5;i<=6;i++)
    {
        p = piece_[s + i];
        if(!p)
            continue;
        for(k=0; k<8; k++)//8个方向
        {
            n = p + knight_dir[k];	//n为新的可能走到的位置
            if(n!=q)
                continue;

            if(legal_position[fSide][n] & position_mask[3])	//马将对应下标为3
            {
                m = p + knight_check[k];
                if(!board_[m])	//马腿位置无棋子占据
                {
                    return true;
                }
            }
        }
    }

    //检测将是否被车攻击
    r=1;
    for(i=7;i<=8;i++)
    {
        p = piece_[s + i];
        if(!p)
            continue;
        if(p%16 == q%16)	//在同一纵线上
        {
            PosAdd = (p>q?-16:16);
            for(p=p+PosAdd; p!=q; p = p+PosAdd)
            {
                if(board_[p])	//车将中间有子隔着
                {
                    r=0;
                    break;
                }
            }
            if(r)
                return true;
        }
        else if(p/16 ==q/16)	//在同一横线上
        {
            PosAdd = (p>q?-1:1);
            for(p=p+PosAdd; p!=q; p = p+PosAdd)
            {
                if(board_[p])
                {
                    r=0;
                    break;
                }
            }
            if(r)
                return true;
        }
    }

    //检测将是否被炮攻击
    int overFlag = 0;	//翻山标志
    for(i=9;i<=10;i++)
    {
        p = piece_[s + i];
        if(!p)
            continue;
        if(p%16 == q%16)	//在同一纵线上
        {
            PosAdd = (p>q?-16:16);
            for(p=p+PosAdd; p!=q; p = p+PosAdd)
            {
                if(board_[p])
                {
                    if(!overFlag)	//隔一子
                        overFlag = 1;
                    else			//隔两子
                    {
                        overFlag = 2;
                        break;
                    }
                }
            }
            if(overFlag==1)
                return true;
        }
        else if(p/16 ==q/16)	//在同一横线上
        {
            PosAdd = (p>q?-1:1);
            for(p=p+PosAdd; p!=q; p = p+PosAdd)
            {
                if(board_[p])
                {
                    if(!overFlag)
                        overFlag = 1;
                    else
                    {
                        overFlag = 2;
                        break;
                    }
                }
            }
            if(overFlag==1)
                return true;
        }
    }

    //检测将是否被兵攻击
    for(i=11;i<=15;i++)
    {
        p = piece_[s + i];
        if(!p)
            continue;
        for(k=0; k<3; k++)//3个方向
        {
            n = p + pawn_dir[fSide][k];	//n为新的可能走到的位置
            if((n==q) && (legal_position[fSide][n] & position_mask[6]))	//兵士将对应下标为6
            {
                return true;
            }
        }
    }

    return false;
}

void Chess::saveMove(uint8_t from, uint8_t to)
{
    unsigned char p;

    p = board_[to];
    piece_[board_[from]] = to;
    if(p)
        piece_[p]=0;
    board_[to] = board_[from];
    board_[from] = 0;

    int r = check();
    board_[from] = board_[to];
    board_[to] = p;
    piece_[board_[from]] = from;
    if(p)
        piece_[p] = to;

    if(!r)
    {
        moves_[moves_size_].from = from;
        moves_[moves_size_].to = to;
        ++moves_size_;
    }
}

size_t Chess::getFenStr(char * str, size_t size)
{
    int i, j, k, pc;

    char * begin = str;
    char * end = str + size - 1;

    for (i = 3; i <= 12; i ++)
    {
        k = 0;
        for (j = 3; j <= 11; j ++)
        {
            pc = board_[(i << 4) + j];
            if (pc != 0)
            {
                if (k > 0)
                {
                    *str = k + '0';
                    if(++str > end)
                        return size;
                    k = 0;
                }
                *str = pieceToFenChar(pc);
                str ++;
            }
            else
            {
                k ++;
            }
        }
        if (k > 0)
        {
            *str = k + '0';
            if(++str > end)
                return size;
        }
        *str = '/';
        if(++str > end)
            return size;
    }
    --str;
    *str = ' ';
    ++str;
    *str = (side_ == 0 ? 'w' : 'b');

    if(++str > end)
        return size;
    *str = '\0';
    return str - begin -1;
}

bool Chess::move(Move mv)
{
    if(!isLegalMove(mv))
        return false;
    unsigned char p;

    p = board_[mv.to];
    piece_[board_[mv.from]] = mv.to;
    if(p)
        piece_[p]=0;
    board_[mv.to] = board_[mv.from];
    board_[mv.from] = 0;

    changeSide();
    genAllMove();

    mv.capture = p;
    move_stack_.push(mv);

    return true;
}

bool Chess::undo()
{
    if(move_stack_.empty())
        return false;
    Move mv = move_stack_.top();
    move_stack_.pop();

    board_[mv.from] = board_[mv.to];
    board_[mv.to] = mv.capture;

    piece_[board_[mv.from]] = mv.from;
    if(mv.capture)
        piece_[board_[mv.to]] = mv.to;
    changeSide();
    genAllMove();

    return true;
}

int Chess::getChessResult()
{
    int s = (side_+1)*16;
    if(!piece_[s] || moves_size_ == 0)
    {
        return 1 - side_;
    }
    else
    {
        return 3;
    }
}

void Chess::genAllMove()
{
    moves_size_ = 0;

    short i,j,k;
    unsigned char p;	//p:棋子位置
    unsigned char n;	//下一步可能行走的位置
    unsigned char m;	//马腿、象眼位置
    int s = (side_+1)*16;		//走棋方，经方16，黑方32
    int overFlag;		//炮的翻山标志

    p = piece_[s];	//将的位置
    if(!p)
        return;

    //将的走法
    for(k=0; k<4; k++)//4个方向
    {
        n = p + king_dir[k];	//n为新的可能走到的位置
        if(legal_position[side_][n] & position_mask[0])	//将对应下标为0
        {
            if( !(board_[n] & s))	//目标位置上没有本方棋子
                saveMove(p, n);
        }
    }

    //士的走法
    for(i=1; i<=2; i++)
    {
        p = piece_[s + i];
        if(!p)
            continue;
        for(k=0; k<4; k++)//4个方向
        {
            n = p + advisor_dir[k];	//n为新的可能走到的位置
            if(legal_position[side_][n] & position_mask[1])	//士将对应下标为1
            {
                if( !(board_[n] & s))	//目标位置上没有本方棋子
                    saveMove(p, n);
            }
        }
    }

    //象的走法
    for(i=3; i<=4; i++)
    {
        p = piece_[s + i];
        if(!p)
            continue;
        for(k=0; k<4; k++)//4个方向
        {
            n = p + bishop_dir[k];	//n为新的可能走到的位置
            if(legal_position[side_][n] & position_mask[2])	//象将对应下标为2
            {
                m = p + bishop_check[k];
                if(!board_[m])	//象眼位置无棋子占据
                {
                    if( !(board_[n] & s))	//目标位置上没有本方棋子
                        saveMove(p, n);
                }
            }
        }
    }

    //马的走法
    for(i=5; i<=6; i++)
    {
        p = piece_[s + i];
        if(!p)
            continue;
        for(k=0; k<8; k++)//8个方向
        {
            n = p + knight_dir[k];	//n为新的可能走到的位置
            if(legal_position[side_][n] & position_mask[3])	//马将对应下标为3
            {
                m = p + knight_check[k];
                if(!board_[m])	//马腿位置无棋子占据
                {
                    if( !(board_[n] & s))	//目标位置上没有本方棋子
                        saveMove(p, n);
                }
            }
        }
    }

    //车的走法
    for(i=7; i<=8; i++)
    {
        p = piece_[s + i];
        if(!p)
            continue;
        for(k=0; k<4; k++)	//4个方向
        {
            for(j=1; j<10; j++)	//横的最多有8个可能走的位置，纵向最多有9个位置
            {
                n = p + j * rook_dir[k];
                if(!(legal_position[side_][n] & position_mask[4]))	//车士将对应下标为4
                    break;//不合理的位置
                if(! board_[n] )	//目标位置上无子
                {
                    saveMove(p, n);
                }
                else if ( board_[n] & s)	//目标位置上有本方棋子
                    break;
                else	//目标位置上有对方棋子
                {
                    saveMove(p, n);
                    break;
                }
            }
        }
    }

    //炮的走法
    for(i=9; i<=10; i++)
    {
        p = piece_[s + i];
        if(!p)
            continue;
        for(k=0; k<4; k++)	//4个方向
        {
            overFlag = 0;
            for(j=1; j<10; j++)	//横的最多有8个可能走的位置，纵向最多有9个位置
            {
                n = p + j * cannon_dir[k];
                if(!(legal_position[side_][n] & position_mask[5]))	//炮士将对应下标为5
                    break;//不合理的位置
                if(! board_[n] )	//目标位置上无子
                {
                    if(!overFlag)	//未翻山
                        saveMove(p, n);
                    //已翻山则不作处理，自动考察向下一个位置
                }
                else//目标位置上有子
                {
                    if (!overFlag)	//未翻山则置翻山标志
                        overFlag = 1;
                    else	//已翻山
                    {
                        if(! (board_[n] & s))//对方棋子
                            saveMove(p, n);
                        break;	//不论吃不吃子，都退出此方向搜索
                    }
                }
            }
        }
    }

    //兵的走法
    for(i=11; i<=15; i++)
    {
        p = piece_[s + i];
        if(!p)
            continue;
        for(k=0; k<3; k++)//3个方向
        {
            n = p + pawn_dir[side_][k];	//n为新的可能走到的位置
            if(legal_position[side_][n] & position_mask[6])	//兵士将对应下标为6
            {
                if( !(board_[n] & s))	//目标位置上没有本方棋子
                    saveMove(p, n);
            }
        }
    }
}

bool Chess::isLegalMove(Move mv)
{
    for(size_t i=0; i < moves_size_; ++i)
    {
        if(mv.from == moves_[i].from && mv.to == moves_[i].to)
            return true;
    }
    return false;
}


void Chess::outputBoard()
{
    for(int i=1; i<=256; i++)
    {
        printf("%3d", board_[i-1]);
        if(i%16==0)
            printf("\n");
    }
}

void Chess::outputPiece()
{
    int i;
    printf("piece_: \n");
    for(i=0;i<16;i++)
        printf("%4d",piece_[i]);
    printf("\n");
    for(i=16;i<32;i++)
        printf("%4d",piece_[i]);
    printf("\n");
    for(i=32;i<48;i++)
        printf("%4d",piece_[i]);
    printf("\n");
}

void Chess::outputAllMove()
{
    printf("output move start\n");
    for(size_t i = 0; i < moves_size_; ++i)
    {
        printf("(%3d,%3d)\n", moves_[i].from, moves_[i].to);
    }
    printf("output move end,total %d\n", (int)moves_size_);
}


